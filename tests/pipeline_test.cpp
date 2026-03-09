#include "db_connector.hpp"
#include "stream_processor.hpp"

#include <chrono>
#include <cstdlib>
#include <vector>

#include <gtest/gtest.h>

#ifdef OLYMPUS_WITH_PQXX
#include <pqxx/pqxx>
#endif

TEST(PipelineTest, InjectedCdcEventIsMappedToSignal) {
	olympus::DBConnector connector;
	connector.configure_database(olympus::Domain::Resources, "postgres://stub/resources");
	ASSERT_TRUE(connector.connect_all());

	olympus::StreamProcessor stream(1337U, &connector);
	ASSERT_TRUE(stream.start_cdc({olympus::Domain::Resources}, "olympus_cdc"));

	olympus::CDCEvent event{};
	event.domain = olympus::Domain::Resources;
	event.channel = "resources_events";
	event.payload = "type=water_level;urgency=0.92;value=17";
	connector.inject_cdc_for_test(event);

	const auto signals = stream.poll();
	ASSERT_EQ(signals.size(), 1U);
	EXPECT_EQ(signals.front().domain, olympus::Domain::Resources);
	EXPECT_EQ(signals.front().source_table, "resources_events");
	EXPECT_NEAR(signals.front().urgency, 0.92, 1e-6);
}

TEST(PipelineTest, LiveNotifyRoundTripIfConfigured) {
#ifndef OLYMPUS_WITH_PQXX
	GTEST_SKIP() << "Built without libpqxx support.";
#else
	const char* conn = std::getenv("OLYMPUS_TEST_PG_CONN");
	if (conn == nullptr || std::string(conn).empty()) {
		GTEST_SKIP() << "Set OLYMPUS_TEST_PG_CONN to run live CDC test.";
	}

	olympus::DBConnector connector;
	connector.configure_database(olympus::Domain::Resources, conn);
	ASSERT_TRUE(connector.connect_all());
	ASSERT_TRUE(connector.listen(olympus::Domain::Resources, "olympus_cdc_test"));

	pqxx::connection writer{conn};
	{
		pqxx::work tx{writer};
		tx.exec("NOTIFY olympus_cdc_test, 'urgency=0.77;table=resources_events'");
		tx.commit();
	}

	const auto events = connector.poll_cdc(std::chrono::milliseconds(250));
	ASSERT_FALSE(events.empty());
	EXPECT_EQ(events.front().domain, olympus::Domain::Resources);
	EXPECT_EQ(events.front().channel, "olympus_cdc_test");
#endif
}

#include "normalizer.hpp"
#include "prometheus.hpp"
#include "themis.hpp"
#include "validator.hpp"

#include <chrono>

#include <gtest/gtest.h>

TEST(IntegrationLatencyTest, SignalToProposalToSimulationUnderBudget) {
  olympus::Validator validator;
  olympus::Normalizer normalizer;
  olympus::Themis themis(0.55);
  olympus::Prometheus prometheus(500, 90);

  olympus::Signal raw{};
  raw.signal_id = "lat-1";
  raw.domain = olympus::Domain::Resources;
  raw.type = "critical_event";
  raw.timestamp = std::chrono::system_clock::now();
  raw.urgency = 0.95;
  raw.payload = "urgency=0.95;drought=true";
  raw.source_db = "resources";
  raw.source_table = "water_levels";

  std::string error;
  ASSERT_TRUE(validator.validate(raw, error)) << error;
  const olympus::Signal signal = normalizer.normalize(raw);

  const auto begin = std::chrono::steady_clock::now();
  themis.ingest(signal);
  const auto proposal = themis.propose();
  ASSERT_TRUE(proposal.has_value());
  const auto report = prometheus.simulate(*proposal);
  const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::steady_clock::now() - begin);

  EXPECT_LT(elapsed.count(), 500) << "signal->proposal->simulation exceeded 500ms budget";
  EXPECT_GT(report.overall_confidence, 0.0);
  EXPECT_FALSE(report.timeline.empty());
}

TEST(IntegrationLatencyTest, BatchSignalsAverageLatencyUnderBudget) {
  olympus::Themis themis(0.55);
  olympus::Prometheus prometheus(300, 60);

  constexpr int kBatch = 20;
  std::chrono::milliseconds total{0};

  for (int i = 0; i < kBatch; ++i) {
    olympus::Signal signal{};
    signal.signal_id = "lat-batch-" + std::to_string(i);
    signal.domain = (i % 2 == 0) ? olympus::Domain::Resources : olympus::Domain::Defense;
    signal.type = "cdc_event";
    signal.timestamp = std::chrono::system_clock::now();
    signal.urgency = 0.60 + (static_cast<double>(i % 5) * 0.07);
    signal.payload = "urgency=0.71";

    const auto begin = std::chrono::steady_clock::now();
    themis.ingest(signal);
    const auto proposal = themis.propose();
    ASSERT_TRUE(proposal.has_value());
    (void)prometheus.simulate(*proposal);
    total += std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - begin);
  }

  const auto avg = total / kBatch;
  EXPECT_LT(avg.count(), 300) << "Average pipeline->themis->prometheus latency is too high";
}

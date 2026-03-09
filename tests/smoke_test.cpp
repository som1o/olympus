#include "app.hpp"
#include "config.hpp"
#include "db_connector.hpp"
#include "prometheus.hpp"
#include "themis.hpp"

#include <gtest/gtest.h>

TEST(OlympusSmokeTest, InstantiateCoreNonAgentComponents) {
  olympus::SystemConfig cfg{};

  olympus::DBConnector db;
  db.configure_database(olympus::Domain::Resources, "postgres://stub/resources");
  ASSERT_TRUE(db.connect_all());

  olympus::Themis themis(cfg.proposal_confidence_floor);
  olympus::Prometheus prometheus(cfg.monte_carlo_trials, cfg.timeline_days);
  olympus::DashboardApp dashboard(cfg.auto_authorize_noncritical,
                                  cfg.refresh_ms,
                                  cfg.max_signals_per_tick);

  olympus::Signal signal{};
  signal.signal_id = "s-1";
  signal.domain = olympus::Domain::Resources;
  signal.urgency = 0.9;
  signal.type = "critical_event";

  themis.ingest(signal);
  const auto proposal = themis.propose();
  ASSERT_TRUE(proposal.has_value());

  const auto report = prometheus.simulate(*proposal);
  const bool approved = dashboard.process(signal, *proposal, report, themis.ledger_mutable(), db);
  (void)approved;
}

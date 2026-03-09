#include "app.hpp"
#include "config.hpp"
#include "db_connector.hpp"
#include "logger.hpp"
#include "normalizer.hpp"
#include "prometheus.hpp"
#include "stream_processor.hpp"
#include "themis.hpp"
#include "validator.hpp"

#include <array>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
  using namespace olympus;

  try {
    const SystemConfig config = ConfigLoader::load_from_file("config/olympus.toml");
    if (config.mode == "stub") {
      throw std::runtime_error("config mode=stub is no longer supported; set [system].mode = \"production\"");
    }

    Logger::log(LogCategory::System, config.name + " ONLINE");

    DBConnector db;
    static constexpr std::array<Domain, 8> domains{
        Domain::Defense, Domain::Governance, Domain::Education, Domain::Health,
        Domain::Local, Domain::Foreign, Domain::Resources, Domain::Technology};
    for (const auto domain : domains) {
      db.configure_database(domain, "postgres://stub/" + to_string(domain));
    }
    db.connect_all();

    StreamProcessor stream(config.signal_seed, &db);
    stream.start_cdc(std::vector<Domain>(domains.begin(), domains.end()), "olympus_cdc");
    Validator validator;
    Normalizer normalizer;
    Themis themis(config.proposal_confidence_floor);
    Prometheus prometheus(config.monte_carlo_trials, config.timeline_days);
    DashboardApp dashboard(config.auto_authorize_noncritical, config.refresh_ms,
                           config.max_signals_per_tick);
    return dashboard.run_live(stream, validator, normalizer, themis, prometheus, db);
  } catch (const std::exception& ex) {
    std::cerr << "fatal: " << ex.what() << '\n';
    return 1;
  }
}

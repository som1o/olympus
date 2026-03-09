#include "prometheus.hpp"

#include <algorithm>

namespace olympus {

Prometheus::Prometheus(const std::size_t monte_carlo_trials, const int horizon_days)
    : monte_carlo_(monte_carlo_trials), horizon_days_(horizon_days) {}

SimulationReport Prometheus::simulate(const DecisionProposal& proposal) const {
  SimulationReport report{};
  report.monte_carlo = monte_carlo_.run(proposal);
  report.cascades = cascade_.propagate(proposal);
  report.population = population_.run(proposal, horizon_days_);
  report.timeline = timeline_.build(report.cascades, horizon_days_);
  report.overall_confidence = std::clamp((report.monte_carlo.p50 + proposal.confidence) / 2.0, 0.0, 1.0);
  return report;
}

} // namespace olympus

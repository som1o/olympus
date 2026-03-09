#pragma once

#include "agent_sim.hpp"
#include "cascade_model.hpp"
#include "contracts.hpp"
#include "monte_carlo.hpp"
#include "timeline.hpp"

#include <vector>

namespace olympus {

struct SimulationReport {
  MonteCarloResult monte_carlo;
  std::vector<CascadeEvent> cascades;
  std::vector<PopulationEffect> population;
  std::vector<TimelineEvent> timeline;
  double overall_confidence{0.0};
};

class Prometheus {
public:
  Prometheus(std::size_t monte_carlo_trials, int horizon_days);
  SimulationReport simulate(const DecisionProposal& proposal) const;

private:
  MonteCarloEngine monte_carlo_;
  CascadeModel cascade_;
  PopulationSim population_;
  TimelineProjector timeline_;
  int horizon_days_;
};

} // namespace olympus

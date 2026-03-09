#include "agent_sim.hpp"

namespace olympus {

std::vector<PopulationEffect> PopulationSim::run(const DecisionProposal& proposal, const int horizon_days) const {
  const double scale = horizon_days >= 90 ? 1.0 : 0.5;
  return {
      {"CIVILIAN_URBAN", proposal.domain == Domain::Resources ? 0.18 * scale : 0.06 * scale},
      {"CIVILIAN_RURAL", proposal.domain == Domain::Resources ? 0.24 * scale : 0.08 * scale},
      {"BUSINESS_SECTOR", proposal.domain == Domain::Resources ? -0.07 * scale : -0.02 * scale},
  };
}

} // namespace olympus

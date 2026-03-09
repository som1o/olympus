#pragma once

#include "contracts.hpp"

#include <string>
#include <vector>

namespace olympus {

struct PopulationEffect {
  std::string segment;
  double delta{0.0};
};

class PopulationSim {
public:
  std::vector<PopulationEffect> run(const DecisionProposal& proposal, int horizon_days) const;
};

} // namespace olympus

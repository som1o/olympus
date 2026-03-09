#pragma once

#include "contracts.hpp"

#include <string>
#include <vector>

namespace olympus {

struct MonteCarloResult {
  double p10{0.0};
  double p50{0.0};
  double p90{0.0};
  std::string summary;
};

class MonteCarloEngine {
public:
  explicit MonteCarloEngine(std::size_t trials);
  MonteCarloResult run(const DecisionProposal& proposal) const;

private:
  std::size_t trials_;
};

} // namespace olympus

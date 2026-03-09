#pragma once

#include "contracts.hpp"

#include <string>

namespace olympus {

class ServiceOptimizer {
public:
  void bootstrap();
  std::string optimize(const DecisionProposal& proposal, double urgency) const;

private:
  bool bootstrapped_{false};
};

} // namespace olympus

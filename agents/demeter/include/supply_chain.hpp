#pragma once

#include "contracts.hpp"

#include <string>

namespace olympus {

class SupplyChain {
public:
  void bootstrap();
  std::string continuity_plan(const DecisionProposal& proposal, double strain) const;

private:
  bool bootstrapped_{false};
};

} // namespace olympus

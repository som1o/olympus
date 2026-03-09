#pragma once

#include "contracts.hpp"

#include <string>

namespace olympus {

class BudgetAllocator {
public:
  void bootstrap();
  double allocation_ratio(const DecisionProposal& proposal) const;
  std::string summary() const;

private:
  bool bootstrapped_{false};
  double reserve_ratio_{0.2};
};

} // namespace olympus

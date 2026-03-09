#pragma once

#include "contracts.hpp"

#include <string>

namespace olympus {

class ResourceAllocator {
public:
  void bootstrap();
  std::string allocation_plan(const DecisionProposal& proposal, double icu_pressure, bool active_outbreak) const;

private:
  bool bootstrapped_{false};
};

} // namespace olympus

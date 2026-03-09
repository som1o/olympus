#pragma once

#include "contracts.hpp"

#include <string>

namespace olympus {

class CurriculumOptimizer {
public:
  void bootstrap();
  std::string optimize(const DecisionProposal& proposal, double enrollment_pressure) const;

private:
  bool bootstrapped_{false};
};

} // namespace olympus

#pragma once

#include "contracts.hpp"

#include <string>

namespace olympus {

class TacticalPlanner {
public:
  void bootstrap();
  std::string plan(const DecisionProposal& proposal, double threat_score);
  std::string summary() const;

private:
  bool bootstrapped_{false};
  std::string last_plan_;
};

} // namespace olympus

#pragma once

#include "contracts.hpp"

#include <string>

namespace olympus {

class TradeAnalyzer {
public:
  void bootstrap();
  std::string impact_brief(const DecisionProposal& proposal, double tension_score) const;

private:
  bool bootstrapped_{false};
};

} // namespace olympus

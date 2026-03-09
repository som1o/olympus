#pragma once

#include "contracts.hpp"

namespace olympus {

class RiskScorer {
public:
  RiskAssessment assess(const DecisionProposal& proposal) const;
};

} // namespace olympus

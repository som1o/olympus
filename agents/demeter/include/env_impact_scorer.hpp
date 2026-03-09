#pragma once

#include "contracts.hpp"

#include <string>

namespace olympus {

class EnvImpactScorer {
public:
  void bootstrap();
  double score(const DecisionProposal& proposal, double strain) const;
  std::string summary() const;

private:
  bool bootstrapped_{false};
};

} // namespace olympus

#pragma once

#include "contracts.hpp"
#include "signal.hpp"

#include <string>

namespace olympus {

class DiplomaticScorer {
public:
  void bootstrap();
  void ingest(const Signal& signal);
  double score(const DecisionProposal& proposal) const;
  std::string summary() const;

private:
  bool bootstrapped_{false};
  double tension_score_{0.0};
};

} // namespace olympus

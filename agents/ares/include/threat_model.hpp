#pragma once

#include "contracts.hpp"
#include "signal.hpp"

#include <cstddef>
#include <string>

namespace olympus {

class ThreatModel {
public:
  void bootstrap();
  void ingest(const Signal& signal);
  double score(const DecisionProposal& proposal) const;
  std::string summary() const;

private:
  bool bootstrapped_{false};
  std::size_t samples_{0};
  std::size_t critical_events_{0};
  double rolling_urgency_{0.0};
};

} // namespace olympus

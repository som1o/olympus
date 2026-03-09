#pragma once

#include "contracts.hpp"
#include "signal.hpp"

#include <string>

namespace olympus {

class PolicyEngine {
public:
  void bootstrap();
  void ingest(const Signal& signal);
  std::string recommend(const DecisionProposal& proposal) const;
  std::string summary() const;

private:
  bool bootstrapped_{false};
  double policy_pressure_{0.0};
  std::string last_signal_type_;
};

} // namespace olympus

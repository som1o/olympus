#pragma once

#include "contracts.hpp"
#include "signal.hpp"

#include <string>

namespace olympus {

class CyberSentinel {
public:
  void bootstrap();
  void ingest(const Signal& signal);
  double threat_index(const DecisionProposal& proposal) const;
  std::string summary() const;

private:
  bool bootstrapped_{false};
  double threat_{0.0};
};

} // namespace olympus

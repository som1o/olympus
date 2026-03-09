#pragma once

#include "contracts.hpp"
#include "signal.hpp"

#include <string>

namespace olympus {

class EpidemicModel {
public:
  void bootstrap();
  void ingest(const Signal& signal);
  double outbreak_pressure(const DecisionProposal& proposal) const;
  std::string summary() const;

private:
  bool bootstrapped_{false};
  double transmission_index_{0.0};
};

} // namespace olympus

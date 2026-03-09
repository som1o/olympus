#pragma once

#include "contracts.hpp"

#include <string>

namespace olympus {

class TechRoadmap {
public:
  void bootstrap();
  std::string next_step(const DecisionProposal& proposal, double threat_index, double stability) const;

private:
  bool bootstrapped_{false};
};

} // namespace olympus

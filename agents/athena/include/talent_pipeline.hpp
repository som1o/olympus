#pragma once

#include "contracts.hpp"

#include <string>

namespace olympus {

class TalentPipeline {
public:
  void bootstrap();
  std::string forecast(const DecisionProposal& proposal, double momentum) const;

private:
  bool bootstrapped_{false};
};

} // namespace olympus

#pragma once

#include "contracts.hpp"

#include <string>

namespace olympus {

class MunicipalPlanner {
public:
  void bootstrap();
  std::string build_plan(const DecisionProposal& proposal, std::size_t surges) const;

private:
  bool bootstrapped_{false};
};

} // namespace olympus

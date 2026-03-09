#pragma once

#include "contracts.hpp"

#include <string>

namespace olympus {

class DecisionReview {
public:
  bool is_critical(const DecisionProposal& proposal) const;
  std::string summarize(const DecisionProposal& proposal) const;
};

} // namespace olympus

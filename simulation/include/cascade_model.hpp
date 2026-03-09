#pragma once

#include "contracts.hpp"

#include <string>
#include <vector>

namespace olympus {

struct CascadeEvent {
  Domain domain{Domain::Unknown};
  double magnitude{0.0};
  int lag_days{0};
  std::string description;
};

class CascadeModel {
public:
  std::vector<CascadeEvent> propagate(const DecisionProposal& proposal) const;
};

} // namespace olympus

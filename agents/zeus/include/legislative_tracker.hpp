#pragma once

#include "contracts.hpp"

#include <cstddef>
#include <string>

namespace olympus {

class LegislativeTracker {
public:
  void bootstrap();
  void register_proposal(const DecisionProposal& proposal);
  std::string summary() const;

private:
  bool bootstrapped_{false};
  std::size_t tracked_{0};
  std::size_t critical_{0};
};

} // namespace olympus

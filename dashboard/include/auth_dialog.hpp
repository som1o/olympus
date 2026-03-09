#pragma once

#include "contracts.hpp"

#include <string>

namespace olympus {

class AuthDialog {
public:
  bool authorize(const DecisionProposal& proposal, bool auto_authorize_noncritical,
                 std::string& reason) const;
};

} // namespace olympus

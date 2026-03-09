#include "auth_dialog.hpp"

namespace olympus {

bool AuthDialog::authorize(const DecisionProposal& proposal, const bool auto_authorize_noncritical,
                           std::string& reason) const {
  if (proposal.risk_level == RiskLevel::Critical) {
    reason = "critical proposal requires manual approval";
    return false;
  }
  reason = auto_authorize_noncritical ? "auto-approved by dashboard policy"
                                      : "auto-approval disabled by policy";
  return auto_authorize_noncritical;
}

} // namespace olympus

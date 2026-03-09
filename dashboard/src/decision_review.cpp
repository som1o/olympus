#include "decision_review.hpp"

#include <sstream>

namespace olympus {

bool DecisionReview::is_critical(const DecisionProposal& proposal) const {
  return proposal.risk_level == RiskLevel::Critical;
}

std::string DecisionReview::summarize(const DecisionProposal& proposal) const {
  std::ostringstream out;
  out << "Proposal: " << proposal.proposal_id << '\n';
  out << "Domain: " << to_string(proposal.domain) << '\n';
  out << "Action: " << proposal.action_type << '\n';
  out << "Params: " << proposal.action_params << '\n';
  out << "Confidence: " << proposal.confidence << '\n';
  out << "Risk: " << to_string(proposal.risk_level) << '\n';
  out << "Justification: " << proposal.justification;
  return out.str();
}

} // namespace olympus

#include "risk_scorer.hpp"

namespace olympus {

RiskAssessment RiskScorer::assess(const DecisionProposal& proposal) const {
  RiskAssessment assessment{};
  assessment.cascade_probability = (1.0 - proposal.confidence) * 0.85;
  assessment.recovery_days = static_cast<int>(30 + (assessment.cascade_probability * 120));

  if (assessment.cascade_probability > 0.75) {
    assessment.level = RiskLevel::Critical;
  } else if (assessment.cascade_probability > 0.55) {
    assessment.level = RiskLevel::High;
  } else if (assessment.cascade_probability > 0.30) {
    assessment.level = RiskLevel::Medium;
  }

  assessment.domains_at_risk = {Domain::Governance, Domain::Resources, Domain::Local};
  return assessment;
}

} // namespace olympus

#include "env_impact_scorer.hpp"

#include <algorithm>

namespace olympus {

void EnvImpactScorer::bootstrap() { bootstrapped_ = true; }

double EnvImpactScorer::score(const DecisionProposal& proposal, const double strain) const {
	const double risk_weight = proposal.risk_level == RiskLevel::Critical ? 0.35
														 : proposal.risk_level == RiskLevel::High    ? 0.25
														 : proposal.risk_level == RiskLevel::Medium  ? 0.15
																																				 : 0.08;
	return std::clamp(strain * 0.7 + risk_weight, 0.0, 1.0);
}

std::string EnvImpactScorer::summary() const {
	return bootstrapped_ ? "env_scorer=online" : "env_scorer=offline";
}

} // namespace olympus

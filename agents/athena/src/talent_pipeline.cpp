#include "talent_pipeline.hpp"

#include <algorithm>

namespace olympus {

void TalentPipeline::bootstrap() { bootstrapped_ = true; }

std::string TalentPipeline::forecast(const DecisionProposal& proposal, const double momentum) const {
	const double signal = std::clamp((proposal.confidence + momentum) * 0.5, 0.0, 1.0);
	if (signal > 0.8) {
		return "high-confidence workforce uplift in 6-10 years";
	}
	if (signal > 0.55) {
		return "moderate uplift with targeted scholarships";
	}
	return "slow talent growth; recommend retention incentives";
}

} // namespace olympus

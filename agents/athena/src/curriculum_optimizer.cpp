#include "curriculum_optimizer.hpp"

#include <algorithm>

namespace olympus {

void CurriculumOptimizer::bootstrap() { bootstrapped_ = true; }

std::string CurriculumOptimizer::optimize(const DecisionProposal& proposal,
																					const double enrollment_pressure) const {
	const double pressure = std::clamp(enrollment_pressure, 0.0, 1.0);
	if (pressure > 0.8 || proposal.risk_level == RiskLevel::High ||
			proposal.risk_level == RiskLevel::Critical) {
		return "prioritize STEM core + teacher hiring fast-track";
	}
	if (pressure > 0.55) {
		return "rebalance curriculum hours toward literacy and numeracy";
	}
	return "maintain baseline curriculum with targeted pilot programs";
}

} // namespace olympus

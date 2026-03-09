#include "budget_allocator.hpp"

#include <algorithm>

namespace olympus {

void BudgetAllocator::bootstrap() {
	bootstrapped_ = true;
	reserve_ratio_ = 0.2;
}

double BudgetAllocator::allocation_ratio(const DecisionProposal& proposal) const {
	const double confidence_factor = std::clamp(proposal.confidence, 0.0, 1.0);
	const double risk_factor = proposal.risk_level == RiskLevel::Critical ? 0.30
														 : proposal.risk_level == RiskLevel::High    ? 0.20
														 : proposal.risk_level == RiskLevel::Medium  ? 0.10
																																				 : 0.05;
	return std::clamp((1.0 - reserve_ratio_) * confidence_factor + risk_factor, 0.05, 0.95);
}

std::string BudgetAllocator::summary() const {
	return "reserve_ratio=" + std::to_string(reserve_ratio_);
}

} // namespace olympus

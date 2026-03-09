#include "resource_allocator.hpp"

#include <algorithm>

namespace olympus {

void ResourceAllocator::bootstrap() { bootstrapped_ = true; }

std::string ResourceAllocator::allocation_plan(const DecisionProposal& proposal,
																							 const double icu_pressure,
																							 const bool active_outbreak) const {
	const double pressure = std::clamp(icu_pressure, 0.0, 1.0);
	if (active_outbreak || pressure >= 0.85 || proposal.risk_level == RiskLevel::Critical) {
		return "activate surge hospitals, reassign ICU teams, emergency vaccine release";
	}
	if (pressure >= 0.6 || proposal.risk_level == RiskLevel::High) {
		return "shift staff between regions and prioritize acute-care capacity";
	}
	return "maintain baseline allocation with contingency reserve";
}

} // namespace olympus

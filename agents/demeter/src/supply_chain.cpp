#include "supply_chain.hpp"

#include <algorithm>

namespace olympus {

void SupplyChain::bootstrap() { bootstrapped_ = true; }

std::string SupplyChain::continuity_plan(const DecisionProposal& proposal, const double strain) const {
	const double load = std::clamp(strain, 0.0, 1.0);
	if (load >= 0.85 || proposal.risk_level == RiskLevel::Critical) {
		return "release strategic reserves and secure emergency import corridors";
	}
	if (load >= 0.6) {
		return "rebalance regional depots and increase logistics throughput";
	}
	return "maintain baseline inventory and monitor transport bottlenecks";
}

} // namespace olympus

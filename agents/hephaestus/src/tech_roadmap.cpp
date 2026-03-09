#include "tech_roadmap.hpp"

#include <algorithm>

namespace olympus {

void TechRoadmap::bootstrap() { bootstrapped_ = true; }

std::string TechRoadmap::next_step(const DecisionProposal& proposal,
																	 const double threat_index,
																	 const double stability) const {
	const double threat = std::clamp(threat_index, 0.0, 1.0);
	if (threat > 0.8 || stability < 0.35 || proposal.risk_level == RiskLevel::Critical) {
		return "activate incident command, isolate critical networks, accelerate patch rollout";
	}
	if (threat > 0.55 || stability < 0.6) {
		return "increase monitoring depth and rebalance grid/infrastructure loads";
	}
	return "continue modernization roadmap with resilience hardening";
}

} // namespace olympus

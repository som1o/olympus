#include "service_optimizer.hpp"

#include <algorithm>

namespace olympus {

void ServiceOptimizer::bootstrap() { bootstrapped_ = true; }

std::string ServiceOptimizer::optimize(const DecisionProposal& proposal, const double urgency) const {
	const double level = std::clamp(urgency, 0.0, 1.0);
	if (level >= 0.8 || proposal.action_type.find("repair") != std::string::npos) {
		return "prioritize utilities uptime and emergency road repairs";
	}
	if (level >= 0.55) {
		return "optimize routing for field teams and reduce queue backlog";
	}
	return "baseline optimization with preventive maintenance windows";
}

} // namespace olympus

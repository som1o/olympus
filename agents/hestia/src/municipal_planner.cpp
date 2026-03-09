#include "municipal_planner.hpp"

namespace olympus {

void MunicipalPlanner::bootstrap() { bootstrapped_ = true; }

std::string MunicipalPlanner::build_plan(const DecisionProposal& proposal, const std::size_t surges) const {
	if (surges >= 4 || proposal.risk_level == RiskLevel::Critical) {
		return "declare municipal emergency and dispatch rapid works units";
	}
	if (surges >= 2 || proposal.risk_level == RiskLevel::High) {
		return "reprioritize maintenance crews and extend service hours";
	}
	return "maintain routine service schedule with targeted inspections";
}

} // namespace olympus

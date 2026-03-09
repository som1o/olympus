#include "tactical_planner.hpp"

#include <algorithm>

namespace olympus {

void TacticalPlanner::bootstrap() {
	bootstrapped_ = true;
	last_plan_.clear();
}

std::string TacticalPlanner::plan(const DecisionProposal& proposal, const double threat_score) {
	const double severity = std::clamp(threat_score, 0.0, 1.0);
	if (severity >= 0.85) {
		last_plan_ = "mobilize-reserve brigades, harden border ISR, raise cyber readiness";
		return last_plan_;
	}
	if (severity >= 0.60) {
		last_plan_ = "increase patrol cadence, stage rapid response teams, expand intelligence sweep";
		return last_plan_;
	}
	if (proposal.action_type.find("monitor") != std::string::npos) {
		last_plan_ = "maintain surveillance posture and issue advisory to command network";
		return last_plan_;
	}
	last_plan_ = "run readiness drills and preserve force posture";
	return last_plan_;
}

std::string TacticalPlanner::summary() const {
	if (!bootstrapped_) {
		return "planner=offline";
	}
	return last_plan_.empty() ? "planner=ready" : "last_plan=" + last_plan_;
}

} // namespace olympus

#include "legislative_tracker.hpp"

namespace olympus {

void LegislativeTracker::bootstrap() {
	bootstrapped_ = true;
	tracked_ = 0;
	critical_ = 0;
}

void LegislativeTracker::register_proposal(const DecisionProposal& proposal) {
	if (!bootstrapped_) {
		bootstrap();
	}
	++tracked_;
	if (proposal.risk_level == RiskLevel::Critical || proposal.risk_level == RiskLevel::High) {
		++critical_;
	}
}

std::string LegislativeTracker::summary() const {
	return "tracked=" + std::to_string(tracked_) + ",critical=" + std::to_string(critical_);
}

} // namespace olympus

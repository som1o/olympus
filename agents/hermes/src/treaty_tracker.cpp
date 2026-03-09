#include "treaty_tracker.hpp"

namespace olympus {

void TreatyTracker::bootstrap() {
	bootstrapped_ = true;
	tracked_ = 0;
	escalations_ = 0;
}

void TreatyTracker::register_proposal(const DecisionProposal& proposal) {
	if (!bootstrapped_) {
		bootstrap();
	}
	++tracked_;
	if (proposal.risk_level == RiskLevel::High || proposal.risk_level == RiskLevel::Critical) {
		++escalations_;
	}
}

std::string TreatyTracker::summary() const {
	return "tracked=" + std::to_string(tracked_) +
				 ",escalations=" + std::to_string(escalations_);
}

} // namespace olympus

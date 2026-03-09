#include "diplomatic_scorer.hpp"

#include <algorithm>

namespace olympus {

void DiplomaticScorer::bootstrap() {
	bootstrapped_ = true;
	tension_score_ = 0.0;
}

void DiplomaticScorer::ingest(const Signal& signal) {
	if (!bootstrapped_) {
		bootstrap();
	}
	tension_score_ = std::clamp(tension_score_ * 0.7 + signal.urgency * 0.3, 0.0, 1.0);
}

double DiplomaticScorer::score(const DecisionProposal& proposal) const {
	return std::clamp(tension_score_ + (1.0 - proposal.confidence) * 0.2, 0.0, 1.0);
}

std::string DiplomaticScorer::summary() const {
	return "tension_score=" + std::to_string(tension_score_);
}

} // namespace olympus

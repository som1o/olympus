#include "cyber_sentinel.hpp"

#include <algorithm>

namespace olympus {

void CyberSentinel::bootstrap() {
	bootstrapped_ = true;
	threat_ = 0.0;
}

void CyberSentinel::ingest(const Signal& signal) {
	if (!bootstrapped_) {
		bootstrap();
	}
	threat_ = std::clamp(threat_ * 0.7 + signal.urgency * 0.3, 0.0, 1.0);
}

double CyberSentinel::threat_index(const DecisionProposal& proposal) const {
	return std::clamp(threat_ + (1.0 - proposal.confidence) * 0.25, 0.0, 1.0);
}

std::string CyberSentinel::summary() const {
	return "cyber_threat=" + std::to_string(threat_);
}

} // namespace olympus

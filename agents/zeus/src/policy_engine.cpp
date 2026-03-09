#include "policy_engine.hpp"

#include <algorithm>

namespace olympus {

void PolicyEngine::bootstrap() {
	bootstrapped_ = true;
	policy_pressure_ = 0.0;
	last_signal_type_.clear();
}

void PolicyEngine::ingest(const Signal& signal) {
	if (!bootstrapped_) {
		bootstrap();
	}
	policy_pressure_ = std::clamp((policy_pressure_ * 0.75) + (signal.urgency * 0.25), 0.0, 1.0);
	last_signal_type_ = signal.type;
}

std::string PolicyEngine::recommend(const DecisionProposal& proposal) const {
	const double pressure = std::clamp(policy_pressure_ + (1.0 - proposal.confidence) * 0.25, 0.0, 1.0);
	if (pressure >= 0.8) {
		return "escalate to emergency decree review";
	}
	if (pressure >= 0.55) {
		return "proceed with inter-ministerial policy package";
	}
	return "standard governance pathway";
}

std::string PolicyEngine::summary() const {
	return "pressure=" + std::to_string(policy_pressure_) +
				 ",last_signal=" + (last_signal_type_.empty() ? std::string("n/a") : last_signal_type_);
}

} // namespace olympus

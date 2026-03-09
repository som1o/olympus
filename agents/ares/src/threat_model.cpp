#include "threat_model.hpp"

#include <algorithm>

namespace olympus {

void ThreatModel::bootstrap() { bootstrapped_ = true; }

void ThreatModel::ingest(const Signal& signal) {
	if (!bootstrapped_) {
		bootstrap();
	}

	++samples_;
	const double clamped = std::clamp(signal.urgency, 0.0, 1.0);
	rolling_urgency_ = (rolling_urgency_ * static_cast<double>(samples_ - 1) + clamped) /
										 static_cast<double>(samples_);
	if (clamped >= 0.75 || signal.type.find("threat") != std::string::npos ||
			signal.type.find("incursion") != std::string::npos) {
		++critical_events_;
	}
}

double ThreatModel::score(const DecisionProposal& proposal) const {
	const double confidence_penalty = std::clamp(1.0 - proposal.confidence, 0.0, 1.0) * 0.35;
	const double event_boost = samples_ == 0 ? 0.0 : (static_cast<double>(critical_events_) / samples_) * 0.40;
	return std::clamp(rolling_urgency_ + confidence_penalty + event_boost, 0.0, 1.0);
}

std::string ThreatModel::summary() const {
	return "samples=" + std::to_string(samples_) +
				 ",critical=" + std::to_string(critical_events_) +
				 ",rolling=" + std::to_string(rolling_urgency_);
}

} // namespace olympus

#include "intel_aggregator.hpp"

#include <algorithm>

namespace olympus {

void IntelAggregator::bootstrap() {
	bootstrapped_ = true;
	observations_ = 0;
	elevated_alerts_ = 0;
	last_signal_type_.clear();
}

void IntelAggregator::ingest(const Signal& signal) {
	if (!bootstrapped_) {
		bootstrap();
	}
	++observations_;
	last_signal_type_ = signal.type;
	if (std::clamp(signal.urgency, 0.0, 1.0) >= 0.7) {
		++elevated_alerts_;
	}
}

std::string IntelAggregator::intel_brief() const {
	return "obs=" + std::to_string(observations_) +
				 ",elevated=" + std::to_string(elevated_alerts_) +
				 ",last=" + (last_signal_type_.empty() ? std::string("n/a") : last_signal_type_);
}

} // namespace olympus

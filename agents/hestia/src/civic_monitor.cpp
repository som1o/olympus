#include "civic_monitor.hpp"

#include <algorithm>

namespace olympus {

void CivicMonitor::bootstrap() {
	bootstrapped_ = true;
	surges_ = 0;
	rolling_urgency_ = 0.0;
}

void CivicMonitor::ingest(const Signal& signal) {
	if (!bootstrapped_) {
		bootstrap();
	}
	rolling_urgency_ = std::clamp(rolling_urgency_ * 0.7 + signal.urgency * 0.3, 0.0, 1.0);
	if (signal.urgency >= 0.8 || signal.type.find("complaint") != std::string::npos ||
			signal.type.find("outage") != std::string::npos) {
		++surges_;
	}
}

std::size_t CivicMonitor::active_surges() const { return surges_; }

std::string CivicMonitor::summary() const {
	return "surges=" + std::to_string(surges_) +
				 ",rolling_urgency=" + std::to_string(rolling_urgency_);
}

} // namespace olympus

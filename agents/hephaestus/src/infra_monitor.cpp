#include "infra_monitor.hpp"

#include <algorithm>

namespace olympus {

void InfraMonitor::bootstrap() {
	bootstrapped_ = true;
	stability_ = 1.0;
}

void InfraMonitor::ingest(const Signal& signal) {
	if (!bootstrapped_) {
		bootstrap();
	}
	stability_ = std::clamp(stability_ * 0.8 + (1.0 - signal.urgency) * 0.2, 0.0, 1.0);
}

double InfraMonitor::stability_index() const { return stability_; }

std::string InfraMonitor::summary() const {
	return "infra_stability=" + std::to_string(stability_);
}

} // namespace olympus

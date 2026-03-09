#include "resource_monitor.hpp"

#include <algorithm>

namespace olympus {

void ResourceMonitor::bootstrap() {
	bootstrapped_ = true;
	strain_ = 0.0;
}

void ResourceMonitor::ingest(const Signal& signal) {
	if (!bootstrapped_) {
		bootstrap();
	}
	strain_ = std::clamp(strain_ * 0.75 + signal.urgency * 0.25, 0.0, 1.0);
}

double ResourceMonitor::strain_index() const { return strain_; }

std::string ResourceMonitor::summary() const {
	return "strain=" + std::to_string(strain_);
}

} // namespace olympus

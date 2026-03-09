#include "outbreak_detector.hpp"

#include <algorithm>

namespace olympus {

void OutbreakDetector::bootstrap() {
	bootstrapped_ = true;
	alerts_ = 0;
	active_ = false;
}

void OutbreakDetector::ingest(const Signal& signal) {
	if (!bootstrapped_) {
		bootstrap();
	}

	const bool severe = std::clamp(signal.urgency, 0.0, 1.0) >= 0.75 ||
											signal.type.find("outbreak") != std::string::npos ||
											signal.type.find("infection") != std::string::npos;
	if (severe) {
		++alerts_;
	}
	active_ = severe || alerts_ >= 2;
}

bool OutbreakDetector::has_active_outbreak() const { return active_; }

std::string OutbreakDetector::summary() const {
	return "alerts=" + std::to_string(alerts_) + ",active=" + (active_ ? "true" : "false");
}

} // namespace olympus

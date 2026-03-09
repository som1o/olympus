#include "research_tracker.hpp"

#include <algorithm>

namespace olympus {

void ResearchTracker::bootstrap() {
	bootstrapped_ = true;
	momentum_ = 0.0;
	publications_surged_ = 0;
}

void ResearchTracker::ingest(const Signal& signal) {
	if (!bootstrapped_) {
		bootstrap();
	}
	momentum_ = std::clamp(momentum_ * 0.8 + signal.urgency * 0.2, 0.0, 1.0);
	if (signal.type.find("research") != std::string::npos || signal.type.find("patent") != std::string::npos) {
		++publications_surged_;
	}
}

double ResearchTracker::research_momentum() const { return momentum_; }

std::string ResearchTracker::summary() const {
	return "momentum=" + std::to_string(momentum_) +
				 ",surges=" + std::to_string(publications_surged_);
}

} // namespace olympus

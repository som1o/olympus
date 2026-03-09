#include "epidemic_model.hpp"

#include <algorithm>

namespace olympus {

void EpidemicModel::bootstrap() {
	bootstrapped_ = true;
	transmission_index_ = 0.0;
}

void EpidemicModel::ingest(const Signal& signal) {
	if (!bootstrapped_) {
		bootstrap();
	}
	transmission_index_ = std::clamp(transmission_index_ * 0.7 + signal.urgency * 0.3, 0.0, 1.0);
}

double EpidemicModel::outbreak_pressure(const DecisionProposal& proposal) const {
	return std::clamp(transmission_index_ + (1.0 - proposal.confidence) * 0.2, 0.0, 1.0);
}

std::string EpidemicModel::summary() const {
	return "tx_index=" + std::to_string(transmission_index_);
}

} // namespace olympus

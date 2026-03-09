#include "inference.hpp"

#include <algorithm>

namespace olympus {

void BayesianInference::update(const Signal& signal) {
  double& belief = beliefs_[signal.domain];
  const double old = belief;
  const double blended = (belief * 0.7) + (signal.urgency * 0.3);
  belief = std::clamp(blended, 0.0, 1.0);
  last_delta_ = std::abs(belief - old);
}

double BayesianInference::belief_for(const Domain domain) const {
  if (const auto it = beliefs_.find(domain); it != beliefs_.end()) {
    return it->second;
  }
  return 0.0;
}

double BayesianInference::last_delta() const { return last_delta_; }

} // namespace olympus

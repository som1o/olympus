#pragma once

#include "signal.hpp"

#include <unordered_map>

namespace olympus {

class BayesianInference {
public:
  void update(const Signal& signal);
  double belief_for(Domain domain) const;
  double last_delta() const;

private:
  std::unordered_map<Domain, double> beliefs_;
  double last_delta_{0.0};
};

} // namespace olympus

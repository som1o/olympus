#include "monte_carlo.hpp"

#include <algorithm>

namespace olympus {

MonteCarloEngine::MonteCarloEngine(const std::size_t trials) : trials_(trials) {}

MonteCarloResult MonteCarloEngine::run(const DecisionProposal& proposal) const {
  const double base = std::clamp(proposal.confidence, 0.1, 0.99);
  const double spread = std::min(0.35, 2000.0 / static_cast<double>(trials_ + 1) * 0.1);

  MonteCarloResult out{};
  out.p10 = std::clamp(base - spread, 0.0, 1.0);
  out.p50 = base;
  out.p90 = std::clamp(base + spread, 0.0, 1.0);
  out.summary = "Stub Monte Carlo completed using " + std::to_string(trials_) + " trials.";
  return out;
}

} // namespace olympus

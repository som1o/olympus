#pragma once

#include "contracts.hpp"
#include "cross_correlator.hpp"
#include "decision_ledger.hpp"
#include "inference.hpp"
#include "priority_queue.hpp"
#include "risk_scorer.hpp"
#include "signal.hpp"

#include <optional>

namespace olympus {

class Themis {
public:
  explicit Themis(double confidence_floor);

  void ingest(const Signal& signal);
  std::optional<DecisionProposal> propose();
  const DecisionLedger& ledger() const;
  DecisionLedger& ledger_mutable();

private:
  static std::string action_for(Domain domain);

  BayesianInference inference_;
  UrgencyQueue queue_;
  RiskScorer risk_scorer_;
  CrossCorrelator correlator_;
  DecisionLedger ledger_;
  std::size_t proposal_counter_{0};
  std::size_t seq_{0};
  double confidence_floor_;
};

} // namespace olympus

#include "themis.hpp"

#include <algorithm>

namespace olympus {

Themis::Themis(const double confidence_floor) : confidence_floor_(confidence_floor) {}

void Themis::ingest(const Signal& signal) {
  inference_.update(signal);

  const double belief = inference_.belief_for(signal.domain);
  const double delta = inference_.last_delta();
  const double score = std::clamp(signal.urgency * (0.6 + belief) * (1.0 + delta), 0.0, 2.0);

  queue_.push(UrgencyItem{signal, score, seq_++});
}

std::optional<DecisionProposal> Themis::propose() {
  const auto item = queue_.pop();
  if (!item.has_value()) {
    return std::nullopt;
  }

  DecisionProposal proposal{};
  proposal.proposal_id = "proposal-" + std::to_string(proposal_counter_++);
  proposal.domain = item->signal.domain;
  proposal.action_type = action_for(item->signal.domain);
  proposal.action_params = item->signal.payload;
  proposal.confidence = std::clamp(0.45 + (item->score * 0.3), confidence_floor_, 0.99);
  proposal.justification = correlator_.correlate(item->signal);
  proposal.timestamp = item->signal.timestamp;

  const RiskAssessment risk = risk_scorer_.assess(proposal);
  proposal.risk_level = risk.level;
  ledger_.append(proposal, risk);
  return proposal;
}

const DecisionLedger& Themis::ledger() const { return ledger_; }

DecisionLedger& Themis::ledger_mutable() { return ledger_; }

std::string Themis::action_for(const Domain domain) {
  switch (domain) {
  case Domain::Defense:
    return "REQUEST_INTEL";
  case Domain::Governance:
    return "CONVENE_TASK_FORCE";
  case Domain::Education:
    return "FUND_GRANT";
  case Domain::Health:
    return "OUTBREAK_ALERT";
  case Domain::Local:
    return "DISPATCH_REPAIR";
  case Domain::Foreign:
    return "INITIATE_NEGOTIATION";
  case Domain::Resources:
    return "IMPORT_AUTHORIZATION";
  case Domain::Technology:
    return "CYBER_INCIDENT_RESPONSE";
  case Domain::Unknown:
    return "NO_OP";
  }
  return "NO_OP";
}

} // namespace olympus

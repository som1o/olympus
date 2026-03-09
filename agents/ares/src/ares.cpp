#include "ares.hpp"

#include <algorithm>

namespace olympus {

std::string AresAgent::name() const { return "ARES"; }
Domain AresAgent::domain() const { return Domain::Defense; }
void AresAgent::on_signal(const Signal& signal) {
  if (signal.domain != Domain::Defense) {
    status_ = AgentStatus::Error;
    return;
  }

  last_urgency_ = std::clamp(signal.urgency, 0.0, 1.0);
  intel_aggregator_.ingest(signal);
  threat_model_.ingest(signal);
  status_ = AgentStatus::Running;
}

void AresAgent::on_proposal(const DecisionProposal& proposal) {
  if (proposal.domain != Domain::Defense) {
    status_ = AgentStatus::Error;
    return;
  }
  status_ = AgentStatus::Executing;
}

ExecutionReport AresAgent::execute(const DecisionProposal& proposal) {
  if (proposal.domain != Domain::Defense) {
    status_ = AgentStatus::Error;
    return ExecutionReport{proposal.proposal_id, false, "ARES rejected non-defense proposal"};
  }

  const double threat_score = threat_model_.score(proposal);
  const std::string plan = tactical_planner_.plan(proposal, threat_score);
  status_ = AgentStatus::Running;
  return ExecutionReport{proposal.proposal_id,
                         true,
                         "threat_score=" + std::to_string(threat_score) +
                             ";urgency=" + std::to_string(last_urgency_) +
                             ";plan=" + plan +
                             ";intel=" + intel_aggregator_.intel_brief() +
                             ";model=" + threat_model_.summary()};
}
AgentStatus AresAgent::status() const { return status_; }
void AresAgent::tick() { if (status_ == AgentStatus::Running) status_ = AgentStatus::Idle; }

} // namespace olympus

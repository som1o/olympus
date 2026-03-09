#include "hestia.hpp"

#include <algorithm>

namespace olympus {

std::string HestiaAgent::name() const { return "HESTIA"; }
Domain HestiaAgent::domain() const { return Domain::Local; }
void HestiaAgent::on_signal(const Signal& signal) {
  if (signal.domain != Domain::Local) {
    status_ = AgentStatus::Error;
    return;
  }

  last_urgency_ = std::clamp(signal.urgency, 0.0, 1.0);
  civic_monitor_.ingest(signal);
  complaint_surges_ = civic_monitor_.active_surges();
  status_ = AgentStatus::Running;
}

void HestiaAgent::on_proposal(const DecisionProposal& proposal) {
  if (proposal.domain != Domain::Local) {
    status_ = AgentStatus::Error;
    return;
  }
  status_ = AgentStatus::Executing;
}

ExecutionReport HestiaAgent::execute(const DecisionProposal& proposal) {
  if (proposal.domain != Domain::Local) {
    status_ = AgentStatus::Error;
    return ExecutionReport{proposal.proposal_id, false, "HESTIA rejected non-local proposal"};
  }

  const std::string municipal_plan = municipal_planner_.build_plan(proposal, complaint_surges_);
  const std::string service_plan = service_optimizer_.optimize(proposal, last_urgency_);
  status_ = AgentStatus::Running;
  return ExecutionReport{proposal.proposal_id,
                         true,
                         "surges=" + std::to_string(complaint_surges_) +
                             ";urgency=" + std::to_string(last_urgency_) +
                             ";municipal_plan=" + municipal_plan +
                             ";service_plan=" + service_plan +
                             ";civic=" + civic_monitor_.summary()};
}
AgentStatus HestiaAgent::status() const { return status_; }
void HestiaAgent::tick() { if (status_ == AgentStatus::Running) status_ = AgentStatus::Idle; }

} // namespace olympus

#include "hephaestus.hpp"

#include <algorithm>

namespace olympus {

std::string HephaestusAgent::name() const { return "HEPHAESTUS"; }
Domain HephaestusAgent::domain() const { return Domain::Technology; }
void HephaestusAgent::on_signal(const Signal& signal) {
  if (signal.domain != Domain::Technology) {
    status_ = AgentStatus::Error;
    return;
  }
  cyber_threat_ = std::clamp(signal.urgency, 0.0, 1.0);
  cyber_sentinel_.ingest(signal);
  infra_monitor_.ingest(signal);
  status_ = AgentStatus::Running;
}

void HephaestusAgent::on_proposal(const DecisionProposal& proposal) {
  if (proposal.domain != Domain::Technology) {
    status_ = AgentStatus::Error;
    return;
  }
  status_ = AgentStatus::Executing;
}

ExecutionReport HephaestusAgent::execute(const DecisionProposal& proposal) {
  if (proposal.domain != Domain::Technology) {
    status_ = AgentStatus::Error;
    return ExecutionReport{proposal.proposal_id, false, "HEPHAESTUS rejected non-technology proposal"};
  }

  const double threat = cyber_sentinel_.threat_index(proposal);
  const double stability = infra_monitor_.stability_index();
  const std::string step = tech_roadmap_.next_step(proposal, threat, stability);
  status_ = AgentStatus::Running;
  return ExecutionReport{proposal.proposal_id,
                         true,
                         "signal_threat=" + std::to_string(cyber_threat_) +
                             ";modeled_threat=" + std::to_string(threat) +
                             ";stability=" + std::to_string(stability) +
                             ";roadmap=" + step +
                             ";sentinel=" + cyber_sentinel_.summary() +
                             ";infra=" + infra_monitor_.summary()};
}
AgentStatus HephaestusAgent::status() const { return status_; }
void HephaestusAgent::tick() { if (status_ == AgentStatus::Running) status_ = AgentStatus::Idle; }

} // namespace olympus

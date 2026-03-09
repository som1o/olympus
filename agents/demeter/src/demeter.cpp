#include "demeter.hpp"

#include <algorithm>

namespace olympus {

std::string DemeterAgent::name() const { return "DEMETER"; }
Domain DemeterAgent::domain() const { return Domain::Resources; }
void DemeterAgent::on_signal(const Signal& signal) {
  if (signal.domain != Domain::Resources) {
    status_ = AgentStatus::Error;
    return;
  }
  resource_strain_ = std::clamp(signal.urgency, 0.0, 1.0);
  resource_monitor_.ingest(signal);
  status_ = AgentStatus::Running;
}

void DemeterAgent::on_proposal(const DecisionProposal& proposal) {
  if (proposal.domain != Domain::Resources) {
    status_ = AgentStatus::Error;
    return;
  }
  status_ = AgentStatus::Executing;
}

ExecutionReport DemeterAgent::execute(const DecisionProposal& proposal) {
  if (proposal.domain != Domain::Resources) {
    status_ = AgentStatus::Error;
    return ExecutionReport{proposal.proposal_id, false, "DEMETER rejected non-resources proposal"};
  }

  const double strain = resource_monitor_.strain_index();
  const std::string continuity = supply_chain_.continuity_plan(proposal, strain);
  const double env_score = env_impact_scorer_.score(proposal, strain);
  status_ = AgentStatus::Running;
  return ExecutionReport{proposal.proposal_id,
                         true,
                         "resource_strain=" + std::to_string(resource_strain_) +
                             ";modeled_strain=" + std::to_string(strain) +
                             ";continuity=" + continuity +
                             ";env_score=" + std::to_string(env_score) +
                             ";monitor=" + resource_monitor_.summary() +
                             ";impact=" + env_impact_scorer_.summary()};
}
AgentStatus DemeterAgent::status() const { return status_; }
void DemeterAgent::tick() { if (status_ == AgentStatus::Running) status_ = AgentStatus::Idle; }

} // namespace olympus

#include "asclepius.hpp"

#include <algorithm>

namespace olympus {

std::string AsclepiusAgent::name() const { return "ASCLEPIUS"; }
Domain AsclepiusAgent::domain() const { return Domain::Health; }
void AsclepiusAgent::on_signal(const Signal& signal) {
  if (signal.domain != Domain::Health) {
    status_ = AgentStatus::Error;
    return;
  }
  icu_pressure_ = std::clamp(signal.urgency, 0.0, 1.0);
  epidemic_model_.ingest(signal);
  outbreak_detector_.ingest(signal);
  status_ = AgentStatus::Running;
}

void AsclepiusAgent::on_proposal(const DecisionProposal& proposal) {
  if (proposal.domain != Domain::Health) {
    status_ = AgentStatus::Error;
    return;
  }
  status_ = AgentStatus::Executing;
}

ExecutionReport AsclepiusAgent::execute(const DecisionProposal& proposal) {
  if (proposal.domain != Domain::Health) {
    status_ = AgentStatus::Error;
    return ExecutionReport{proposal.proposal_id, false, "ASCLEPIUS rejected non-health proposal"};
  }

  const bool outbreak = outbreak_detector_.has_active_outbreak();
  const double pressure = epidemic_model_.outbreak_pressure(proposal);
  const std::string plan = resource_allocator_.allocation_plan(proposal, icu_pressure_, outbreak);
  status_ = AgentStatus::Running;
  return ExecutionReport{proposal.proposal_id,
                         true,
                         "icu=" + std::to_string(icu_pressure_) +
                             ";outbreak_pressure=" + std::to_string(pressure) +
                             ";plan=" + plan +
                             ";model=" + epidemic_model_.summary() +
                             ";detector=" + outbreak_detector_.summary()};
}
AgentStatus AsclepiusAgent::status() const { return status_; }
void AsclepiusAgent::tick() { if (status_ == AgentStatus::Running) status_ = AgentStatus::Idle; }

} // namespace olympus

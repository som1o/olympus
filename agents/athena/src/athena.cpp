#include "athena.hpp"

#include <algorithm>

namespace olympus {

std::string AthenaAgent::name() const { return "ATHENA"; }
Domain AthenaAgent::domain() const { return Domain::Education; }
void AthenaAgent::on_signal(const Signal& signal) {
  if (signal.domain != Domain::Education) {
    status_ = AgentStatus::Error;
    return;
  }
  enrollment_pressure_ = std::clamp(signal.urgency, 0.0, 1.0);
  research_tracker_.ingest(signal);
  status_ = AgentStatus::Running;
}

void AthenaAgent::on_proposal(const DecisionProposal& proposal) {
  if (proposal.domain != Domain::Education) {
    status_ = AgentStatus::Error;
    return;
  }
  status_ = AgentStatus::Executing;
}

ExecutionReport AthenaAgent::execute(const DecisionProposal& proposal) {
  if (proposal.domain != Domain::Education) {
    status_ = AgentStatus::Error;
    return ExecutionReport{proposal.proposal_id, false, "ATHENA rejected non-education proposal"};
  }

  const double momentum = research_tracker_.research_momentum();
  const std::string curriculum = curriculum_optimizer_.optimize(proposal, enrollment_pressure_);
  const std::string talent = talent_pipeline_.forecast(proposal, momentum);
  status_ = AgentStatus::Running;
  return ExecutionReport{proposal.proposal_id,
                         true,
                         "pressure=" + std::to_string(enrollment_pressure_) +
                             ";curriculum=" + curriculum +
                             ";talent=" + talent +
                             ";research=" + research_tracker_.summary()};
}
AgentStatus AthenaAgent::status() const { return status_; }
void AthenaAgent::tick() { if (status_ == AgentStatus::Running) status_ = AgentStatus::Idle; }

} // namespace olympus

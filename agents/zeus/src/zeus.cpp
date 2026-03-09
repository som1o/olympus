#include "zeus.hpp"

namespace olympus {

std::string ZeusAgent::name() const { return "ZEUS"; }
Domain ZeusAgent::domain() const { return Domain::Governance; }
void ZeusAgent::on_signal(const Signal& signal) {
  if (signal.domain != Domain::Governance) {
    status_ = AgentStatus::Error;
    return;
  }
  policy_engine_.ingest(signal);
  status_ = AgentStatus::Running;
}

void ZeusAgent::on_proposal(const DecisionProposal& proposal) {
  if (proposal.domain != Domain::Governance) {
    status_ = AgentStatus::Error;
    return;
  }
  ++proposal_count_;
  legislative_tracker_.register_proposal(proposal);
  status_ = AgentStatus::Executing;
}

ExecutionReport ZeusAgent::execute(const DecisionProposal& proposal) {
  if (proposal.domain != Domain::Governance) {
    status_ = AgentStatus::Error;
    return ExecutionReport{proposal.proposal_id, false, "ZEUS rejected non-governance proposal"};
  }

  const std::string recommendation = policy_engine_.recommend(proposal);
  const double budget = budget_allocator_.allocation_ratio(proposal);
  status_ = AgentStatus::Running;
  return ExecutionReport{proposal.proposal_id,
                         true,
                         "sequence=" + std::to_string(proposal_count_) +
                             ";recommendation=" + recommendation +
                             ";budget_ratio=" + std::to_string(budget) +
                             ";policy=" + policy_engine_.summary() +
                             ";legislation=" + legislative_tracker_.summary() +
                             ";allocator=" + budget_allocator_.summary()};
}
AgentStatus ZeusAgent::status() const { return status_; }
void ZeusAgent::tick() { if (status_ == AgentStatus::Running) status_ = AgentStatus::Idle; }

} // namespace olympus

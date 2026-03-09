#include "hermes.hpp"

#include <algorithm>

namespace olympus {

std::string HermesAgent::name() const { return "HERMES"; }
Domain HermesAgent::domain() const { return Domain::Foreign; }
void HermesAgent::on_signal(const Signal& signal) {
  if (signal.domain != Domain::Foreign) {
    status_ = AgentStatus::Error;
    return;
  }
  diplomatic_tension_ = std::clamp(signal.urgency, 0.0, 1.0);
  diplomatic_scorer_.ingest(signal);
  status_ = AgentStatus::Running;
}

void HermesAgent::on_proposal(const DecisionProposal& proposal) {
  if (proposal.domain != Domain::Foreign) {
    status_ = AgentStatus::Error;
    return;
  }
  treaty_tracker_.register_proposal(proposal);
  status_ = AgentStatus::Executing;
}

ExecutionReport HermesAgent::execute(const DecisionProposal& proposal) {
  if (proposal.domain != Domain::Foreign) {
    status_ = AgentStatus::Error;
    return ExecutionReport{proposal.proposal_id, false, "HERMES rejected non-foreign proposal"};
  }

  const double score = diplomatic_scorer_.score(proposal);
  const std::string trade_impact = trade_analyzer_.impact_brief(proposal, score);
  status_ = AgentStatus::Running;
  return ExecutionReport{proposal.proposal_id,
                         true,
                         "tension=" + std::to_string(diplomatic_tension_) +
                             ";diplo_score=" + std::to_string(score) +
                             ";trade=" + trade_impact +
                             ";scorer=" + diplomatic_scorer_.summary() +
                             ";treaties=" + treaty_tracker_.summary()};
}
AgentStatus HermesAgent::status() const { return status_; }
void HermesAgent::tick() { if (status_ == AgentStatus::Running) status_ = AgentStatus::Idle; }

} // namespace olympus

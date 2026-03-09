#pragma once

#include "agent_base.hpp"
#include "intel_aggregator.hpp"
#include "tactical_planner.hpp"
#include "threat_model.hpp"

namespace olympus {

class AresAgent final : public AgentBase {
public:
  std::string name() const override;
  Domain domain() const override;
  void on_signal(const Signal& signal) override;
  void on_proposal(const DecisionProposal& proposal) override;
  ExecutionReport execute(const DecisionProposal& proposal) override;
  AgentStatus status() const override;
  void tick() override;

private:
  AgentStatus status_{AgentStatus::Idle};
  double last_urgency_{0.0};
  ThreatModel threat_model_;
  TacticalPlanner tactical_planner_;
  IntelAggregator intel_aggregator_;
};

} // namespace olympus

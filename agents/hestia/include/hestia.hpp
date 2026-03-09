#pragma once

#include "agent_base.hpp"
#include "civic_monitor.hpp"
#include "municipal_planner.hpp"
#include "service_optimizer.hpp"

namespace olympus {

class HestiaAgent final : public AgentBase {
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
  std::size_t complaint_surges_{0};
  double last_urgency_{0.0};
  CivicMonitor civic_monitor_;
  MunicipalPlanner municipal_planner_;
  ServiceOptimizer service_optimizer_;
};

} // namespace olympus

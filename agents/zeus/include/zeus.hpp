#pragma once

#include "agent_base.hpp"
#include "budget_allocator.hpp"
#include "legislative_tracker.hpp"
#include "policy_engine.hpp"

namespace olympus {

class ZeusAgent final : public AgentBase {
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
  std::size_t proposal_count_{0};
  PolicyEngine policy_engine_;
  BudgetAllocator budget_allocator_;
  LegislativeTracker legislative_tracker_;
};

} // namespace olympus

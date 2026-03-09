#pragma once

#include "agent_base.hpp"
#include "env_impact_scorer.hpp"
#include "resource_monitor.hpp"
#include "supply_chain.hpp"

namespace olympus {

class DemeterAgent final : public AgentBase {
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
  double resource_strain_{0.0};
  ResourceMonitor resource_monitor_;
  SupplyChain supply_chain_;
  EnvImpactScorer env_impact_scorer_;
};

} // namespace olympus

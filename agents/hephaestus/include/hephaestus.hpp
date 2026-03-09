#pragma once

#include "agent_base.hpp"
#include "cyber_sentinel.hpp"
#include "infra_monitor.hpp"
#include "tech_roadmap.hpp"

namespace olympus {

class HephaestusAgent final : public AgentBase {
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
  double cyber_threat_{0.0};
  CyberSentinel cyber_sentinel_;
  InfraMonitor infra_monitor_;
  TechRoadmap tech_roadmap_;
};

} // namespace olympus

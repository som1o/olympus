#pragma once

#include "agent_base.hpp"
#include "curriculum_optimizer.hpp"
#include "research_tracker.hpp"
#include "talent_pipeline.hpp"

namespace olympus {

class AthenaAgent final : public AgentBase {
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
  double enrollment_pressure_{0.0};
  CurriculumOptimizer curriculum_optimizer_;
  ResearchTracker research_tracker_;
  TalentPipeline talent_pipeline_;
};

} // namespace olympus

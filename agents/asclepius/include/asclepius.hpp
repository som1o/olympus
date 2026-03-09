#pragma once

#include "agent_base.hpp"
#include "epidemic_model.hpp"
#include "outbreak_detector.hpp"
#include "resource_allocator.hpp"

namespace olympus {

class AsclepiusAgent final : public AgentBase {
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
  double icu_pressure_{0.0};
  EpidemicModel epidemic_model_;
  OutbreakDetector outbreak_detector_;
  ResourceAllocator resource_allocator_;
};

} // namespace olympus

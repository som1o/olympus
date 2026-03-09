#pragma once

#include "agent_base.hpp"
#include "diplomatic_scorer.hpp"
#include "trade_analyzer.hpp"
#include "treaty_tracker.hpp"

namespace olympus {

class HermesAgent final : public AgentBase {
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
  double diplomatic_tension_{0.0};
  DiplomaticScorer diplomatic_scorer_;
  TreatyTracker treaty_tracker_;
  TradeAnalyzer trade_analyzer_;
};

} // namespace olympus

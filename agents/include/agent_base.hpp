#pragma once

#include "contracts.hpp"
#include "signal.hpp"

#include <string>

namespace olympus {

enum class AgentStatus { Idle, Running, Executing, Error };

struct ExecutionReport {
  std::string proposal_id;
  bool success{false};
  std::string summary;
};

class AgentBase {
public:
  virtual ~AgentBase() = default;

  virtual std::string name() const = 0;
  virtual Domain domain() const = 0;
  virtual void on_signal(const Signal& signal) = 0;
  virtual void on_proposal(const DecisionProposal& proposal) = 0;
  virtual ExecutionReport execute(const DecisionProposal& proposal) = 0;
  virtual AgentStatus status() const = 0;
  virtual void tick() = 0;
};

} // namespace olympus

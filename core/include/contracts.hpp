#pragma once

#include "signal.hpp"

#include <chrono>
#include <string>
#include <vector>

namespace olympus {

enum class RiskLevel { Low, Medium, High, Critical };

enum class AuthorizationStatus { Pending, Authorized, Vetoed, Executed, Failed };

struct DecisionProposal {
  std::string proposal_id;
  Domain domain{Domain::Unknown};
  std::string action_type;
  std::string action_params;
  double confidence{0.0};
  RiskLevel risk_level{RiskLevel::Low};
  std::string justification;
  std::chrono::system_clock::time_point timestamp;
};

struct RiskAssessment {
  RiskLevel level{RiskLevel::Low};
  double cascade_probability{0.0};
  std::vector<Domain> domains_at_risk;
  int recovery_days{0};
};

struct LedgerEntry {
  std::string proposal_id;
  Domain domain{Domain::Unknown};
  std::string action;
  double confidence{0.0};
  RiskLevel risk_level{RiskLevel::Low};
  AuthorizationStatus status{AuthorizationStatus::Pending};
  std::string outcome;
  std::string entry_hash;
  std::chrono::system_clock::time_point timestamp;
};

inline std::string to_string(const RiskLevel level) {
  switch (level) {
  case RiskLevel::Low:
    return "LOW";
  case RiskLevel::Medium:
    return "MEDIUM";
  case RiskLevel::High:
    return "HIGH";
  case RiskLevel::Critical:
    return "CRITICAL";
  }
  return "LOW";
}

} // namespace olympus

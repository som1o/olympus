#pragma once

#include "decision_ledger.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace olympus {

struct AuditEvent {
  std::string actor;
  std::string action;
  std::string detail;
  std::string timestamp;
};

class AuditStream {
public:
  void record(const std::string& actor, const std::string& action, const std::string& detail,
              DecisionLedger* ledger);
  std::string as_text(std::size_t max_items) const;

private:
  std::vector<AuditEvent> events_;
  std::size_t max_items_{512};
};

} // namespace olympus

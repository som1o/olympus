#pragma once

#include "contracts.hpp"

#include <vector>

namespace olympus {

class DecisionLedger {
public:
  void append(const DecisionProposal& proposal, const RiskAssessment& risk);
  void append_authorization(const DecisionProposal& proposal, AuthorizationStatus status,
                            const std::string& outcome);
  void append_audit(const std::string& actor, const std::string& action,
                    const std::string& detail);
  const std::vector<LedgerEntry>& entries() const;

private:
  void append_entry(LedgerEntry entry);

  std::vector<LedgerEntry> entries_;
  std::size_t audit_seq_{0};
  static std::string hash_entry(const LedgerEntry& entry, const std::string& previous_hash);
};

} // namespace olympus

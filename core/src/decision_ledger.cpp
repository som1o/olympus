#include "decision_ledger.hpp"

#include <chrono>
#include <functional>
#include <sstream>

namespace olympus {

void DecisionLedger::append(const DecisionProposal& proposal, const RiskAssessment& risk) {
  LedgerEntry entry{};
  entry.proposal_id = proposal.proposal_id;
  entry.domain = proposal.domain;
  entry.action = proposal.action_type;
  entry.confidence = proposal.confidence;
  entry.risk_level = risk.level;
  entry.status = AuthorizationStatus::Pending;
  entry.outcome = "proposal-generated";
  entry.timestamp = proposal.timestamp;
  append_entry(std::move(entry));
}

void DecisionLedger::append_authorization(const DecisionProposal& proposal,
                                          const AuthorizationStatus status,
                                          const std::string& outcome) {
  LedgerEntry entry{};
  entry.proposal_id = proposal.proposal_id;
  entry.domain = proposal.domain;
  entry.action = proposal.action_type;
  entry.confidence = proposal.confidence;
  entry.risk_level = proposal.risk_level;
  entry.status = status;
  entry.outcome = outcome;
  entry.timestamp = std::chrono::system_clock::now();
  append_entry(std::move(entry));
}

void DecisionLedger::append_audit(const std::string& actor,
                                  const std::string& action,
                                  const std::string& detail) {
  LedgerEntry entry{};
  entry.proposal_id = "audit-" + std::to_string(audit_seq_++);
  entry.domain = Domain::Unknown;
  entry.action = "AUDIT/" + action;
  entry.confidence = 1.0;
  entry.risk_level = RiskLevel::Low;
  entry.status = AuthorizationStatus::Executed;
  entry.outcome = actor + ": " + detail;
  entry.timestamp = std::chrono::system_clock::now();
  append_entry(std::move(entry));
}

void DecisionLedger::append_entry(LedgerEntry entry) {
  const std::string prev = entries_.empty() ? "genesis" : entries_.back().entry_hash;
  entry.entry_hash = hash_entry(entry, prev);
  entries_.push_back(std::move(entry));
}

const std::vector<LedgerEntry>& DecisionLedger::entries() const { return entries_; }

std::string DecisionLedger::hash_entry(const LedgerEntry& entry, const std::string& previous_hash) {
  std::ostringstream raw;
  raw << previous_hash << '|' << entry.proposal_id << '|' << to_string(entry.risk_level) << '|'
      << entry.action << '|' << entry.confidence << '|' << entry.outcome;
  const std::size_t hashed = std::hash<std::string>{}(raw.str());
  return std::to_string(hashed);
}

} // namespace olympus

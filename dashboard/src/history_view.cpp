#include "history_view.hpp"

#include <array>
#include <sstream>

namespace olympus {

std::string HistoryView::summarize(const DecisionLedger& ledger) const {
  const auto& entries = ledger.entries();
  std::array<std::size_t, 4> risk_counts{0, 0, 0, 0};
  std::array<std::size_t, 5> status_counts{0, 0, 0, 0, 0};

  for (const auto& entry : entries) {
    switch (entry.risk_level) {
    case RiskLevel::Low:
      ++risk_counts[0];
      break;
    case RiskLevel::Medium:
      ++risk_counts[1];
      break;
    case RiskLevel::High:
      ++risk_counts[2];
      break;
    case RiskLevel::Critical:
      ++risk_counts[3];
      break;
    }

    switch (entry.status) {
    case AuthorizationStatus::Pending:
      ++status_counts[0];
      break;
    case AuthorizationStatus::Authorized:
      ++status_counts[1];
      break;
    case AuthorizationStatus::Vetoed:
      ++status_counts[2];
      break;
    case AuthorizationStatus::Executed:
      ++status_counts[3];
      break;
    case AuthorizationStatus::Failed:
      ++status_counts[4];
      break;
    }
  }

  std::ostringstream out;
  out << "Ledger entries: " << entries.size() << "\n";
  out << "Risk profile: LOW=" << risk_counts[0] << " MEDIUM=" << risk_counts[1]
      << " HIGH=" << risk_counts[2] << " CRITICAL=" << risk_counts[3] << "\n";
  out << "Authorization states: PENDING=" << status_counts[0]
      << " AUTHORIZED=" << status_counts[1]
      << " VETOED=" << status_counts[2]
      << " EXECUTED=" << status_counts[3]
      << " FAILED=" << status_counts[4];

  if (!entries.empty()) {
    const auto& last = entries.back();
    out << "\n\nLast entry\n";
    out << "proposal=" << last.proposal_id << "\n";
    out << "domain=" << to_string(last.domain) << "\n";
    out << "action=" << last.action << "\n";
    out << "confidence=" << last.confidence << "\n";
    out << "risk=" << to_string(last.risk_level) << "\n";
    out << "hash=" << last.entry_hash;
  }
  return out.str();
}

} // namespace olympus

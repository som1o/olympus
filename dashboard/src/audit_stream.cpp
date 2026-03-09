#include "audit_stream.hpp"

#include "logger.hpp"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace olympus {

namespace {
std::string now_iso8601() {
  const auto now = std::chrono::system_clock::now();
  const std::time_t tt = std::chrono::system_clock::to_time_t(now);
  std::tm tm{};
  localtime_r(&tt, &tm);

  std::ostringstream out;
  out << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
  return out.str();
}
} // namespace

void AuditStream::record(const std::string& actor, const std::string& action,
                         const std::string& detail, DecisionLedger* ledger) {
  AuditEvent event{actor, action, detail, now_iso8601()};
  events_.push_back(event);
  if (events_.size() > max_items_) {
    events_.erase(events_.begin(), events_.begin() +
                                     static_cast<std::ptrdiff_t>(events_.size() - max_items_));
  }

  std::filesystem::create_directories("logs");
  std::ofstream file("logs/dashboard_audit.log", std::ios::app);
  if (file) {
    file << event.timestamp << " actor=" << event.actor << " action=" << event.action
         << " detail=\"" << event.detail << "\"\n";
  }

  Logger::log(LogCategory::Dashboard,
              "AUDIT actor=" + actor + " action=" + action + " detail=" + detail);

  if (ledger != nullptr) {
    ledger->append_audit(actor, action, detail);
  }
}

std::string AuditStream::as_text(const std::size_t max_items) const {
  if (events_.empty()) {
    return "No audit events yet";
  }

  const std::size_t count = std::min(max_items, events_.size());
  const std::size_t start = events_.size() - count;

  std::ostringstream out;
  for (std::size_t i = start; i < events_.size(); ++i) {
    const auto& e = events_[i];
    out << e.timestamp << " | " << e.actor << " | " << e.action << " | " << e.detail;
    if (i + 1 < events_.size()) {
      out << '\n';
    }
  }
  return out.str();
}

} // namespace olympus

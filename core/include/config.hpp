#pragma once

#include <cstdint>
#include <string>

namespace olympus {

struct SystemConfig {
  std::string name{"OLYMPUS"};
  std::string mode{"production"};
  bool log_to_console{true};
  std::uint64_t signal_seed{42};
  std::size_t max_signals_per_tick{64};
  double proposal_confidence_floor{0.55};
  double high_urgency_threshold{0.75};
  std::size_t monte_carlo_trials{1000};
  int timeline_days{90};
  bool auto_authorize_noncritical{true};
  int refresh_ms{250};
};

class ConfigLoader {
public:
  static SystemConfig load_from_file(const std::string& path);
};

} // namespace olympus

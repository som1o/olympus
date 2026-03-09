#pragma once

#include <chrono>
#include <cstdint>
#include <string>

namespace olympus {

enum class Domain {
  Defense,
  Governance,
  Education,
  Health,
  Local,
  Foreign,
  Resources,
  Technology,
  Unknown
};

struct Signal {
  std::string signal_id;
  Domain domain{Domain::Unknown};
  std::string type;
  std::chrono::system_clock::time_point timestamp;
  double urgency{0.0};
  std::string payload;
  std::string source_db;
  std::string source_table;
};

inline std::string to_string(const Domain domain) {
  switch (domain) {
  case Domain::Defense:
    return "defense";
  case Domain::Governance:
    return "governance";
  case Domain::Education:
    return "education";
  case Domain::Health:
    return "health";
  case Domain::Local:
    return "local";
  case Domain::Foreign:
    return "foreign";
  case Domain::Resources:
    return "resources";
  case Domain::Technology:
    return "technology";
  case Domain::Unknown:
    return "unknown";
  }
  return "unknown";
}

} // namespace olympus

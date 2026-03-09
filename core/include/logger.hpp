#pragma once

#include <string>

namespace olympus {

enum class LogCategory {
  Pipeline,
  Themis,
  Prometheus,
  Dashboard,
  System
};

class Logger {
public:
  static void log(LogCategory category, const std::string& message);
};

} // namespace olympus

#include "logger.hpp"

#include <iostream>

namespace olympus {

namespace {
std::string tag(const LogCategory category) {
  switch (category) {
  case LogCategory::Pipeline:
    return "[PIPELINE]";
  case LogCategory::Themis:
    return "[THEMIS]";
  case LogCategory::Prometheus:
    return "[PROMETHEUS]";
  case LogCategory::Dashboard:
    return "[DASHBOARD]";
  case LogCategory::System:
    return "[SYSTEM]";
  }
  return "[SYSTEM]";
}
} // namespace

void Logger::log(const LogCategory category, const std::string& message) {
  std::cout << tag(category) << " " << message << '\n';
}

} // namespace olympus

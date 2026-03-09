#pragma once

#include "signal.hpp"

#include <cstddef>
#include <string>

namespace olympus {

class CivicMonitor {
public:
  void bootstrap();
  void ingest(const Signal& signal);
  std::size_t active_surges() const;
  std::string summary() const;

private:
  bool bootstrapped_{false};
  std::size_t surges_{0};
  double rolling_urgency_{0.0};
};

} // namespace olympus

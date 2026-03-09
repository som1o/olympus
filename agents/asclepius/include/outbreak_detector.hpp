#pragma once

#include "signal.hpp"

#include <cstddef>
#include <string>

namespace olympus {

class OutbreakDetector {
public:
  void bootstrap();
  void ingest(const Signal& signal);
  bool has_active_outbreak() const;
  std::string summary() const;

private:
  bool bootstrapped_{false};
  std::size_t alerts_{0};
  bool active_{false};
};

} // namespace olympus

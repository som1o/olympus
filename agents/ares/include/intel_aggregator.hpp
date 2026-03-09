#pragma once

#include "signal.hpp"

#include <cstddef>
#include <string>

namespace olympus {

class IntelAggregator {
public:
  void bootstrap();
  void ingest(const Signal& signal);
  std::string intel_brief() const;

private:
  bool bootstrapped_{false};
  std::size_t observations_{0};
  std::size_t elevated_alerts_{0};
  std::string last_signal_type_;
};

} // namespace olympus

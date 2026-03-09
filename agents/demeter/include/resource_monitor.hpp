#pragma once

#include "signal.hpp"

#include <string>

namespace olympus {

class ResourceMonitor {
public:
  void bootstrap();
  void ingest(const Signal& signal);
  double strain_index() const;
  std::string summary() const;

private:
  bool bootstrapped_{false};
  double strain_{0.0};
};

} // namespace olympus

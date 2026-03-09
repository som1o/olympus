#pragma once

#include "signal.hpp"

#include <string>

namespace olympus {

class InfraMonitor {
public:
  void bootstrap();
  void ingest(const Signal& signal);
  double stability_index() const;
  std::string summary() const;

private:
  bool bootstrapped_{false};
  double stability_{1.0};
};

} // namespace olympus

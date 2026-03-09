#pragma once

#include "signal.hpp"

#include <string>

namespace olympus {

class ResearchTracker {
public:
  void bootstrap();
  void ingest(const Signal& signal);
  double research_momentum() const;
  std::string summary() const;

private:
  bool bootstrapped_{false};
  double momentum_{0.0};
  std::size_t publications_surged_{0};
};

} // namespace olympus

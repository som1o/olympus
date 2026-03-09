#pragma once

#include "prometheus.hpp"

#include <string>

namespace olympus {

class SimRenderer {
public:
  std::string render_text(const SimulationReport& report) const;
};

} // namespace olympus

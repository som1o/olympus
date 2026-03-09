#pragma once

#include "renderer.hpp"

namespace olympus {

class SimViewer {
public:
  std::string render(const SimulationReport& report) const;

private:
  SimRenderer renderer_;
};

} // namespace olympus

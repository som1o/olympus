#include "renderer.hpp"

#include <sstream>

namespace olympus {

std::string SimRenderer::render_text(const SimulationReport& report) const {
  std::ostringstream out;
  out << "Simulation\n";
  out << "- Monte Carlo P10/P50/P90: " << report.monte_carlo.p10 << "/" << report.monte_carlo.p50
      << "/" << report.monte_carlo.p90 << "\n";
  out << "- Cascades: " << report.cascades.size() << "\n";
  out << "- Timeline Events: " << report.timeline.size() << "\n";
  out << "- Overall Confidence: " << report.overall_confidence;
  return out.str();
}

} // namespace olympus

#include "sim_viewer.hpp"

#include <sstream>

namespace olympus {

std::string SimViewer::render(const SimulationReport& report) const {
  std::ostringstream out;
  out << "PROMETHEUS Outcome Summary\n";
  out << "==========================\n";
  out << renderer_.render_text(report) << "\n\n";
  out << "Topline\n";
  out << "- Cascade events: " << report.cascades.size() << "\n";
  out << "- Population effects: " << report.population.size() << "\n";
  out << "- Timeline horizon events: " << report.timeline.size() << "\n";
  out << "- Confidence: " << report.overall_confidence;
  return out.str();
}

} // namespace olympus

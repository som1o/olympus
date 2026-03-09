#include "config.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace olympus {

namespace {
std::string trim(std::string value) {
  auto not_space = [](unsigned char c) { return !std::isspace(c); };
  value.erase(value.begin(), std::find_if(value.begin(), value.end(), not_space));
  value.erase(std::find_if(value.rbegin(), value.rend(), not_space).base(), value.end());
  return value;
}

std::string unquote(std::string value) {
  if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
    value.erase(value.begin());
    value.pop_back();
  }
  return value;
}
} // namespace

SystemConfig ConfigLoader::load_from_file(const std::string& path) {
  std::ifstream in(path);
  if (!in) {
    throw std::runtime_error("cannot open config: " + path);
  }

  SystemConfig cfg{};
  std::string section;
  std::string line;

  while (std::getline(in, line)) {
    line = trim(line);
    if (line.empty() || line.front() == '#') {
      continue;
    }
    if (line.front() == '[' && line.back() == ']') {
      section = line.substr(1, line.size() - 2);
      continue;
    }

    const auto sep = line.find('=');
    if (sep == std::string::npos) {
      continue;
    }

    const std::string key = trim(line.substr(0, sep));
    const std::string raw = trim(line.substr(sep + 1));
    const std::string value = unquote(raw);

    if (section == "system") {
      if (key == "name") cfg.name = value;
      if (key == "mode") cfg.mode = value;
      if (key == "log_to_console") cfg.log_to_console = (value == "true");
    }
    if (section == "pipeline") {
      if (key == "signal_seed") cfg.signal_seed = std::stoull(value);
      if (key == "max_signals_per_tick") cfg.max_signals_per_tick = std::stoull(value);
    }
    if (section == "themis") {
      if (key == "proposal_confidence_floor") cfg.proposal_confidence_floor = std::stod(value);
      if (key == "high_urgency_threshold") cfg.high_urgency_threshold = std::stod(value);
    }
    if (section == "prometheus") {
      if (key == "monte_carlo_trials") cfg.monte_carlo_trials = std::stoull(value);
      if (key == "timeline_days") cfg.timeline_days = std::stoi(value);
    }
    if (section == "dashboard") {
      if (key == "auto_authorize_noncritical") cfg.auto_authorize_noncritical = (value == "true");
      if (key == "refresh_ms") cfg.refresh_ms = std::stoi(value);
    }
  }

  return cfg;
}

} // namespace olympus

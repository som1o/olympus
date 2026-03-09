#include "stream_processor.hpp"

#include <array>
#include <chrono>
#include <string_view>

namespace olympus {

namespace {
std::uint64_t next_value(std::uint64_t& state) {
  // LCG is enough for deterministic synthetic signals in stub mode.
  state = (state * 6364136223846793005ULL + 1ULL);
  return state;
}

double extract_urgency(const std::string& payload) {
  constexpr std::string_view key = "urgency=";
  const auto pos = payload.find(key);
  if (pos == std::string::npos) {
    return 0.5;
  }

  const auto begin = pos + key.size();
  const auto end = payload.find_first_of(",;} \t", begin);
  const std::string value = payload.substr(begin, end - begin);

  try {
    const double parsed = std::stod(value);
    if (parsed < 0.0) {
      return 0.0;
    }
    if (parsed > 1.0) {
      return 1.0;
    }
    return parsed;
  } catch (...) {
    return 0.5;
  }
}
} // namespace

StreamProcessor::StreamProcessor(const std::uint64_t seed, DBConnector* connector)
    : state_(seed == 0 ? 1 : seed), connector_(connector) {}

bool StreamProcessor::start_cdc(const std::vector<Domain>& domains, const std::string& channel) {
  if (connector_ == nullptr) {
    return false;
  }

  bool all_ok = true;
  cdc_channel_ = channel;
  for (const auto domain : domains) {
    all_ok = connector_->listen(domain, cdc_channel_) && all_ok;
  }
  cdc_enabled_ = all_ok;
  return all_ok;
}

std::vector<Signal> StreamProcessor::poll() {
  if (connector_ != nullptr && cdc_enabled_) {
    auto events = connector_->poll_cdc(std::chrono::milliseconds(25));
    std::vector<Signal> out;
    out.reserve(events.size());
    for (const auto& event : events) {
      out.push_back(map_cdc(event));
    }
    if (!out.empty()) {
      return out;
    }
  }

  static constexpr std::array<Domain, 8> domains{
      Domain::Defense, Domain::Governance, Domain::Education, Domain::Health,
      Domain::Local, Domain::Foreign, Domain::Resources, Domain::Technology};

  std::vector<Signal> out;
  out.reserve(domains.size());

  const auto now = std::chrono::system_clock::now();
  for (std::size_t i = 0; i < domains.size(); ++i) {
    const auto value = next_value(state_);
    const double urgency = static_cast<double>(value % 100) / 100.0;
    Signal signal{};
    signal.signal_id = "sig-" + std::to_string(tick_) + "-" + std::to_string(i);
    signal.domain = domains[i];
    signal.type = urgency > 0.75 ? "critical_event" : "status_update";
    signal.timestamp = now;
    signal.urgency = urgency;
    signal.payload = "{\"value\":" + std::to_string(value % 1000) + "}";
    signal.source_db = to_string(domains[i]);
    signal.source_table = "events";
    out.push_back(std::move(signal));
  }

  ++tick_;
  return out;
}

Signal StreamProcessor::map_cdc(const CDCEvent& event) const {
  Signal signal{};
  signal.signal_id = "cdc-" + std::to_string(
                               static_cast<std::uint64_t>(event.received_at.time_since_epoch().count()));
  signal.domain = event.domain;
  signal.type = "cdc_event";
  signal.timestamp = event.received_at;
  signal.urgency = extract_urgency(event.payload);
  signal.payload = event.payload;
  signal.source_db = to_string(event.domain);
  signal.source_table = event.channel;
  return signal;
}

} // namespace olympus

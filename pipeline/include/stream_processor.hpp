#pragma once

#include "db_connector.hpp"
#include "signal.hpp"

#include <cstdint>
#include <vector>

namespace olympus {

class StreamProcessor {
public:
  explicit StreamProcessor(std::uint64_t seed, DBConnector* connector = nullptr);

  bool start_cdc(const std::vector<Domain>& domains, const std::string& channel);
  std::vector<Signal> poll();

private:
  Signal map_cdc(const CDCEvent& event) const;

  std::uint64_t tick_{0};
  std::uint64_t state_;
  DBConnector* connector_{nullptr};
  std::string cdc_channel_{"olympus_cdc"};
  bool cdc_enabled_{false};
};

} // namespace olympus

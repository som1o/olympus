#pragma once

#include "cascade_model.hpp"

#include <string>
#include <vector>

namespace olympus {

struct TimelineEvent {
  int day{0};
  Domain domain{Domain::Unknown};
  double probability{0.0};
  std::string event_description;
};

class TimelineProjector {
public:
  std::vector<TimelineEvent> build(const std::vector<CascadeEvent>& cascades, int horizon_days) const;
};

} // namespace olympus

#include "timeline.hpp"

#include <algorithm>

namespace olympus {

std::vector<TimelineEvent> TimelineProjector::build(const std::vector<CascadeEvent>& cascades,
                                                    const int horizon_days) const {
  std::vector<TimelineEvent> out;
  out.reserve(cascades.size());

  for (const auto& cascade : cascades) {
    out.push_back(TimelineEvent{
        .day = std::min(cascade.lag_days, horizon_days),
        .domain = cascade.domain,
        .probability = std::clamp(cascade.magnitude + 0.2, 0.0, 1.0),
        .event_description = cascade.description,
    });
  }
  return out;
}

} // namespace olympus

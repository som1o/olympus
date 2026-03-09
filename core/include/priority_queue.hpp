#pragma once

#include "signal.hpp"

#include <optional>
#include <queue>

namespace olympus {

struct UrgencyItem {
  Signal signal;
  double score{0.0};
  std::size_t sequence{0};
};

class UrgencyQueue {
public:
  void push(UrgencyItem item);
  std::optional<UrgencyItem> pop();
  std::size_t size() const;

private:
  struct Compare {
    bool operator()(const UrgencyItem& a, const UrgencyItem& b) const;
  };

  std::priority_queue<UrgencyItem, std::vector<UrgencyItem>, Compare> queue_;
};

} // namespace olympus

#include "priority_queue.hpp"

namespace olympus {

bool UrgencyQueue::Compare::operator()(const UrgencyItem& a, const UrgencyItem& b) const {
  if (a.score == b.score) {
    return a.sequence > b.sequence;
  }
  return a.score < b.score;
}

void UrgencyQueue::push(UrgencyItem item) { queue_.push(std::move(item)); }

std::optional<UrgencyItem> UrgencyQueue::pop() {
  if (queue_.empty()) {
    return std::nullopt;
  }
  UrgencyItem item = queue_.top();
  queue_.pop();
  return item;
}

std::size_t UrgencyQueue::size() const { return queue_.size(); }

} // namespace olympus

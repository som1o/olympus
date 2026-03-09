#include "cache.hpp"

namespace olympus {

SignalCache::SignalCache(const std::size_t capacity) : capacity_(capacity) {}

void SignalCache::put(std::string key, Signal value) {
  if (const auto it = values_.find(key); it != values_.end()) {
    order_.erase(it->second.it);
    values_.erase(it);
  }

  order_.push_front(key);
  values_.emplace(key, Entry{std::move(value), order_.begin()});

  if (values_.size() > capacity_) {
    const std::string& victim = order_.back();
    values_.erase(victim);
    order_.pop_back();
  }
}

std::optional<Signal> SignalCache::get(const std::string& key) {
  if (const auto it = values_.find(key); it != values_.end()) {
    order_.erase(it->second.it);
    order_.push_front(key);
    it->second.it = order_.begin();
    return it->second.value;
  }
  return std::nullopt;
}

std::size_t SignalCache::size() const { return values_.size(); }

} // namespace olympus

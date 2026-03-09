#pragma once

#include "signal.hpp"

#include <cstddef>
#include <list>
#include <optional>
#include <string>
#include <unordered_map>

namespace olympus {

class SignalCache {
public:
  explicit SignalCache(std::size_t capacity);

  void put(std::string key, Signal value);
  std::optional<Signal> get(const std::string& key);
  std::size_t size() const;

private:
  using ListIt = std::list<std::string>::iterator;

  struct Entry {
    Signal value;
    ListIt it;
  };

  std::size_t capacity_;
  std::list<std::string> order_;
  std::unordered_map<std::string, Entry> values_;
};

} // namespace olympus

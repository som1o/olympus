#pragma once

#include "signal.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace olympus {

class NotificationPanel {
public:
  void push(const Signal& signal);
  const std::vector<Signal>& items() const;
  std::string latest_line() const;
  std::string as_text(std::size_t max_items) const;

private:
  static std::string severity(const Signal& signal);

  std::vector<Signal> notifications_;
  std::size_t max_items_{256};
};

} // namespace olympus

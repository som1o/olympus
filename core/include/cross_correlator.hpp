#pragma once

#include "signal.hpp"

#include <string>

namespace olympus {

class CrossCorrelator {
public:
  std::string correlate(const Signal& signal) const;
};

} // namespace olympus

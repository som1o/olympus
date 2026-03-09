#pragma once

#include "signal.hpp"

namespace olympus {

class Normalizer {
public:
  Signal normalize(const Signal& signal) const;
};

} // namespace olympus

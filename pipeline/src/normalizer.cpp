#include "normalizer.hpp"

#include <algorithm>
#include <cctype>

namespace olympus {

Signal Normalizer::normalize(const Signal& signal) const {
  Signal normalized = signal;
  normalized.urgency = std::clamp(signal.urgency, 0.0, 1.0);

  std::transform(normalized.type.begin(), normalized.type.end(), normalized.type.begin(),
                 [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  return normalized;
}

} // namespace olympus

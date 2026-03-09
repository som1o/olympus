#pragma once

#include "signal.hpp"

#include <string>

namespace olympus {

class Validator {
public:
  bool validate(const Signal& signal, std::string& error) const;
};

} // namespace olympus

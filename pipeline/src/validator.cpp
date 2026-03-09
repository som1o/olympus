#include "validator.hpp"

namespace olympus {

bool Validator::validate(const Signal& signal, std::string& error) const {
  if (signal.signal_id.empty()) {
    error = "missing signal_id";
    return false;
  }
  if (signal.domain == Domain::Unknown) {
    error = "unknown domain";
    return false;
  }
  if (signal.urgency < 0.0 || signal.urgency > 1.0) {
    error = "urgency out of range";
    return false;
  }
  error.clear();
  return true;
}

} // namespace olympus

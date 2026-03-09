#pragma once

#include "decision_ledger.hpp"

#include <string>

namespace olympus {

class HistoryView {
public:
  std::string summarize(const DecisionLedger& ledger) const;
};

} // namespace olympus

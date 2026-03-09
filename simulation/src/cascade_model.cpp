#include "cascade_model.hpp"

namespace olympus {

std::vector<CascadeEvent> CascadeModel::propagate(const DecisionProposal& proposal) const {
  if (proposal.domain == Domain::Resources) {
    return {
        {Domain::Governance, 0.42, 14, "Budget pressure from imports"},
        {Domain::Foreign, 0.37, 21, "Trade posture shift due to emergency procurement"},
        {Domain::Local, 0.28, 30, "Municipal stability improves with supply recovery"},
    };
  }

  return {
      {Domain::Governance, 0.20, 10, "Moderate cross-domain impact expected"},
      {Domain::Local, 0.16, 20, "Local services may adjust"},
  };
}

} // namespace olympus

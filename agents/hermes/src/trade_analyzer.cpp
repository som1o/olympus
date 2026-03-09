#include "trade_analyzer.hpp"

#include <algorithm>

namespace olympus {

void TradeAnalyzer::bootstrap() { bootstrapped_ = true; }

std::string TradeAnalyzer::impact_brief(const DecisionProposal& proposal, const double tension_score) const {
	const double stress = std::clamp(tension_score, 0.0, 1.0);
	if (stress > 0.8 || proposal.action_type.find("sanction") != std::string::npos) {
		return "high trade disruption risk; pre-stage import substitutions";
	}
	if (stress > 0.55) {
		return "moderate trade volatility; activate bilateral stabilizers";
	}
	return "low trade impact expected";
}

} // namespace olympus

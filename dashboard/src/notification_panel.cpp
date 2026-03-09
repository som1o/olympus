#include "notification_panel.hpp"

#include <algorithm>
#include <sstream>

namespace olympus {

void NotificationPanel::push(const Signal& signal) {
	notifications_.push_back(signal);
	if (notifications_.size() > max_items_) {
		notifications_.erase(notifications_.begin(), notifications_.begin() +
																										static_cast<std::ptrdiff_t>(notifications_.size() - max_items_));
	}
}

const std::vector<Signal>& NotificationPanel::items() const { return notifications_; }

std::string NotificationPanel::latest_line() const {
	if (notifications_.empty()) {
		return "No notifications yet";
	}

	const Signal& s = notifications_.back();
	return "[" + severity(s) + "] " + to_string(s.domain) + " :: " + s.type +
				 " (urgency=" + std::to_string(s.urgency) + ")";
}

std::string NotificationPanel::as_text(const std::size_t max_items) const {
	if (notifications_.empty()) {
		return "No notifications yet";
	}

	const std::size_t count = std::min(max_items, notifications_.size());
	const std::size_t start = notifications_.size() - count;

	std::ostringstream out;
	for (std::size_t i = start; i < notifications_.size(); ++i) {
		const Signal& s = notifications_[i];
		out << "- [" << severity(s) << "] " << to_string(s.domain) << " :: " << s.type
				<< " urgency=" << s.urgency;
		if (i + 1 < notifications_.size()) {
			out << '\n';
		}
	}
	return out.str();
}

std::string NotificationPanel::severity(const Signal& signal) {
	if (signal.urgency >= 0.95) {
		return "BLACK";
	}
	if (signal.urgency >= 0.8) {
		return "RED";
	}
	if (signal.urgency >= 0.65) {
		return "ORANGE";
	}
	if (signal.urgency >= 0.45) {
		return "YELLOW";
	}
	return "GREEN";
}

} // namespace olympus

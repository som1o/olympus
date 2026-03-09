#include "db_explorer.hpp"

#include <array>
#include <sstream>

namespace olympus {

std::string DBExplorer::status(const DBConnector& connector) const {
  static constexpr std::array<Domain, 8> domains{
      Domain::Defense, Domain::Governance, Domain::Education, Domain::Health,
      Domain::Local, Domain::Foreign, Domain::Resources, Domain::Technology};

  std::ostringstream out;
  std::size_t up = 0;
  out << "Database Connectivity\n";
  out << "---------------------\n";
  for (const auto domain : domains) {
    const bool is_up = connector.connected(domain);
    if (is_up) {
      ++up;
    }
    out << to_string(domain) << ": " << (is_up ? "UP" : "DOWN") << '\n';
  }

  out << "\nSummary: " << up << "/" << domains.size() << " databases connected";
  return out.str();
}

} // namespace olympus

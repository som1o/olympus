#pragma once

#include "db_connector.hpp"

#include <string>

namespace olympus {

class DBExplorer {
public:
  std::string status(const DBConnector& connector) const;
};

} // namespace olympus

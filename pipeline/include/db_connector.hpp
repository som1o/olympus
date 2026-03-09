#pragma once

#include "signal.hpp"

#include <chrono>
#include <deque>
#include <memory>
#include <string>
#include <unordered_map>

#ifdef OLYMPUS_WITH_PQXX
#include <pqxx/pqxx>
#endif

namespace olympus {

struct CDCEvent {
  Domain domain{Domain::Unknown};
  std::string channel;
  std::string payload;
  std::chrono::system_clock::time_point received_at;
};

class DBConnector {
public:
  void configure_database(Domain domain, std::string conn_str);
  bool connect_all();
  bool connected(Domain domain) const;

  bool listen(Domain domain, const std::string& channel);
  std::vector<CDCEvent> poll_cdc(std::chrono::milliseconds timeout);

  // Test helper for deterministic CDC without a live database.
  void inject_cdc_for_test(CDCEvent event);

private:
  struct ConnectionState {
    std::string conn_str;
    bool connected{false};
#ifdef OLYMPUS_WITH_PQXX
    std::unique_ptr<pqxx::connection> conn;
#endif
  };

  std::unordered_map<Domain, std::string> configs_;
  std::unordered_map<Domain, bool> states_;
  std::unordered_map<Domain, ConnectionState> connections_;
  std::deque<CDCEvent> injected_events_;
};

} // namespace olympus

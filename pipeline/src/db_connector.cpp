#include "db_connector.hpp"

#include <thread>

#ifdef OLYMPUS_WITH_PQXX
#include <libpq-fe.h>
#endif

namespace olympus {

void DBConnector::configure_database(const Domain domain, std::string conn_str) {
  configs_[domain] = std::move(conn_str);
  ConnectionState state{};
  state.conn_str = configs_[domain];
  connections_[domain] = std::move(state);
}

bool DBConnector::connect_all() {
  bool at_least_one = false;
  for (auto& [domain, state] : connections_) {
#ifdef OLYMPUS_WITH_PQXX
    try {
      state.conn = std::make_unique<pqxx::connection>(state.conn_str);
      state.connected = state.conn->is_open();
    } catch (...) {
      state.connected = false;
      state.conn.reset();
    }
#else
    state.connected = !state.conn_str.empty();
#endif
    states_[domain] = state.connected;
    at_least_one = at_least_one || state.connected;
  }
  return at_least_one;
}

bool DBConnector::connected(const Domain domain) const {
  if (const auto it = states_.find(domain); it != states_.end()) {
    return it->second;
  }
  return false;
}

bool DBConnector::listen(const Domain domain, const std::string& channel) {
  const auto it = connections_.find(domain);
  if (it == connections_.end()) {
    return false;
  }

#ifdef OLYMPUS_WITH_PQXX
  if (!it->second.connected || !it->second.conn) {
    return false;
  }

  try {
    pqxx::work tx{*it->second.conn};
    tx.exec("LISTEN " + tx.quote_name(channel));
    tx.commit();
    return true;
  } catch (...) {
    return false;
  }
#else
  (void)channel;
  return it->second.connected;
#endif
}

std::vector<CDCEvent> DBConnector::poll_cdc(const std::chrono::milliseconds timeout) {
  std::vector<CDCEvent> out;

  while (!injected_events_.empty()) {
    out.push_back(std::move(injected_events_.front()));
    injected_events_.pop_front();
  }

#ifdef OLYMPUS_WITH_PQXX
  const auto deadline = std::chrono::steady_clock::now() + timeout;
  while (std::chrono::steady_clock::now() < deadline) {
    bool saw_any = false;
    for (auto& [domain, state] : connections_) {
      if (!state.connected || !state.conn) {
        continue;
      }

      PGconn* raw = state.conn->raw_connection();
      if (!raw) {
        continue;
      }

      if (PQconsumeInput(raw) == 0) {
        continue;
      }

      while (PGnotify* notify = PQnotifies(raw)) {
        CDCEvent event{};
        event.domain = domain;
        event.channel = notify->relname != nullptr ? notify->relname : "";
        event.payload = notify->extra != nullptr ? notify->extra : "";
        event.received_at = std::chrono::system_clock::now();
        out.push_back(std::move(event));
        PQfreemem(notify);
        saw_any = true;
      }
    }

    if (saw_any) {
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }
#else
  (void)timeout;
#endif

  return out;
}

void DBConnector::inject_cdc_for_test(CDCEvent event) {
  if (event.received_at.time_since_epoch().count() == 0) {
    event.received_at = std::chrono::system_clock::now();
  }
  injected_events_.push_back(std::move(event));
}

} // namespace olympus

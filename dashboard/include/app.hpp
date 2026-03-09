#pragma once

#include "audit_stream.hpp"
#include "auth_dialog.hpp"
#include "db_explorer.hpp"
#include "decision_review.hpp"
#include "history_view.hpp"
#include "notification_panel.hpp"
#include "sim_viewer.hpp"

#include "normalizer.hpp"
#include "prometheus.hpp"
#include "stream_processor.hpp"
#include "themis.hpp"
#include "validator.hpp"

#include <gtk/gtk.h>

#include <array>
#include <memory>
#include <optional>
#include <string>

namespace olympus {

struct DashboardUiState;

struct PendingAuthorization {
  Signal signal;
  DecisionProposal proposal;
  SimulationReport report;
};

class DashboardApp {
public:
  DashboardApp(bool auto_authorize_noncritical, int refresh_ms, std::size_t max_signals_per_tick);
  ~DashboardApp();

  int run_live(StreamProcessor& stream, Validator& validator, Normalizer& normalizer,
               Themis& themis, Prometheus& prometheus, DBConnector& connector);

  bool process(const Signal& signal, const DecisionProposal& proposal, const SimulationReport& report,
               DecisionLedger& ledger, const DBConnector& connector);

private:
  static constexpr std::size_t kDomainCount = 8;

  void build_ui(GtkApplication* app);
  void on_pause_clicked_internal();
  void on_login_clicked_internal(std::size_t index);
  void on_authorize_pending_internal(bool authorize);
  void persist_action(const std::string& actor, const std::string& action,
                      const std::string& detail);
  std::size_t logged_in_operator_count() const;
  std::size_t required_consensus() const;
  std::string operator_name(std::size_t index) const;
  static std::size_t domain_index(Domain domain);
  void update_domain_stats(const Signal& signal);

  static void on_activate(GtkApplication* app, gpointer user_data);
  static gboolean on_tick_callback(gpointer user_data);
  static void on_pause_clicked(GtkButton* button, gpointer user_data);
  static void on_login_a_clicked(GtkButton* button, gpointer user_data);
  static void on_login_b_clicked(GtkButton* button, gpointer user_data);
  static void on_approve_pending_clicked(GtkButton* button, gpointer user_data);
  static void on_veto_pending_clicked(GtkButton* button, gpointer user_data);
  static void on_domain_chart_draw(GtkDrawingArea* area, cairo_t* cr,
                                   int width, int height, gpointer user_data);

  bool on_tick();
  void update_ui_state();

  bool auto_authorize_noncritical_;
  int refresh_ms_{250};
  std::size_t max_signals_per_tick_{64};
  std::string latest_notification_;
  std::string latest_proposal_;
  std::string latest_simulation_;
  std::string latest_history_;
  std::string latest_db_status_;
  std::string latest_system_status_;
  std::string latest_audit_status_;

  std::size_t processed_signals_{0};
  std::size_t proposals_generated_{0};
  std::size_t authorized_count_{0};
  std::size_t vetoed_count_{0};
  bool paused_{false};
  bool operators_logged_in_[2]{false, false};

  std::array<std::size_t, kDomainCount> domain_signal_counts_{};
  std::array<std::size_t, kDomainCount> domain_authorized_counts_{};
  std::array<std::size_t, kDomainCount> domain_vetoed_counts_{};
  std::array<double, kDomainCount> domain_last_urgency_{};

  std::optional<PendingAuthorization> pending_critical_;

  NotificationPanel notifications_;
  DecisionReview review_;
  AuthDialog auth_;
  AuditStream audit_stream_;
  SimViewer sim_viewer_;
  HistoryView history_;
  DBExplorer db_explorer_;

  StreamProcessor* stream_{nullptr};
  Validator* validator_{nullptr};
  Normalizer* normalizer_{nullptr};
  Themis* themis_{nullptr};
  Prometheus* prometheus_{nullptr};
  DBConnector* connector_{nullptr};
  DecisionLedger* ledger_{nullptr};

  std::unique_ptr<DashboardUiState> ui_;
};

} // namespace olympus

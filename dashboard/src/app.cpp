#include "app.hpp"

#include "logger.hpp"

#include <algorithm>
#include <array>
#include <stdexcept>

#include <gtk/gtk.h>

namespace olympus {

struct DashboardUiState {
  GtkApplication* app{nullptr};
  GtkWidget* window{nullptr};
  GtkWidget* status_label{nullptr};
  GtkWidget* pending_label{nullptr};

  GtkWidget* op_name_entry_a{nullptr};
  GtkWidget* op_name_entry_b{nullptr};
  GtkWidget* op_login_button_a{nullptr};
  GtkWidget* op_login_button_b{nullptr};
  GtkWidget* consensus_spin{nullptr};
  GtkWidget* approve_pending_button{nullptr};
  GtkWidget* veto_pending_button{nullptr};

  GtkWidget* pause_button{nullptr};
  GtkWidget* domain_chart{nullptr};
  GtkWidget* domain_card_labels[8]{};

  GtkWidget* feed_text{nullptr};
  GtkWidget* review_text{nullptr};
  GtkWidget* sim_text{nullptr};
  GtkWidget* history_text{nullptr};
  GtkWidget* db_text{nullptr};
  GtkWidget* system_text{nullptr};
  GtkWidget* audit_text{nullptr};

  bool ready{false};
};

namespace {
constexpr std::array<Domain, 8> kDomains{
    Domain::Defense, Domain::Governance, Domain::Education, Domain::Health,
    Domain::Local, Domain::Foreign, Domain::Resources, Domain::Technology};

GtkWidget* make_text_view() {
  GtkWidget* view = gtk_text_view_new();
  gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view), FALSE);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD_CHAR);
  return view;
}

void set_text(GtkWidget* view, const std::string& text) {
  GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
  gtk_text_buffer_set_text(buffer, text.c_str(), static_cast<gint>(text.size()));
}
} // namespace

DashboardApp::DashboardApp(const bool auto_authorize_noncritical, const int refresh_ms,
                           const std::size_t max_signals_per_tick)
    : auto_authorize_noncritical_(auto_authorize_noncritical),
      refresh_ms_(refresh_ms),
      max_signals_per_tick_(max_signals_per_tick),
      ui_(std::make_unique<DashboardUiState>()) {
  ui_->app = gtk_application_new("com.olympus.dashboard", G_APPLICATION_DEFAULT_FLAGS);
  if (ui_->app == nullptr) {
    throw std::runtime_error("failed to initialize GTK dashboard application");
  }

  g_signal_connect(ui_->app, "activate", G_CALLBACK(DashboardApp::on_activate), this);
  latest_system_status_ = "Dashboard initialized";
  latest_audit_status_ = "No audit events yet";
}

DashboardApp::~DashboardApp() {
  if (ui_ && ui_->app != nullptr) {
    g_object_unref(ui_->app);
    ui_->app = nullptr;
  }
}

int DashboardApp::run_live(StreamProcessor& stream, Validator& validator,
                           Normalizer& normalizer, Themis& themis,
                           Prometheus& prometheus, DBConnector& connector) {
  stream_ = &stream;
  validator_ = &validator;
  normalizer_ = &normalizer;
  themis_ = &themis;
  prometheus_ = &prometheus;
  connector_ = &connector;
  ledger_ = &themis.ledger_mutable();

  persist_action("system", "dashboard_started", "live runtime initialized");

  g_timeout_add(static_cast<guint>(refresh_ms_), DashboardApp::on_tick_callback, this);
  if (ui_ && ui_->app != nullptr) {
    return g_application_run(G_APPLICATION(ui_->app), 0, nullptr);
  }
  return 1;
}

void DashboardApp::build_ui(GtkApplication* app) {
  auto& ui = *ui_;

  ui.window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(ui.window), "OLYMPUS Dashboard");
  gtk_window_set_default_size(GTK_WINDOW(ui.window), 1600, 980);

  GtkWidget* root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
  gtk_widget_set_margin_top(root, 10);
  gtk_widget_set_margin_bottom(root, 10);
  gtk_widget_set_margin_start(root, 10);
  gtk_widget_set_margin_end(root, 10);
  gtk_window_set_child(GTK_WINDOW(ui.window), root);

  GtkWidget* top_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
  gtk_box_append(GTK_BOX(root), top_bar);

  ui.status_label = gtk_label_new("OLYMPUS booting...");
  gtk_label_set_xalign(GTK_LABEL(ui.status_label), 0.0F);
  gtk_widget_set_hexpand(ui.status_label, TRUE);
  gtk_box_append(GTK_BOX(top_bar), ui.status_label);

  ui.pause_button = gtk_button_new_with_label("Pause Stream");
  gtk_box_append(GTK_BOX(top_bar), ui.pause_button);
  g_signal_connect(ui.pause_button, "clicked", G_CALLBACK(DashboardApp::on_pause_clicked), this);

  GtkWidget* auth_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
  gtk_box_append(GTK_BOX(root), auth_bar);

  ui.op_name_entry_a = gtk_entry_new();
  gtk_editable_set_text(GTK_EDITABLE(ui.op_name_entry_a), "operator-alpha");
  gtk_widget_set_size_request(ui.op_name_entry_a, 150, -1);
  gtk_box_append(GTK_BOX(auth_bar), ui.op_name_entry_a);

  ui.op_login_button_a = gtk_button_new_with_label("Login A");
  gtk_box_append(GTK_BOX(auth_bar), ui.op_login_button_a);
  g_signal_connect(ui.op_login_button_a, "clicked", G_CALLBACK(DashboardApp::on_login_a_clicked), this);

  ui.op_name_entry_b = gtk_entry_new();
  gtk_editable_set_text(GTK_EDITABLE(ui.op_name_entry_b), "operator-beta");
  gtk_widget_set_size_request(ui.op_name_entry_b, 150, -1);
  gtk_box_append(GTK_BOX(auth_bar), ui.op_name_entry_b);

  ui.op_login_button_b = gtk_button_new_with_label("Login B");
  gtk_box_append(GTK_BOX(auth_bar), ui.op_login_button_b);
  g_signal_connect(ui.op_login_button_b, "clicked", G_CALLBACK(DashboardApp::on_login_b_clicked), this);

  GtkAdjustment* consensus_adj = gtk_adjustment_new(2.0, 1.0, 2.0, 1.0, 0.0, 0.0);
  ui.consensus_spin = gtk_spin_button_new(consensus_adj, 1.0, 0);
  gtk_box_append(GTK_BOX(auth_bar), gtk_label_new("Consensus M:"));
  gtk_box_append(GTK_BOX(auth_bar), ui.consensus_spin);

  ui.approve_pending_button = gtk_button_new_with_label("Approve Pending Critical");
  gtk_box_append(GTK_BOX(auth_bar), ui.approve_pending_button);
  g_signal_connect(ui.approve_pending_button, "clicked",
                   G_CALLBACK(DashboardApp::on_approve_pending_clicked), this);

  ui.veto_pending_button = gtk_button_new_with_label("Veto Pending Critical");
  gtk_box_append(GTK_BOX(auth_bar), ui.veto_pending_button);
  g_signal_connect(ui.veto_pending_button, "clicked",
                   G_CALLBACK(DashboardApp::on_veto_pending_clicked), this);

  ui.pending_label = gtk_label_new("Pending critical: none");
  gtk_label_set_xalign(GTK_LABEL(ui.pending_label), 0.0F);
  gtk_widget_set_hexpand(ui.pending_label, TRUE);
  gtk_box_append(GTK_BOX(root), ui.pending_label);

  GtkWidget* domain_section = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
  gtk_box_append(GTK_BOX(root), domain_section);

  ui.domain_chart = gtk_drawing_area_new();
  gtk_drawing_area_set_content_width(GTK_DRAWING_AREA(ui.domain_chart), 700);
  gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(ui.domain_chart), 220);
  gtk_widget_set_hexpand(ui.domain_chart, TRUE);
  gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(ui.domain_chart),
                                 DashboardApp::on_domain_chart_draw, this, nullptr);
  gtk_box_append(GTK_BOX(domain_section), ui.domain_chart);

  GtkWidget* cards_grid = gtk_grid_new();
  gtk_grid_set_row_spacing(GTK_GRID(cards_grid), 6);
  gtk_grid_set_column_spacing(GTK_GRID(cards_grid), 6);
  gtk_box_append(GTK_BOX(domain_section), cards_grid);

  for (std::size_t i = 0; i < kDomainCount; ++i) {
    GtkWidget* frame = gtk_frame_new(to_string(kDomains[i]).c_str());
    GtkWidget* label = gtk_label_new("signals=0\nauth=0 veto=0\nurgency=0.0");
    gtk_label_set_xalign(GTK_LABEL(label), 0.0F);
    gtk_widget_set_margin_top(label, 6);
    gtk_widget_set_margin_bottom(label, 6);
    gtk_widget_set_margin_start(label, 6);
    gtk_widget_set_margin_end(label, 6);
    gtk_frame_set_child(GTK_FRAME(frame), label);
    gtk_grid_attach(GTK_GRID(cards_grid), frame,
                    static_cast<int>(i % 2), static_cast<int>(i / 2), 1, 1);
    ui.domain_card_labels[i] = label;
  }

  GtkWidget* main_split = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_widget_set_vexpand(main_split, TRUE);
  gtk_box_append(GTK_BOX(root), main_split);

  GtkWidget* feed_scroll = gtk_scrolled_window_new();
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(feed_scroll),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  ui.feed_text = make_text_view();
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(feed_scroll), ui.feed_text);
  gtk_widget_set_hexpand(feed_scroll, TRUE);
  gtk_widget_set_vexpand(feed_scroll, TRUE);
  gtk_paned_set_start_child(GTK_PANED(main_split), feed_scroll);

  GtkWidget* notebook = gtk_notebook_new();
  gtk_widget_set_hexpand(notebook, TRUE);
  gtk_widget_set_vexpand(notebook, TRUE);
  gtk_paned_set_end_child(GTK_PANED(main_split), notebook);

  auto add_tab = [&](const char* title, GtkWidget** out_view) {
    GtkWidget* scroller = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroller),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    GtkWidget* view = make_text_view();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroller), view);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), scroller, gtk_label_new(title));
    *out_view = view;
  };

  add_tab("Decision Review", &ui.review_text);
  add_tab("Simulation", &ui.sim_text);
  add_tab("Decision History", &ui.history_text);
  add_tab("Database Health", &ui.db_text);
  add_tab("System Metrics", &ui.system_text);
  add_tab("Audit Stream", &ui.audit_text);

  ui.ready = true;
  update_ui_state();
  gtk_window_present(GTK_WINDOW(ui.window));
}

bool DashboardApp::process(const Signal& signal, const DecisionProposal& proposal,
                           const SimulationReport& report, DecisionLedger& ledger,
                           const DBConnector& connector) {
  notifications_.push(signal);

  latest_notification_ = notifications_.latest_line();
  latest_proposal_ = review_.summarize(proposal);
  latest_simulation_ = sim_viewer_.render(report);
  latest_history_ = history_.summarize(ledger);
  latest_db_status_ = db_explorer_.status(connector);

  if (review_.is_critical(proposal)) {
    if (!pending_critical_.has_value()) {
      pending_critical_ = PendingAuthorization{signal, proposal, report};
      latest_system_status_ = "Critical proposal pending consensus: " + proposal.proposal_id;
      persist_action("system", "critical_pending", proposal.proposal_id);
    }
    update_ui_state();
    return false;
  }

  std::string reason;
  bool authorized = false;
  if (logged_in_operator_count() == 0) {
    reason = "no logged-in operator";
  } else {
    authorized = auth_.authorize(proposal, auto_authorize_noncritical_, reason);
  }

  if (authorized) {
    ++authorized_count_;
    ++domain_authorized_counts_[domain_index(proposal.domain)];
    ledger.append_authorization(proposal, AuthorizationStatus::Authorized, reason);
    persist_action(operator_name(0), "approve_noncritical", proposal.proposal_id + " " + reason);
  } else {
    ++vetoed_count_;
    ++domain_vetoed_counts_[domain_index(proposal.domain)];
    ledger.append_authorization(proposal, AuthorizationStatus::Vetoed, reason);
    persist_action("policy", "veto_noncritical", proposal.proposal_id + " " + reason);
  }

  latest_system_status_ = "Authorization: " + std::string(authorized ? "AUTHORIZED" : "VETOED") +
                          " (" + reason + ")";
  update_ui_state();
  return authorized;
}

bool DashboardApp::on_tick() {
  if (stream_ == nullptr || validator_ == nullptr || normalizer_ == nullptr ||
      themis_ == nullptr || prometheus_ == nullptr || connector_ == nullptr ||
      ledger_ == nullptr) {
    latest_system_status_ = "Runtime context not initialized";
    update_ui_state();
    return true;
  }

  if (paused_) {
    update_ui_state();
    return true;
  }

  const auto raw_signals = stream_->poll();
  std::size_t accepted = 0;
  std::size_t proposals = 0;

  for (const auto& raw : raw_signals) {
    if (accepted >= max_signals_per_tick_) {
      break;
    }

    std::string error;
    if (!validator_->validate(raw, error)) {
      latest_system_status_ = "Rejected signal: " + error;
      continue;
    }

    const Signal signal = normalizer_->normalize(raw);
    update_domain_stats(signal);
    themis_->ingest(signal);
    ++accepted;
    ++processed_signals_;

    const auto proposal = themis_->propose();
    if (!proposal.has_value()) {
      continue;
    }

    const SimulationReport report = prometheus_->simulate(*proposal);
    process(signal, *proposal, report, *ledger_, *connector_);
    ++proposals;
    ++proposals_generated_;
  }

  latest_system_status_ = "tick accepted=" + std::to_string(accepted) +
                          " proposals=" + std::to_string(proposals) +
                          " processed_total=" + std::to_string(processed_signals_) +
                          " authorized=" + std::to_string(authorized_count_) +
                          " vetoed=" + std::to_string(vetoed_count_);
  update_ui_state();
  return true;
}

void DashboardApp::update_ui_state() {
  if (!ui_ || !ui_->ready) {
    return;
  }

  if (ui_->status_label != nullptr) {
    gtk_label_set_text(GTK_LABEL(ui_->status_label), latest_system_status_.c_str());
  }

  if (ui_->pending_label != nullptr) {
    if (pending_critical_.has_value()) {
      gtk_label_set_text(GTK_LABEL(ui_->pending_label),
                         ("Pending critical: " + pending_critical_->proposal.proposal_id).c_str());
    } else {
      gtk_label_set_text(GTK_LABEL(ui_->pending_label), "Pending critical: none");
    }
  }

  if (ui_->op_login_button_a != nullptr) {
    gtk_button_set_label(GTK_BUTTON(ui_->op_login_button_a),
                         operators_logged_in_[0] ? "Logout A" : "Login A");
  }
  if (ui_->op_login_button_b != nullptr) {
    gtk_button_set_label(GTK_BUTTON(ui_->op_login_button_b),
                         operators_logged_in_[1] ? "Logout B" : "Login B");
  }

  if (ui_->feed_text != nullptr) {
    set_text(ui_->feed_text, notifications_.as_text(120));
  }
  if (ui_->review_text != nullptr) {
    set_text(ui_->review_text, latest_proposal_.empty() ? "No proposal yet" : latest_proposal_);
  }
  if (ui_->sim_text != nullptr) {
    set_text(ui_->sim_text, latest_simulation_.empty() ? "No simulation yet" : latest_simulation_);
  }
  if (ui_->history_text != nullptr) {
    set_text(ui_->history_text, latest_history_.empty() ? "No history yet" : latest_history_);
  }
  if (ui_->db_text != nullptr) {
    set_text(ui_->db_text, latest_db_status_.empty() ? "No DB status yet" : latest_db_status_);
  }
  if (ui_->system_text != nullptr) {
    set_text(ui_->system_text,
             "processed_signals=" + std::to_string(processed_signals_) +
                 "\nproposals_generated=" + std::to_string(proposals_generated_) +
                 "\nauthorized=" + std::to_string(authorized_count_) +
                 "\nvetoed=" + std::to_string(vetoed_count_) +
                 "\nlogged_operators=" + std::to_string(logged_in_operator_count()) +
                 "\nconsensus_required=" + std::to_string(required_consensus()) +
                 "\nrefresh_ms=" + std::to_string(refresh_ms_) +
                 "\nmax_signals_per_tick=" + std::to_string(max_signals_per_tick_) +
                 "\npaused=" + std::string(paused_ ? "true" : "false"));
  }
  if (ui_->audit_text != nullptr) {
    set_text(ui_->audit_text, audit_stream_.as_text(200));
  }

  for (std::size_t i = 0; i < kDomainCount; ++i) {
    if (ui_->domain_card_labels[i] != nullptr) {
      std::string card = "signals=" + std::to_string(domain_signal_counts_[i]) +
                         "\nauthorized=" + std::to_string(domain_authorized_counts_[i]) +
                         " vetoed=" + std::to_string(domain_vetoed_counts_[i]) +
                         "\nlast_urgency=" + std::to_string(domain_last_urgency_[i]);
      gtk_label_set_text(GTK_LABEL(ui_->domain_card_labels[i]), card.c_str());
    }
  }

  if (ui_->domain_chart != nullptr) {
    gtk_widget_queue_draw(ui_->domain_chart);
  }
}

void DashboardApp::on_pause_clicked_internal() {
  paused_ = !paused_;
  if (ui_ && ui_->pause_button != nullptr) {
    gtk_button_set_label(GTK_BUTTON(ui_->pause_button), paused_ ? "Resume Stream" : "Pause Stream");
  }
  latest_system_status_ = paused_ ? "Stream paused by operator" : "Stream resumed by operator";
  persist_action("operator", paused_ ? "pause" : "resume", "stream state changed");
  update_ui_state();
}

void DashboardApp::on_login_clicked_internal(const std::size_t index) {
  if (index >= 2) {
    return;
  }

  operators_logged_in_[index] = !operators_logged_in_[index];
  const std::string name = operator_name(index);
  persist_action(name, operators_logged_in_[index] ? "login" : "logout", "operator session state");
  latest_system_status_ = name + (operators_logged_in_[index] ? " logged in" : " logged out");
  update_ui_state();
}

void DashboardApp::on_authorize_pending_internal(const bool authorize) {
  if (!pending_critical_.has_value() || ledger_ == nullptr) {
    latest_system_status_ = "No pending critical proposal";
    update_ui_state();
    return;
  }

  const std::size_t votes = logged_in_operator_count();
  const std::size_t required = required_consensus();
  if (authorize && votes < required) {
    latest_system_status_ = "Consensus not met: votes=" + std::to_string(votes) +
                            " required=" + std::to_string(required);
    update_ui_state();
    return;
  }

  const auto pending = *pending_critical_;
  pending_critical_.reset();

  const std::string actor = operator_name(0) + "+" + operator_name(1);
  if (authorize) {
    ++authorized_count_;
    ++domain_authorized_counts_[domain_index(pending.proposal.domain)];
    ledger_->append_authorization(pending.proposal, AuthorizationStatus::Authorized,
                                  "critical consensus reached");
    persist_action(actor, "approve_critical",
                   pending.proposal.proposal_id + " votes=" + std::to_string(votes));
    latest_system_status_ = "Critical proposal authorized: " + pending.proposal.proposal_id;
  } else {
    ++vetoed_count_;
    ++domain_vetoed_counts_[domain_index(pending.proposal.domain)];
    ledger_->append_authorization(pending.proposal, AuthorizationStatus::Vetoed,
                                  "critical veto by operator(s)");
    persist_action(actor, "veto_critical", pending.proposal.proposal_id);
    latest_system_status_ = "Critical proposal vetoed: " + pending.proposal.proposal_id;
  }

  update_ui_state();
}

void DashboardApp::persist_action(const std::string& actor, const std::string& action,
                                  const std::string& detail) {
  audit_stream_.record(actor, action, detail, ledger_);
  latest_audit_status_ = actor + " " + action + " " + detail;
}

std::size_t DashboardApp::logged_in_operator_count() const {
  return static_cast<std::size_t>(operators_logged_in_[0]) +
         static_cast<std::size_t>(operators_logged_in_[1]);
}

std::size_t DashboardApp::required_consensus() const {
  if (ui_ && ui_->consensus_spin != nullptr) {
    const int value = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ui_->consensus_spin));
    return static_cast<std::size_t>(std::clamp(value, 1, 2));
  }
  return 2;
}

std::string DashboardApp::operator_name(const std::size_t index) const {
  if (!ui_ || index > 1) {
    return index == 0 ? "operator-alpha" : "operator-beta";
  }

  GtkWidget* entry = index == 0 ? ui_->op_name_entry_a : ui_->op_name_entry_b;
  if (entry == nullptr) {
    return index == 0 ? "operator-alpha" : "operator-beta";
  }

  const char* text = gtk_editable_get_text(GTK_EDITABLE(entry));
  if (text == nullptr || std::string(text).empty()) {
    return index == 0 ? "operator-alpha" : "operator-beta";
  }
  return text;
}

std::size_t DashboardApp::domain_index(const Domain domain) {
  switch (domain) {
  case Domain::Defense:
    return 0;
  case Domain::Governance:
    return 1;
  case Domain::Education:
    return 2;
  case Domain::Health:
    return 3;
  case Domain::Local:
    return 4;
  case Domain::Foreign:
    return 5;
  case Domain::Resources:
    return 6;
  case Domain::Technology:
    return 7;
  case Domain::Unknown:
    return 0;
  }
  return 0;
}

void DashboardApp::update_domain_stats(const Signal& signal) {
  const std::size_t idx = domain_index(signal.domain);
  ++domain_signal_counts_[idx];
  domain_last_urgency_[idx] = signal.urgency;
}

void DashboardApp::on_activate(GtkApplication* app, gpointer user_data) {
  auto* self = static_cast<DashboardApp*>(user_data);
  self->build_ui(app);
}

gboolean DashboardApp::on_tick_callback(gpointer user_data) {
  auto* self = static_cast<DashboardApp*>(user_data);
  return self->on_tick() ? G_SOURCE_CONTINUE : G_SOURCE_REMOVE;
}

void DashboardApp::on_pause_clicked(GtkButton*, gpointer user_data) {
  auto* self = static_cast<DashboardApp*>(user_data);
  self->on_pause_clicked_internal();
}

void DashboardApp::on_login_a_clicked(GtkButton*, gpointer user_data) {
  auto* self = static_cast<DashboardApp*>(user_data);
  self->on_login_clicked_internal(0);
}

void DashboardApp::on_login_b_clicked(GtkButton*, gpointer user_data) {
  auto* self = static_cast<DashboardApp*>(user_data);
  self->on_login_clicked_internal(1);
}

void DashboardApp::on_approve_pending_clicked(GtkButton*, gpointer user_data) {
  auto* self = static_cast<DashboardApp*>(user_data);
  self->on_authorize_pending_internal(true);
}

void DashboardApp::on_veto_pending_clicked(GtkButton*, gpointer user_data) {
  auto* self = static_cast<DashboardApp*>(user_data);
  self->on_authorize_pending_internal(false);
}

void DashboardApp::on_domain_chart_draw(GtkDrawingArea*, cairo_t* cr,
                                        const int width, const int height,
                                        gpointer user_data) {
  auto* self = static_cast<DashboardApp*>(user_data);

  cairo_set_source_rgb(cr, 0.08, 0.10, 0.13);
  cairo_rectangle(cr, 0, 0, width, height);
  cairo_fill(cr);

  const double margin = 20.0;
  const double plot_w = std::max(1.0, static_cast<double>(width) - margin * 2.0);
  const double plot_h = std::max(1.0, static_cast<double>(height) - margin * 2.0);
  const double bar_w = plot_w / static_cast<double>(kDomainCount);

  for (std::size_t i = 0; i < kDomainCount; ++i) {
    const double urgency = std::clamp(self->domain_last_urgency_[i], 0.0, 1.0);
    const double bar_h = plot_h * urgency;
    const double x = margin + static_cast<double>(i) * bar_w + 4.0;
    const double y = margin + (plot_h - bar_h);

    cairo_set_source_rgb(cr, 0.2 + urgency * 0.6, 0.7 - urgency * 0.4, 0.2);
    cairo_rectangle(cr, x, y, std::max(6.0, bar_w - 8.0), bar_h);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 0.9, 0.9, 0.95);
    cairo_move_to(cr, x, height - 4.0);
    cairo_show_text(cr, to_string(kDomains[i]).substr(0, 3).c_str());
  }
}

} // namespace olympus

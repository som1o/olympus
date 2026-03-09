cmake_minimum_required(VERSION 3.28)
project(olympus VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

option(OLYMPUS_STRICT_WARNINGS "Enable strict warnings" ON)
option(OLYMPUS_ENABLE_OPTIONAL_DEPS "Enable optional third-party deps" ON)
option(OLYMPUS_BUILD_TESTS "Build tests" ON)

set(OLYMPUS_HAVE_PQXX OFF)
set(OLYMPUS_HAVE_GTK4 OFF)
set(OLYMPUS_HAVE_ADWAITA OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)
set(OLYMPUS_HAVE_GTK4 ON)

if(OLYMPUS_STRICT_WARNINGS)
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Optional dependencies from roadmap; prototype builds without them.
if(OLYMPUS_ENABLE_OPTIONAL_DEPS)
  find_package(Protobuf QUIET)
  find_package(spdlog QUIET)
  find_package(libpqxx QUIET)
  find_package(OpenGL QUIET)

  if(libpqxx_FOUND)
    set(OLYMPUS_HAVE_PQXX ON)
  endif()

  pkg_check_modules(ADWAITA QUIET IMPORTED_TARGET libadwaita-1)
  if(ADWAITA_FOUND)
    set(OLYMPUS_HAVE_ADWAITA ON)
  endif()
endif()

add_library(olympus_pipeline
  pipeline/src/db_connector.cpp
  pipeline/src/stream_processor.cpp
  pipeline/src/normalizer.cpp
  pipeline/src/validator.cpp
  pipeline/src/cache.cpp
)
target_include_directories(olympus_pipeline PUBLIC pipeline/include)
if(OLYMPUS_HAVE_PQXX)
  target_compile_definitions(olympus_pipeline PUBLIC OLYMPUS_WITH_PQXX)
  target_link_libraries(olympus_pipeline PUBLIC libpqxx::pqxx)
endif()

add_library(olympus_agents
  agents/ares/src/ares.cpp
  agents/ares/src/intel_aggregator.cpp
  agents/ares/src/tactical_planner.cpp
  agents/ares/src/threat_model.cpp
  agents/zeus/src/zeus.cpp
  agents/zeus/src/budget_allocator.cpp
  agents/zeus/src/legislative_tracker.cpp
  agents/zeus/src/policy_engine.cpp
  agents/athena/src/athena.cpp
  agents/athena/src/curriculum_optimizer.cpp
  agents/athena/src/research_tracker.cpp
  agents/athena/src/talent_pipeline.cpp
  agents/asclepius/src/asclepius.cpp
  agents/asclepius/src/epidemic_model.cpp
  agents/asclepius/src/outbreak_detector.cpp
  agents/asclepius/src/resource_allocator.cpp
  agents/hestia/src/hestia.cpp
  agents/hestia/src/civic_monitor.cpp
  agents/hestia/src/municipal_planner.cpp
  agents/hestia/src/service_optimizer.cpp
  agents/hermes/src/hermes.cpp
  agents/hermes/src/diplomatic_scorer.cpp
  agents/hermes/src/trade_analyzer.cpp
  agents/hermes/src/treaty_tracker.cpp
  agents/demeter/src/demeter.cpp
  agents/demeter/src/env_impact_scorer.cpp
  agents/demeter/src/resource_monitor.cpp
  agents/demeter/src/supply_chain.cpp
  agents/hephaestus/src/hephaestus.cpp
  agents/hephaestus/src/cyber_sentinel.cpp
  agents/hephaestus/src/infra_monitor.cpp
  agents/hephaestus/src/tech_roadmap.cpp
)
target_include_directories(olympus_agents PUBLIC
  agents/include
  agents/ares/include
  agents/zeus/include
  agents/athena/include
  agents/asclepius/include
  agents/hestia/include
  agents/hermes/include
  agents/demeter/include
  agents/hephaestus/include
  core/include
  pipeline/include
)

add_library(olympus_core
  core/src/config.cpp
  core/src/logger.cpp
  core/src/themis.cpp
  core/src/inference.cpp
  core/src/priority_queue.cpp
  core/src/risk_scorer.cpp
  core/src/decision_ledger.cpp
  core/src/cross_correlator.cpp
)
target_include_directories(olympus_core PUBLIC core/include pipeline/include)

add_library(olympus_simulation
  simulation/src/prometheus.cpp
  simulation/src/monte_carlo.cpp
  simulation/src/cascade_model.cpp
  simulation/src/agent_sim.cpp
  simulation/src/timeline.cpp
  simulation/src/renderer.cpp
)
target_include_directories(olympus_simulation PUBLIC simulation/include core/include pipeline/include)

add_library(olympus_dashboard
  dashboard/src/app.cpp
  dashboard/src/audit_stream.cpp
  dashboard/src/notification_panel.cpp
  dashboard/src/decision_review.cpp
  dashboard/src/sim_viewer.cpp
  dashboard/src/auth_dialog.cpp
  dashboard/src/history_view.cpp
  dashboard/src/db_explorer.cpp
)
target_include_directories(olympus_dashboard PUBLIC dashboard/include core/include simulation/include pipeline/include)
target_link_libraries(olympus_dashboard PUBLIC olympus_pipeline olympus_core olympus_simulation)
target_compile_definitions(olympus_dashboard PUBLIC OLYMPUS_WITH_GTK4)
target_link_libraries(olympus_dashboard PUBLIC PkgConfig::GTK4)
if(OLYMPUS_HAVE_ADWAITA)
  target_compile_definitions(olympus_dashboard PUBLIC OLYMPUS_WITH_ADWAITA)
  target_link_libraries(olympus_dashboard PUBLIC PkgConfig::ADWAITA)
endif()

add_executable(olympus
  core/main.cpp
)

target_link_libraries(olympus
  PRIVATE
    olympus_pipeline
    olympus_agents
    olympus_core
    olympus_dashboard
    olympus_simulation
)

include(CTest)
if(BUILD_TESTING AND OLYMPUS_BUILD_TESTS)
  find_package(GTest QUIET)
  if(GTest_FOUND)
    add_executable(olympus_smoke_test tests/smoke_test.cpp)
    add_executable(olympus_pipeline_test tests/pipeline_test.cpp)
    add_executable(olympus_integration_test tests/integration_latency_test.cpp)

    target_link_libraries(olympus_smoke_test
      PRIVATE
        GTest::gtest_main
        olympus_pipeline
        olympus_agents
        olympus_core
        olympus_simulation
        olympus_dashboard
    )
    target_link_libraries(olympus_pipeline_test
      PRIVATE
        GTest::gtest_main
        olympus_pipeline
        olympus_core
    )
    target_link_libraries(olympus_integration_test
      PRIVATE
        GTest::gtest_main
        olympus_pipeline
        olympus_core
        olympus_simulation
    )
    include(GoogleTest)
    gtest_discover_tests(olympus_smoke_test)
    gtest_discover_tests(olympus_pipeline_test)
    gtest_discover_tests(olympus_integration_test)
  endif()
endif()


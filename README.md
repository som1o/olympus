# OLYMPUS

**THIS PROJECT IS CURRENTLY IN ACTIVE DEVELOPMENT. MOST FEATURES ARE NOT COMPLETE AND MANY WORKFLOWS WILL NOT WORK YET.**

OLYMPUS is a multi-agent, simulation-backed civic intelligence platform designed to ingest signals, normalize and validate data, score risk and urgency, propose decisions, and expose operator-facing review and control through a live dashboard.

The architecture is intentionally modular. Each subsystem is responsible for a specific stage of the pipeline so the system can evolve from a prototype into a robust orchestration platform:

- `pipeline/`: ingestion, stream processing, normalization, validation, caching, and data connector boundaries.
- `core/`: shared runtime primitives including config loading, logging, inference support, risk scoring, priorities, and decision ledger logic.
- `agents/`: domain-specialized agents (Ares, Zeus, Athena, Asclepius, Hestia, Hermes, Demeter, Hephaestus).
- `simulation/`: consequence modeling and forecasting components, including Monte Carlo and timeline simulations.
- `dashboard/`: GTK4-based operator interface for monitoring, auditing, and intervention.
- `tests/`: smoke, pipeline, and integration-level test targets.

## Vision

OLYMPUS aims to become a strategic coordination runtime where policy, operations, and risk are treated as connected systems instead of isolated dashboards. The long-term objective is to support explainable, traceable decision support across defense, governance, education, health, local services, foreign affairs, resources, and technology domains.

## Current Status

This repository is a **prototype in progress**.

- Interfaces and internals are still changing.
- Optional dependencies may be unavailable in some environments.
- Agent behavior and simulation fidelity are incomplete.
- Dashboard paths may compile but still fail at runtime depending on host setup.

Expect frequent changes while core capabilities are stabilized.

## Build Quick Start

```bash
cmake -S . -B build -G Ninja
cmake --build build
./build/olympus
```

## Project Layout

```text
agents/       Domain-focused decision agents
core/         Shared runtime and entrypoint
pipeline/     Data processing pipeline
simulation/   Forecasting and consequence models
dashboard/    Operator UI and controls
tests/        Automated tests
config/       Runtime configuration

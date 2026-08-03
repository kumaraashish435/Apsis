# apps/sim-engine — Product 4 physics depth (C++), if and when justified

Status: **not yet built.** Full product spec: `docs/products/product-4-digital-twin.md`; see also `aspis-simplification-review.md` item #1 for why this isn't Product 1 infrastructure.

## What this folder is for

A separate, high-performance C++ simulation engine — real subsystem physics (ADCS, thermal, power), exposed via gRPC to `apps/api` and (read-only) to `apps/ai-services`.

## The trigger condition — read this before writing any C++

Products 1–3's orbit propagation and pass prediction stay **in-process, in C#**, inside `apps/api/Apsis.Api/Propagation/`. This folder only gets real content when Product 4's subsystem fidelity requirements genuinely can't be met by C# — e.g., attitude dynamics or thermal modeling at a tick rate/precision that demands it — **not** just because a C++ engine was part of the original long-term architecture. If Product 4's ADCS/thermal/power models run fine in C#, leave this folder empty and revisit the decision at Product 6 (advanced physics) instead.

## If and when it is justified

```
src/
├── propagation/       Migrated from apps/api/Apsis.Api/Propagation/Sgp4 + Keplerian, only if perf/reuse justifies the move
├── pass-prediction/    Migrated from Modules/MissionPlanner, same condition
├── attitude/            ADCS — quaternion attitude dynamics, torque model
├── power/                Equivalent-circuit battery model, temperature-dependent capacity
├── thermal/              Multi-node thermal network
└── scheduler/            Multi-rate tick loop
tests/                    Golden-file/scenario tests — same discipline as Apsis.Api.Tests/Propagation
proto/                    gRPC contracts, mirrored in libs/proto/
```

1. Write an ADR documenting the specific trigger (measured fidelity/performance requirement, not a preference) before starting.
2. Validate every new physics module against a published reference test case — the same golden-file discipline `Apsis.Api.Tests/Propagation` already established in Product 1. Nothing here ships without that.
3. Define the gRPC contract in `libs/proto/` first, as the shared source of truth between this engine, `apps/api`, and (read-only) `apps/ai-services`.

## What NOT to do

Don't stand this up preemptively "because Product 5's AI needs a fast simulator" — Product 5 is a read-only consumer of whatever data Products 1-4 already produce; it doesn't require this engine to exist any sooner than Product 4's own fidelity needs demand.

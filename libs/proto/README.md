# libs/proto — shared gRPC/event contracts

Status: **not yet built.** Build trigger: created alongside `apps/sim-engine`, only once that folder gets real content (see its README) — there is no cross-language service boundary to define a contract for before then.

## What this folder will contain, if and when it's needed

`.proto` files defining the gRPC contract between `apps/api` (C#), `apps/sim-engine` (C++), and the read-only consumption path from `apps/ai-services` (Python) — e.g., the `StateVector` shape already defined in `apps/api/Apsis.Api/Propagation/Sgp4/ISgp4Propagator.cs`, mirrored here once propagation (or newer physics modules) actually lives in a separate process.

## How to build it when the time comes

1. Start from the C# interfaces already in `apps/api/Apsis.Api/Propagation/` — translate `ISgp4Propagator`/`IKeplerianPropagator` and their `StateVector`/`OrbitalElements` records into `.proto` messages/services directly, rather than redesigning the contract from scratch.
2. Generate client/server stubs for both C# and C++ (and Python, read-only, for Product 5) from these same `.proto` files — this folder is the single source of truth; no language's generated code should be hand-edited.
3. Version the contract deliberately (proto package versioning) once more than one product depends on it, so `apps/api` and `apps/sim-engine` can be deployed independently without breaking each other.

## What NOT to do

Don't create speculative `.proto` files before `apps/sim-engine` has real content — an unused contract drifts out of sync with nothing to keep it honest.

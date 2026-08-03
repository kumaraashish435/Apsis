# Status: not yet built

This folder belongs to **Product 4 — Digital Twin** (physics depth).

**Contains:** a separate C++ simulation engine — real subsystem physics (ADCS, thermal, power), exposed via gRPC.

**Build trigger:** Introduce only when real subsystem physics genuinely needs a separate, high-performance process and a stable multi-language contract. Products 1-3's orbit propagation and pass prediction stay in-process in C# — see `aspis-simplification-review.md`, item #1. Do not create this folder's contents before Product 4.

See `docs/architecture/folder-structure.md` for the full monorepo map and build order.

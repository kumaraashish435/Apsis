# infra/k8s — Kubernetes, only if Container Apps genuinely isn't enough

Status: **not yet built, and may never need to be for a long time.** See `aspis-simplification-review.md` item #10.

## Default path — use this instead, through Products 1-3 at least

Azure Container Apps (or scaled Azure App Service instances) for hosting every product's backend/frontend. It gives container orchestration and autoscaling without a team of two operating a Kubernetes cluster — secrets, ingress, node upgrades, on-call runbooks, etc. A production-grade k8s setup done properly by a small team learning as they go is realistically a 2-3 month tax, not a one-sprint task, and that cost is easy to underestimate until you're paying it.

## The specific trigger for populating this folder

A measured autoscaling need that Container Apps' scale-to-N genuinely cannot satisfy — e.g., a specific number of concurrent Digital Twin simulation sessions (Product 4+) with a scaling pattern Container Apps can't express. Not "Kubernetes is the more sophisticated choice" — a concrete, observed limit.

## If and when that trigger is hit

1. Write an ADR naming the specific limit that forced the move.
2. Start with a managed control plane (AKS), not self-hosted Kubernetes — don't compound the operational cost.
3. Migrate one service at a time, starting with whichever service actually hit the scaling limit, not a big-bang migration of everything at once.

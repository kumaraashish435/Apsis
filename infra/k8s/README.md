# Status: not yet built

**Contains:** Kubernetes manifests/Helm charts.

**Build trigger:** Default to Azure Container Apps (or scaled App Service instances) first. Only populate this folder once autoscaling needs genuinely exceed what Container Apps can do — see `aspis-simplification-review.md`, item #10. A 2-person team should not be operating a Kubernetes cluster before that point.

See `docs/architecture/folder-structure.md` for the full monorepo map and build order.

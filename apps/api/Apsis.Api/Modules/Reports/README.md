# Modules/Reports — Product 3 backend module

Status: **not yet built.** Build trigger: Product 2 (Mission Planner) is live and validated. Full product spec: `docs/products/product-3-mission-control.md`.

## What this folder will contain

PDF/exportable mission summary generation. `ReportsController`.

## How to build it

1. A simple templated report (mission metadata + a telemetry summary table + the alert list over a time range) rendered to PDF server-side. Any mainstream .NET PDF library is fine here — this is the least architecturally interesting module in the product, deliberately.
2. Generated reports are stored in blob storage (Azure Blob Storage / S3) and referenced by URL from a `Report` row — don't store PDF bytes in Postgres.
3. REST: `GET /api/v1/missions/{id}/reports`, `POST /api/v1/missions/{id}/reports` (generate).

## What NOT to build here

No AI-generated narrative summaries yet — that's Product 5's Telemetry Summaries feature (LLM-based), which can eventually generate the *content* this module still just renders and stores.

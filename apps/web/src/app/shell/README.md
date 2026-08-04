# shell — Product 1, active, shared by every future product

## What this module does

The app-wide layout: top nav bar + `<router-outlet>`. Every product page in ASPIS — today just Orbital Simulator, eventually all five — renders inside this shell. It's the one frontend piece every product touches, so changes here have platform-wide blast radius.

## How it works today

`Shell` (`shell.ts`) holds a `products` array — one entry per product, each with a `label`, `path`, and `active` flag. The template (`shell.html`) renders a real `routerLink` for any product with `active: true` and a disabled, tooltipped span for the rest. Right now only Orbital Simulator is `active: true`; the other four render as visibly-disabled nav items rather than being hidden, so the nav's shape doesn't change every time a new product ships — it just "lights up."

## Process: how to activate a new product here

Two changes, always together:
1. In `../app.routes.ts`, add a `path` + `loadChildren` entry for the new product (mirror the `simulator` entry already there).
2. In `shell.ts`'s `products` array, flip that product's `active` to `true`.

Nothing else about `shell.ts`/`shell.html` should need to change per-product — if a product's activation requires touching the shell beyond this, that's a signal the shell's abstraction has drifted and is worth revisiting.

## What NOT to put here

No product-specific logic or state. This component knows product names, paths, and active/inactive status — nothing about what's inside any product. If you find yourself importing something from `orbital-simulator/` into this folder, that's a boundary violation.

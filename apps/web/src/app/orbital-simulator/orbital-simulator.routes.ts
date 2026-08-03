import { Routes } from '@angular/router';

/**
 * Product 1 — Orbital Simulator routes.
 * Lazy-loaded as a whole from app.routes.ts so later products
 * (Mission Planner, Mission Control, ...) don't pull this in until needed.
 */
export const ORBITAL_SIMULATOR_ROUTES: Routes = [
  { path: '', redirectTo: 'dashboard', pathMatch: 'full' },
  {
    path: 'dashboard',
    loadComponent: () => import('./dashboard/dashboard').then((m) => m.Dashboard),
  },
  {
    path: 'simulations/new',
    loadComponent: () =>
      import('./simulation-editor/simulation-editor').then((m) => m.SimulationEditor),
  },
  {
    path: 'simulations/:id',
    loadComponent: () =>
      import('./simulation-editor/simulation-editor').then((m) => m.SimulationEditor),
  },
  {
    path: 'simulations/:id/view',
    loadComponent: () => import('./viewer-3d/viewer-3d').then((m) => m.Viewer3d),
  },
  {
    path: 'compare',
    loadComponent: () => import('./comparison/comparison').then((m) => m.Comparison),
  },
  {
    path: 'glossary',
    loadComponent: () => import('./glossary/glossary').then((m) => m.Glossary),
  },
];

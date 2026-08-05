import { Routes } from '@angular/router';
import { authGuard } from './auth/auth.guard';

export const routes: Routes = [
  {
    path: 'auth/sign-in',
    loadComponent: () => import('./auth/sign-in/sign-in').then((m) => m.SignIn),
  },
  {
    path: 'auth/sign-up',
    loadComponent: () => import('./auth/sign-up/sign-up').then((m) => m.SignUp),
  },
  {
    // Public, unauthenticated — the "shareable read-only link" feature.
    // No Shell chrome either: this page stands alone since whoever opens it
    // may not even have an account.
    path: 'shared/:token',
    loadComponent: () =>
      import('./orbital-simulator/shared-viewer/shared-viewer').then((m) => m.SharedViewer),
  },
  {
    // The 3D viewer is deliberately NOT nested under Shell — it wants the
    // full viewport for the WebGL canvas and already has its own top bar
    // (see viewer-3d.html), so Shell's product-switcher chrome would just be
    // a second, redundant header stacked above it.
    path: 'simulator/simulations/:id/view',
    canActivate: [authGuard],
    loadComponent: () => import('./orbital-simulator/viewer-3d/viewer-3d').then((m) => m.Viewer3d),
  },
  {
    path: '',
    canActivate: [authGuard],
    loadComponent: () => import('./shell/shell').then((m) => m.Shell),
    children: [
      { path: '', redirectTo: 'simulator', pathMatch: 'full' },
      {
        // Product 1 — the only product with real routes right now.
        // Products 2-5 get their own top-level path + loadChildren entry
        // here once they exist — see docs/architecture/folder-structure.md.
        path: 'simulator',
        loadChildren: () =>
          import('./orbital-simulator/orbital-simulator.routes').then(
            (m) => m.ORBITAL_SIMULATOR_ROUTES,
          ),
      },
    ],
  },
];

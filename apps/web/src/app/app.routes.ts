import { Routes } from '@angular/router';

export const routes: Routes = [
  {
    path: '',
    redirectTo: 'dashboard',
    pathMatch: 'full'
  },
  {
    path: 'dashboard',
    loadComponent: () =>
      import('./sections/dashboard/dashboard')
        .then(m => m.Dashboard)
  },
  {
    path: 'orbit',
    loadComponent: () =>
      import('./sections/orbit/orbit')
        .then(m => m.Orbit)
  },
  {
    path: 'analytics',
    loadComponent: () =>
      import('./sections/analytics/analytics')
        .then(m => m.Analytics)
  },
  {
    path: 'settings',
    loadComponent: () =>
      import('./sections/settings/settings')
        .then(m => m.Settings)
  }
];
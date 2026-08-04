import { Routes } from '@angular/router';
import { Viewer3d } from './orbital-simulator/viewer-3d/viewer-3d';

export const routes: Routes = [
  {
    // path: '',
    // loadComponent: () => import('./shell/shell').then((m) => m.Shell),
    // children: [
    //   { path: '', redirectTo: 'simulator', pathMatch: 'full' },
    //   {
    //     // Product 1 — the only product with real routes right now.
    //     // Products 2-5 get their own top-level path + loadChildren entry
    //     // here once they exist — see docs/architecture/folder-structure.md.
    //     path: 'simulator',
    //     loadChildren: () =>
    //       import('./orbital-simulator/orbital-simulator.routes').then(
    //         (m) => m.ORBITAL_SIMULATOR_ROUTES,
    //       ),
    //   },
    // ],

    
    path: '',
    component: Viewer3d,
  

  },
];

import {  RenderMode, ServerRoute} from '@angular/ssr';


export const serverRoutes: ServerRoute[] = [
   {
    path: '',
    renderMode: RenderMode.Client
  },
  {
    path: 'dashboard',
    renderMode: RenderMode.Client
  },
  {
    path: 'orbit',
    renderMode: RenderMode.Client
  },
  {
    path: 'analytics',
    renderMode: RenderMode.Client
  },
  {
    path: 'settings',
    renderMode: RenderMode.Client
  }
];

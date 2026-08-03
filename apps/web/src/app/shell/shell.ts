import { Component } from '@angular/core';
import { RouterLink, RouterLinkActive, RouterOutlet } from '@angular/router';

/**
 * App-wide layout: top nav + router outlet.
 * Every product (orbital-simulator, mission-planner, ...) is a lazy-loaded
 * child route rendered inside this shell — see app.routes.ts.
 */
@Component({
  selector: 'app-shell',
  imports: [RouterLink, RouterLinkActive, RouterOutlet],
  templateUrl: './shell.html',
  styleUrl: './shell.scss',
})
export class Shell {
  protected readonly products = [
    { label: 'Orbital Simulator', path: '/simulator', active: true },
    { label: 'Mission Planner', path: '/mission-planner', active: false },
    { label: 'Mission Control', path: '/mission-control', active: false },
    { label: 'Digital Twin', path: '/digital-twin', active: false },
    { label: 'AI Assistant', path: '/ai', active: false },
  ];
}

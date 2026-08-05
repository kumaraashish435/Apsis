import { DecimalPipe } from '@angular/common';
import { Component, computed, inject, signal } from '@angular/core';
import { ActivatedRoute, RouterLink } from '@angular/router';
import { EarthView } from '../viewer-3d/earth-view/earth-view';
import { GroundTrack } from '../viewer-3d/ground-track/ground-track';
import { ExportFormat, OrbitalElementsReadout, Simulation, StateVector } from '../simulations/simulation.models';
import { SimulationsService } from '../simulations/simulations.service';

type ViewMode = '3d' | 'ground';

/**
 * The "shareable read-only link" destination — a standalone page (no Shell,
 * no auth guard, no top-bar sign-out) reachable by anyone with the URL. Talks
 * to the API's public `/shared/{token}` routes exclusively — see
 * AuthController's counterpart for why those need no bearer token: a
 * ShareToken is itself the capability, same idea as an unlisted-link share.
 */
@Component({
  selector: 'app-shared-viewer',
  imports: [EarthView, GroundTrack, RouterLink, DecimalPipe],
  templateUrl: './shared-viewer.html',
  styleUrl: './shared-viewer.scss',
})
export class SharedViewer {
  private readonly route = inject(ActivatedRoute);
  private readonly simulations = inject(SimulationsService);

  protected readonly token = this.route.snapshot.paramMap.get('token')!;
  protected readonly viewMode = signal<ViewMode>('3d');
  protected readonly loading = signal(true);
  protected readonly notFound = signal(false);
  protected readonly simulation = signal<Simulation | null>(null);
  protected readonly states = signal<StateVector[]>([]);
  protected readonly elementsAtEpoch = signal<OrbitalElementsReadout | null>(null);

  protected readonly satelliteEntries = computed(() => {
    const sim = this.simulation();
    const points = this.states();
    return points.length >= 2
      ? [{ name: sim?.name ?? 'Satellite', states: points.map((p) => ({ epochSeconds: p.epochSeconds, xKm: p.xKm, yKm: p.yKm, zKm: p.zKm })) }]
      : [];
  });

  constructor() {
    this.simulations.getShared(this.token).subscribe({
      next: (sim) => {
        this.simulation.set(sim);
        this.simulations.propagateShared(this.token).subscribe({
          next: (res) => {
            this.states.set(res.states);
            this.elementsAtEpoch.set(res.elements[0] ?? null);
            this.loading.set(false);
          },
          error: () => this.loading.set(false),
        });
      },
      error: () => {
        this.notFound.set(true);
        this.loading.set(false);
      },
    });
  }

  protected setViewMode(mode: ViewMode): void {
    this.viewMode.set(mode);
  }

  protected exportAs(format: ExportFormat): void {
    const sim = this.simulation();
    if (!sim) {
      return;
    }
    const hint = sim.name.toLowerCase().replace(/[^a-z0-9]+/g, '-').replace(/^-|-$/g, '') || 'simulation';
    this.simulations.exportShared(this.token, format, hint).subscribe();
  }
}

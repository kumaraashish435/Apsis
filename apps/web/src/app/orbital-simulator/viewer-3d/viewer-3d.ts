import { DecimalPipe } from '@angular/common';
import { Component, computed, inject, signal } from '@angular/core';
import { ActivatedRoute, RouterLink } from '@angular/router';
import { forkJoin } from 'rxjs';
import { AuthService } from '../../auth/auth.service';
import { ExportFormat, OrbitalElementsReadout, Simulation, StateVector } from '../simulations/simulation.models';
import { SimulationsService } from '../simulations/simulations.service';
import { EarthView } from './earth-view/earth-view';
import { GroundTrack } from './ground-track/ground-track';
import { TopBar } from '../../components/top-bar/top-bar';

type ViewMode = '3d' | 'ground';

@Component({
  selector: 'app-orbit-viewer',
  imports: [EarthView, GroundTrack, TopBar, RouterLink, DecimalPipe],
  templateUrl: './viewer-3d.html',
  styleUrl: './viewer-3d.scss',
})
export class Viewer3d {
  private readonly route = inject(ActivatedRoute);
  private readonly simulations = inject(SimulationsService);
  protected readonly auth = inject(AuthService);

  protected readonly simulationId = this.route.snapshot.paramMap.get('id')!;
  protected readonly viewMode = signal<ViewMode>('3d');
  protected readonly loading = signal(true);
  protected readonly errorMessage = signal<string | null>(null);
  protected readonly simulation = signal<Simulation | null>(null);
  protected readonly states = signal<StateVector[]>([]);
  protected readonly elementsAtEpoch = signal<OrbitalElementsReadout | null>(null);

  protected readonly satelliteEntries = computed(() => {
    const sim = this.simulation();
    const points = this.states();
    return points.length >= 2 ? [{ name: sim?.name ?? 'Satellite', states: this.toSceneStates(points) }] : [];
  });

  constructor() {
    forkJoin({
      simulation: this.simulations.get(this.simulationId),
      propagation: this.simulations.propagate(this.simulationId),
    }).subscribe({
      next: ({ simulation, propagation }) => {
        this.simulation.set(simulation);
        this.states.set(propagation.states);
        this.elementsAtEpoch.set(propagation.elements[0] ?? null);
        this.loading.set(false);
      },
      error: (err) => {
        this.errorMessage.set(err?.error?.message ?? 'Could not load this simulation.');
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
    this.simulations.export(sim.id, format, this.fileNameHint(sim)).subscribe();
  }

  private fileNameHint(sim: Simulation): string {
    return sim.name.toLowerCase().replace(/[^a-z0-9]+/g, '-').replace(/^-|-$/g, '') || 'simulation';
  }

  /** EarthView's SatelliteStateVector shape is {epochSeconds, xKm, yKm, zKm} — the position subset of the fuller StateVector this page fetches. */
  private toSceneStates(points: StateVector[]) {
    return points.map((p) => ({ epochSeconds: p.epochSeconds, xKm: p.xKm, yKm: p.yKm, zKm: p.zKm }));
  }
}

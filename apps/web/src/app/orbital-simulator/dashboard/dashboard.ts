import { DatePipe } from '@angular/common';
import { Component, OnInit, inject, signal } from '@angular/core';
import { RouterLink } from '@angular/router';
import { Simulation } from '../simulations/simulation.models';
import { SimulationsService } from '../simulations/simulations.service';

@Component({
  selector: 'app-dashboard',
  imports: [RouterLink, DatePipe],
  templateUrl: './dashboard.html',
  styleUrl: './dashboard.scss',
})
export class Dashboard implements OnInit {
  private readonly simulations = inject(SimulationsService);

  protected readonly items = signal<Simulation[]>([]);
  protected readonly loading = signal(true);
  protected readonly errorMessage = signal<string | null>(null);
  protected readonly copiedId = signal<string | null>(null);

  ngOnInit(): void {
    this.load();
  }

  private load(): void {
    this.loading.set(true);
    this.simulations.list().subscribe({
      next: (items) => {
        this.items.set(items);
        this.loading.set(false);
      },
      error: () => {
        this.errorMessage.set('Could not load your simulations.');
        this.loading.set(false);
      },
    });
  }

  protected remove(sim: Simulation): void {
    if (!confirm(`Delete "${sim.name}"? This can't be undone.`)) {
      return;
    }
    this.simulations.remove(sim.id).subscribe(() => {
      this.items.update((items) => items.filter((i) => i.id !== sim.id));
    });
  }

  protected toggleShare(sim: Simulation): void {
    const request = sim.isPublic ? this.simulations.unshare(sim.id) : this.simulations.share(sim.id);
    request.subscribe((res) => {
      this.items.update((items) =>
        items.map((i) => (i.id === sim.id ? { ...i, isPublic: res.isPublic, shareToken: res.shareToken } : i)),
      );
      if (res.isPublic) {
        this.copyShareLink({ ...sim, isPublic: true, shareToken: res.shareToken });
      }
    });
  }

  protected copyShareLink(sim: Simulation): void {
    if (!sim.shareToken) {
      return;
    }
    const url = `${location.origin}/shared/${sim.shareToken}`;
    navigator.clipboard?.writeText(url);
    this.copiedId.set(sim.id);
    setTimeout(() => this.copiedId.set(null), 2000);
  }

  protected inputSummary(sim: Simulation): string {
    return sim.inputType === 'Tle' ? 'TLE' : `Elements (a=${sim.semiMajorAxisKm?.toFixed(0)} km)`;
  }
}

import { DecimalPipe } from '@angular/common';
import { Component, OnInit, computed, inject, signal } from '@angular/core';
import { ActivatedRoute, RouterLink } from '@angular/router';
import { DivergencePoint, Simulation } from '../simulations/simulation.models';
import { SimulationsService } from '../simulations/simulations.service';

const CHART_WIDTH = 640;
const CHART_HEIGHT = 280;
const MARGIN = { top: 16, right: 16, bottom: 28, left: 52 };

@Component({
  selector: 'app-comparison',
  imports: [RouterLink, DecimalPipe],
  templateUrl: './comparison.html',
  styleUrl: './comparison.scss',
})
export class Comparison implements OnInit {
  private readonly route = inject(ActivatedRoute);
  private readonly simulations = inject(SimulationsService);

  protected readonly tleSimulations = signal<Simulation[]>([]);
  protected readonly selectedId = signal<string | null>(null);
  protected readonly divergence = signal<DivergencePoint[] | null>(null);
  protected readonly loading = signal(false);
  protected readonly errorMessage = signal<string | null>(null);
  protected readonly hoverIndex = signal<number | null>(null);

  protected readonly chartWidth = CHART_WIDTH;
  protected readonly chartHeight = CHART_HEIGHT;
  protected readonly plotWidth = CHART_WIDTH - MARGIN.left - MARGIN.right;
  protected readonly plotHeight = CHART_HEIGHT - MARGIN.top - MARGIN.bottom;
  protected readonly margin = MARGIN;

  private readonly maxDistance = computed(() => {
    const points = this.divergence();
    if (!points || points.length === 0) {
      return 1;
    }
    return Math.max(...points.map((p) => p.distanceKm), 1e-9);
  });

  private readonly maxTime = computed(() => {
    const points = this.divergence();
    return points && points.length > 0 ? points[points.length - 1].epochSeconds : 1;
  });

  protected readonly linePath = computed(() => {
    const points = this.divergence();
    if (!points || points.length === 0) {
      return '';
    }
    return points
      .map((p, i) => `${i === 0 ? 'M' : 'L'} ${this.xFor(p.epochSeconds)} ${this.yFor(p.distanceKm)}`)
      .join(' ');
  });

  protected readonly yTicks = computed(() => {
    const max = this.maxDistance();
    return [0, 0.25, 0.5, 0.75, 1].map((f) => f * max);
  });

  protected readonly xTicks = computed(() => {
    const max = this.maxTime();
    return [0, 0.25, 0.5, 0.75, 1].map((f) => f * max);
  });

  protected readonly hoverPoint = computed(() => {
    const idx = this.hoverIndex();
    const points = this.divergence();
    return idx !== null && points ? points[idx] : null;
  });

  ngOnInit(): void {
    this.simulations.list().subscribe((sims) => {
      const tleSims = sims.filter((s) => s.inputType === 'Tle');
      this.tleSimulations.set(tleSims);

      const queryId = this.route.snapshot.queryParamMap.get('id');
      const initial = queryId && tleSims.some((s) => s.id === queryId) ? queryId : (tleSims[0]?.id ?? null);
      if (initial) {
        this.select(initial);
      }
    });
  }

  protected select(id: string): void {
    this.selectedId.set(id);
    this.loading.set(true);
    this.errorMessage.set(null);
    this.divergence.set(null);

    this.simulations.compare(id).subscribe({
      next: (res) => {
        this.divergence.set(res.divergence);
        this.loading.set(false);
      },
      error: (err) => {
        this.errorMessage.set(err?.error?.message ?? 'Could not compare this simulation.');
        this.loading.set(false);
      },
    });
  }

  protected onHover(event: MouseEvent, svg: Element): void {
    const points = this.divergence();
    if (!points || points.length === 0) {
      return;
    }
    const rect = svg.getBoundingClientRect();
    const scaleX = CHART_WIDTH / rect.width;
    const xInChart = (event.clientX - rect.left) * scaleX;
    const t = ((xInChart - MARGIN.left) / this.plotWidth) * this.maxTime();

    let nearest = 0;
    let bestDiff = Infinity;
    points.forEach((p, i) => {
      const diff = Math.abs(p.epochSeconds - t);
      if (diff < bestDiff) {
        bestDiff = diff;
        nearest = i;
      }
    });
    this.hoverIndex.set(nearest);
  }

  protected clearHover(): void {
    this.hoverIndex.set(null);
  }

  protected xFor(epochSeconds: number): number {
    return MARGIN.left + (epochSeconds / this.maxTime()) * this.plotWidth;
  }

  protected yFor(distanceKm: number): number {
    return MARGIN.top + this.plotHeight - (distanceKm / this.maxDistance()) * this.plotHeight;
  }

  protected formatMinutes(seconds: number): string {
    return `${(seconds / 60).toFixed(0)} min`;
  }
}

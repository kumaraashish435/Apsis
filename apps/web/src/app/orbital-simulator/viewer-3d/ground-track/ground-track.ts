import { Component, computed, input } from '@angular/core';
import { StateVector } from '../../simulations/simulation.models';

/**
 * The 2D ground-track view: the path traced on the ground by the
 * sub-satellite point over the whole propagated window. Latitude/longitude
 * come straight from the backend (GeodeticConverter.cs) — this component's
 * only job is projecting them onto an equirectangular grid and breaking the
 * polyline where it crosses the antimeridian, so a real eastbound track
 * doesn't draw as a spurious line clear across the map. Graticule only, no
 * coastline/continent geometry — see ../../README.md for why.
 */
@Component({
  selector: 'app-ground-track',
  imports: [],
  templateUrl: './ground-track.html',
  styleUrl: './ground-track.scss',
})
export class GroundTrack {
  readonly states = input<StateVector[]>([]);

  protected readonly pathSegments = computed(() => this.buildSegments(this.states()));
  protected readonly markerPosition = computed(() => {
    const points = this.states();
    if (points.length === 0) {
      return null;
    }
    const last = points[points.length - 1];
    return { x: last.lonDeg + 180, y: 90 - last.latDeg };
  });

  private buildSegments(states: StateVector[]): string[] {
    if (states.length < 2) {
      return [];
    }

    const segments: string[] = [];
    let current = `M ${states[0].lonDeg + 180} ${90 - states[0].latDeg}`;

    for (let i = 1; i < states.length; i++) {
      const prev = states[i - 1];
      const curr = states[i];
      const x = curr.lonDeg + 180;
      const y = 90 - curr.latDeg;

      if (Math.abs(curr.lonDeg - prev.lonDeg) > 180) {
        // Crossed the antimeridian — end this segment and start a new one
        // rather than drawing a line straight across the map.
        segments.push(current);
        current = `M ${x} ${y}`;
      } else {
        current += ` L ${x} ${y}`;
      }
    }

    segments.push(current);
    return segments;
  }
}

import * as THREE from 'three';
import { kmToScene } from '../utils/math';

/**
 * Matches the backend's StateVector shape — see
 * apps/api/Apsis.Api/Propagation/Sgp4/ISgp4Propagator.cs. epochSeconds is
 * seconds since the propagation's start time, not a wall-clock timestamp.
 */
export interface SatelliteStateVector {
  epochSeconds: number;
  xKm: number;
  yKm: number;
  zKm: number;
}

export class Orbit {
  private readonly scenePoints: THREE.Vector3[];

  constructor(private readonly states: SatelliteStateVector[]) {
    if (states.length < 2) {
      throw new Error('Orbit requires at least 2 state vectors to be meaningful.');
    }
    this.scenePoints = states.map((s) => kmToScene(s.xKm, s.yKm, s.zKm));
  }

  get points(): readonly THREE.Vector3[] {
    return this.scenePoints;
  }

  get periodSeconds(): number {
    return this.states[this.states.length - 1].epochSeconds - this.states[0].epochSeconds;
  }

  /**
   * Linear interpolation between the two nearest samples, looping over the
   * orbit's own period. Good enough for visualization at typical propagation
   * sample spacing — not a substitute for re-querying the backend if higher
   * precision is ever needed client-side.
   */
  positionAt(simSeconds: number): THREE.Vector3 {
    const t = this.wrapToOrbit(simSeconds);

    let i = 0;
    while (i < this.states.length - 2 && this.states[i + 1].epochSeconds < t) i++;

    const a = this.states[i];
    const b = this.states[i + 1];
    const span = b.epochSeconds - a.epochSeconds;
    const alpha = span > 0 ? (t - a.epochSeconds) / span : 0;

    return this.scenePoints[i].clone().lerp(this.scenePoints[i + 1], alpha);
  }

  private wrapToOrbit(simSeconds: number): number {
    const start = this.states[0].epochSeconds;
    const period = this.periodSeconds;
    if (period <= 0) return start;
    return start + (((simSeconds - start) % period) + period) % period;
  }
}

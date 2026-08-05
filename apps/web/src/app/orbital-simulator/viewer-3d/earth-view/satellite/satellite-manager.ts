import * as THREE from 'three';
import { Satellite } from './satellite';
import { SatelliteStateVector } from './orbit';
import { disposeObject3D, generateId } from '../utils/helpers';

/** A named satellite's propagated track — the shape EarthView's `satellites` input expects. */
export interface SatelliteEntry {
  name: string;
  states: SatelliteStateVector[];
}

export class SatelliteManager {
  private readonly satellites = new Map<string, Satellite>();

  constructor(private readonly parent: THREE.Object3D) {}

  /**
   * Called once real propagated state vectors are available for a satellite
   * — i.e. after the SGP4/SDP4 backend call resolves. See
   * apps/api/Apsis.Api/Modules/Simulations/SimulationsController.cs's
   * Propagate endpoint for where these values are meant to come from; until
   * that's implemented, earth-view.ts seeds this with demo data (see
   * utils/helpers.ts's generateDemoCircularOrbit).
   */
  addSatellite(name: string, states: SatelliteStateVector[]): string {
    const id = generateId('sat');
    const satellite = new Satellite(id, name, states);
    this.satellites.set(id, satellite);
    this.parent.add(satellite.group);
    return id;
  }

  removeSatellite(id: string): void {
    const satellite = this.satellites.get(id);
    if (!satellite) return;
    this.parent.remove(satellite.group);
    disposeObject3D(satellite.group);
    this.satellites.delete(id);
  }

  update(simSeconds: number): void {
    for (const satellite of this.satellites.values()) satellite.update(simSeconds);
  }

  getGroups(): THREE.Object3D[] {
    return [...this.satellites.values()].map((s) => s.group);
  }

  get count(): number {
    return this.satellites.size;
  }

  dispose(): void {
    for (const id of [...this.satellites.keys()]) this.removeSatellite(id);
  }
}

import * as THREE from 'three';
import { Orbit, SatelliteStateVector } from './orbit';
import { createOrbitLine } from './orbit-renderer';

export class Satellite {
  readonly group = new THREE.Group();
  readonly orbit: Orbit;

  private readonly marker: THREE.Mesh;

  constructor(
    readonly id: string,
    readonly name: string,
    states: SatelliteStateVector[],
    markerColor = 0xffcc00,
  ) {
    this.orbit = new Orbit(states);

    const markerGeometry = new THREE.SphereGeometry(0.012, 12, 12);
    const markerMaterial = new THREE.MeshBasicMaterial({ color: markerColor });
    this.marker = new THREE.Mesh(markerGeometry, markerMaterial);
    this.marker.name = `satellite-marker:${id}`;

    this.group.add(createOrbitLine(this.orbit));
    this.group.add(this.marker);
  }

  update(simSeconds: number): void {
    this.marker.position.copy(this.orbit.positionAt(simSeconds));
  }
}

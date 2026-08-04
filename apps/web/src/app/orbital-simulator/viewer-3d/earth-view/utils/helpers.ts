import * as THREE from 'three';
import { EARTH_RADIUS_KM } from './constants';
import { SatelliteStateVector } from '../satellite/orbit';

/** Recursively disposes geometries/materials/textures under an Object3D — call
 *  on teardown so removed/replaced meshes don't leak GPU memory. */
export function disposeObject3D(root: THREE.Object3D): void {
  root.traverse((obj) => {
    const mesh = obj as THREE.Mesh;
    if (mesh.geometry) mesh.geometry.dispose();

    const material = mesh.material;
    if (Array.isArray(material)) {
      material.forEach(disposeMaterial);
    } else if (material) {
      disposeMaterial(material);
    }
  });
}

function disposeMaterial(material: THREE.Material): void {
  for (const key of Object.keys(material)) {
    const value = (material as unknown as Record<string, unknown>)[key];
    if (value instanceof THREE.Texture) value.dispose();
  }
  material.dispose();
}

let idCounter = 0;
export function generateId(prefix: string): string {
  idCounter += 1;
  return `${prefix}-${idCounter}`;
}

/**
 * DEMO DATA ONLY. Generates a simple circular-orbit state vector series so
 * the orbit-animation pipeline (SatelliteManager -> Orbit -> orbit line +
 * moving marker -> camera auto-fit) can be seen working end to end before
 * Propagation/Sgp4 is implemented on the backend.
 *
 * Replace calls to this with real data from
 * GET /api/v1/simulations/{id}/propagate (Modules/Simulations,
 * Propagation/Sgp4) once that endpoint returns real StateVector arrays —
 * the shape is deliberately identical (epochSeconds/xKm/yKm/zKm) so swapping
 * the data source requires no change to anything downstream of it.
 */
export function generateDemoCircularOrbit(
  altitudeKm: number,
  inclinationDeg: number,
  periodSeconds: number,
  sampleCount = 180,
): SatelliteStateVector[] {
  const radiusKm = EARTH_RADIUS_KM + altitudeKm;
  const inclinationRad = (inclinationDeg * Math.PI) / 180;

  const states: SatelliteStateVector[] = [];
  for (let i = 0; i <= sampleCount; i++) {
    const angle = (i / sampleCount) * Math.PI * 2;
    const epochSeconds = (i / sampleCount) * periodSeconds;

    const xOrbitPlane = radiusKm * Math.cos(angle);
    const yOrbitPlane = radiusKm * Math.sin(angle);

    states.push({
      epochSeconds,
      xKm: xOrbitPlane,
      yKm: yOrbitPlane * Math.cos(inclinationRad),
      zKm: yOrbitPlane * Math.sin(inclinationRad),
    });
  }
  return states;
}

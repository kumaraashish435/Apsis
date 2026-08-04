import * as THREE from 'three';
import { Orbit } from './orbit';

export function createOrbitLine(orbit: Orbit, color = 0x4fc3f7): THREE.Line {
  const geometry = new THREE.BufferGeometry().setFromPoints([...orbit.points]);
  const material = new THREE.LineBasicMaterial({ color, transparent: true, opacity: 0.6 });
  return new THREE.Line(geometry, material);
}

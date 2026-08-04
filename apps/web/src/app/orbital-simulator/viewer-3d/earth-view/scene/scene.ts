import * as THREE from 'three';
import { KM_TO_SCENE, MOON_ORBIT_PERIOD_SECONDS, MOON_ORBIT_RADIUS_KM, MOON_RADIUS_KM } from '../utils/constants';

export function createScene(): THREE.Scene {
  const scene = new THREE.Scene();
  scene.add(createStarfield());
  return scene;
}

function createStarfield(count = 3000, radius = 400): THREE.Points {
  const positions = new Float32Array(count * 3);
  for (let i = 0; i < count; i++) {
    const point = new THREE.Vector3().randomDirection().multiplyScalar(radius);
    positions.set([point.x, point.y, point.z], i * 3);
  }

  const geometry = new THREE.BufferGeometry();
  geometry.setAttribute('position', new THREE.BufferAttribute(positions, 3));

  const material = new THREE.PointsMaterial({ color: 0xffffff, size: 0.4, sizeAttenuation: true });
  return new THREE.Points(geometry, material);
}

/**
 * Simplified circular Moon — not a real ephemeris. It exists mainly to give
 * the camera auto-fit logic (see camera/camera.ts) a second real body to
 * frame alongside Earth and satellites. Deliberately excluded from the
 * default auto-fit target in earth-view.ts: at its true-to-scale distance
 * (~60 scene units) including it would zoom the camera out so far that
 * near-Earth satellite orbits become indistinguishable from Earth itself.
 */
export function createMoon(): THREE.Mesh {
  const radius = MOON_RADIUS_KM * KM_TO_SCENE;
  const geometry = new THREE.SphereGeometry(radius, 32, 32);
  const material = new THREE.MeshStandardMaterial({ color: 0xaaaaaa, roughness: 1 });
  const moon = new THREE.Mesh(geometry, material);
  moon.position.set(MOON_ORBIT_RADIUS_KM * KM_TO_SCENE, 0, 0);
  return moon;
}

export function updateMoon(moon: THREE.Mesh, simSeconds: number): void {
  const angle = (simSeconds / MOON_ORBIT_PERIOD_SECONDS) * Math.PI * 2;
  const distance = MOON_ORBIT_RADIUS_KM * KM_TO_SCENE;
  moon.position.set(Math.cos(angle) * distance, 0, Math.sin(angle) * distance);
}

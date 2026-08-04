import * as THREE from 'three';
import { SCENE_EARTH_RADIUS } from '../utils/constants';

/** Simple additive-glow shell — not physically based, just a cheap, standard
 *  Three.js technique for an atmosphere-like halo around a planet. */
export function createAtmosphere(): THREE.Mesh {
  const geometry = new THREE.SphereGeometry(SCENE_EARTH_RADIUS * 1.02, 64, 64);
  const material = new THREE.MeshBasicMaterial({
    color: 0x3a8bd6,
    transparent: true,
    opacity: 0.15,
    side: THREE.BackSide,
    blending: THREE.AdditiveBlending,
    depthWrite: false,
  });
  return new THREE.Mesh(geometry, material);
}

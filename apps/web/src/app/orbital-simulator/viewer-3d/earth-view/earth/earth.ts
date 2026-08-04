import * as THREE from 'three';
import { SCENE_EARTH_RADIUS } from '../utils/constants';
import { loadColorTexture } from './textures';

export function createEarth(): THREE.Mesh {
  const geometry = new THREE.SphereGeometry(SCENE_EARTH_RADIUS, 64, 64);
  const material = new THREE.MeshPhongMaterial({ map: loadColorTexture('earth_day.jpg') });
  return new THREE.Mesh(geometry, material);
}

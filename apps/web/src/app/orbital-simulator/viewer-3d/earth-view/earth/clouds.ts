import * as THREE from 'three';
import { SCENE_EARTH_RADIUS } from '../utils/constants';
import { loadDataTexture } from './textures';

/**
 * Optional cloud layer. Stays invisible until public/textures/earth_clouds.png
 * (an alpha-mapped texture) actually exists and loads — that way a missing
 * texture degrades to "no clouds," not a broken opaque sphere covering Earth.
 */
export function createClouds(): THREE.Mesh {
  const geometry = new THREE.SphereGeometry(SCENE_EARTH_RADIUS * 1.01, 64, 64);
  const material = new THREE.MeshLambertMaterial({
    transparent: true,
    opacity: 0.6,
    depthWrite: false,
  });

  const clouds = new THREE.Mesh(geometry, material);
  clouds.visible = false;

  material.alphaMap = loadDataTexture('earth_clouds.png', () => {
    clouds.visible = true;
  });

  return clouds;
}

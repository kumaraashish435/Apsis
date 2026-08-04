import * as THREE from 'three';

export interface SceneLights {
  ambient: THREE.AmbientLight;
  sun: THREE.DirectionalLight;
}

export function createLights(): SceneLights {
  const ambient = new THREE.AmbientLight(0xffffff, 0.3);

  const sun = new THREE.DirectionalLight(0xffffff, 3);
  sun.position.set(5, 3, 5);

  return { ambient, sun };
}

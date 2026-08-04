import * as THREE from 'three';
import { OrbitControls } from 'three-stdlib';

export function createOrbitControls(
  camera: THREE.PerspectiveCamera,
  domElement: HTMLElement,
): OrbitControls {
  const controls = new OrbitControls(camera, domElement);
  controls.enableDamping = true;
  controls.dampingFactor = 0.05;
  controls.enablePan = false;

  controls.minDistance = 2.5; // scene units
  controls.maxDistance = 10; // scene units


  
  controls.target.set(0, 0, 0);
  return controls;
}

import * as THREE from 'three';
import { KM_TO_SCENE } from './constants';

export function kmToScene(xKm: number, yKm: number, zKm: number): THREE.Vector3 {
  return new THREE.Vector3(xKm * KM_TO_SCENE, yKm * KM_TO_SCENE, zKm * KM_TO_SCENE);
}

export function clamp(value: number, min: number, max: number): number {
  return Math.min(Math.max(value, min), max);
}

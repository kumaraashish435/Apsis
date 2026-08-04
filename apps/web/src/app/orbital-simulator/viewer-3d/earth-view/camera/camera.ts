import * as THREE from 'three';
import type { OrbitControls } from 'three-stdlib';

export function createCamera(aspect: number): THREE.PerspectiveCamera {
  const camera = new THREE.PerspectiveCamera(60, aspect, 0.01, 2000);
  camera.position.set(0, 0, 4);
  return camera;
}

/**
 * Constrains the camera/controls so zooming out can never go further than
 * "everything currently tracked fits in view" — this is what stops the user
 * from scrolling out into empty space. Call this whenever the set of tracked
 * objects changes (a satellite is added/removed).
 *
 * minDistanceFloor is deliberately a separate input from the bounding
 * sphere, not derived from it: a satellite's orbit can be much wider than
 * Earth itself, which inflates sphere.radius and would otherwise push the
 * near-zoom limit inside Earth's own surface. Pass Earth's actual scene
 * radius (see earth-view.ts's refitCamera) so the camera can never clip
 * through the mesh regardless of how far out the tracked satellites are.
 */
// export function fitCameraToBoundingSphere(
//   camera: THREE.PerspectiveCamera,
//   controls: OrbitControls,
//   sphere: THREE.Sphere,
//   minDistanceFloor: number,
//   padding = 1.4,
// ): void {
//   const verticalFovRad = THREE.MathUtils.degToRad(camera.fov);
//   const horizontalFovRad = 2 * Math.atan(Math.tan(verticalFovRad / 2) * camera.aspect);
//   const limitingFovRad = Math.min(verticalFovRad, horizontalFovRad);

//   const fitDistance = (sphere.radius * padding) / Math.sin(limitingFovRad / 2);

//   controls.target.copy(sphere.center);
//   controls.maxDistance = Math.max(fitDistance, minDistanceFloor * 1.5);
//   controls.minDistance = minDistanceFloor;

//   const currentDistance = camera.position.distanceTo(controls.target);
//   if (currentDistance > controls.maxDistance || currentDistance < controls.minDistance) {
//     const direction = camera.position.clone().sub(controls.target);
//     if (direction.lengthSq() === 0) direction.set(0, 0.3, 1);
//     direction.normalize();
//     const clampedDistance = THREE.MathUtils.clamp(
//       currentDistance,
//       controls.minDistance,
//       controls.maxDistance,
//     );
//     camera.position.copy(sphere.center).add(direction.multiplyScalar(clampedDistance));
//   }

//   controls.update();
// }

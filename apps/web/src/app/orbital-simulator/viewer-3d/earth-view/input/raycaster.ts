import * as THREE from 'three';

export function pickObject(
  camera: THREE.Camera,
  ndc: THREE.Vector2,
  candidates: THREE.Object3D[],
): THREE.Object3D | null {
  const raycaster = new THREE.Raycaster();
  raycaster.setFromCamera(ndc, camera);
  const hits = raycaster.intersectObjects(candidates, true);
  return hits.length > 0 ? hits[0].object : null;
}

import * as THREE from 'three';
import type { OrbitControls } from 'three-stdlib';

/**
 * OrbitControls listens for pointerdown/wheel directly on the canvas, so by
 * default drag-to-orbit and scroll-to-zoom fire no matter where on the
 * canvas the pointer is — including empty space (starfield/background).
 *
 * This gates it to "only when the pointer is over an interactive object"
 * (Earth, satellites, ...) by raycasting on pointerdown/wheel and flipping
 * `controls.enabled` accordingly *before* OrbitControls' own listeners run.
 * That ordering is why these listeners are registered on the canvas's
 * PARENT with `capture: true`: capture-phase listeners on an ancestor always
 * run before target-phase listeners on the descendant itself, regardless of
 * registration order — see OrbitControls.js's onPointerDown/onMouseWheel,
 * which both bail out early when `scope.enabled === false`.
 */
export class OrbitControlsGate {
  private readonly raycaster = new THREE.Raycaster();
  private readonly ndc = new THREE.Vector2();
  private readonly parent: HTMLElement;

  constructor(
    private readonly controls: OrbitControls,
    private readonly camera: THREE.Camera,
    private readonly domElement: HTMLElement,
    private readonly pickableObjects: () => THREE.Object3D[],
  ) {
    this.parent = domElement.parentElement ?? domElement;
    this.parent.addEventListener('pointerdown', this.handlePointerDown, { capture: true });
    this.parent.addEventListener('wheel', this.handleWheel, { capture: true });
  }

  private hitsObject(clientX: number, clientY: number): boolean {
    const rect = this.domElement.getBoundingClientRect();
    this.ndc.x = ((clientX - rect.left) / rect.width) * 2 - 1;
    this.ndc.y = -((clientY - rect.top) / rect.height) * 2 + 1;
    this.raycaster.setFromCamera(this.ndc, this.camera);
    return this.raycaster.intersectObjects(this.pickableObjects(), true).length > 0;
  }

  private handlePointerDown = (event: PointerEvent): void => {
    this.controls.enabled = this.hitsObject(event.clientX, event.clientY);
  };

  private handleWheel = (event: WheelEvent): void => {
    this.controls.enabled = this.hitsObject(event.clientX, event.clientY);
  };

  dispose(): void {
    this.parent.removeEventListener('pointerdown', this.handlePointerDown, { capture: true });
    this.parent.removeEventListener('wheel', this.handleWheel, { capture: true });
  }
}

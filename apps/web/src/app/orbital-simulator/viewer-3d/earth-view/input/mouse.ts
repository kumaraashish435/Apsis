import * as THREE from 'three';

export class MouseTracker {
  readonly ndc = new THREE.Vector2();

  constructor(private readonly domElement: HTMLElement) {
    domElement.addEventListener('pointermove', this.handlePointerMove);
  }

  private handlePointerMove = (event: PointerEvent): void => {
    const rect = this.domElement.getBoundingClientRect();
    this.ndc.x = ((event.clientX - rect.left) / rect.width) * 2 - 1;
    this.ndc.y = -((event.clientY - rect.top) / rect.height) * 2 + 1;
  };

  dispose(): void {
    this.domElement.removeEventListener('pointermove', this.handlePointerMove);
  }
}

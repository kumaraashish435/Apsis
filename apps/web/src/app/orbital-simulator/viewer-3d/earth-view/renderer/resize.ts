import * as THREE from 'three';

export class ResizeHandler {
  private readonly observer: ResizeObserver;

  constructor(
    private readonly container: HTMLElement,
    private readonly camera: THREE.PerspectiveCamera,
    private readonly renderer: THREE.WebGLRenderer,
    private readonly onResize?: () => void,
  ) {
    this.observer = new ResizeObserver(() => this.handleResize());
    this.observer.observe(container);
  }

  private handleResize(): void {
    const { clientWidth, clientHeight } = this.container;
    if (!clientWidth || !clientHeight) return;

    this.camera.aspect = clientWidth / clientHeight;
    this.camera.updateProjectionMatrix();
    this.renderer.setSize(clientWidth, clientHeight);
    this.onResize?.();
  }

  dispose(): void {
    this.observer.disconnect();
  }
}

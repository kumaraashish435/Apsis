import * as THREE from 'three';
import { MouseTracker } from './mouse';
import { KeyboardTracker } from './keyboard';
import { pickObject } from './raycaster';

/** Wires mouse/keyboard/raycaster together and exposes a click-on-object
 *  event — e.g. clicking a satellite marker to select it. Picking is lazy:
 *  pickableObjects() is called at click time, not cached, so it always sees
 *  the current set of satellites even as they're added/removed. */
export class InputManager {
  readonly mouse: MouseTracker;
  readonly keyboard = new KeyboardTracker();

  private clickHandlers: Array<(object: THREE.Object3D | null) => void> = [];

  constructor(
    private readonly domElement: HTMLElement,
    private readonly camera: THREE.Camera,
    private readonly pickableObjects: () => THREE.Object3D[],
  ) {
    this.mouse = new MouseTracker(domElement);
    domElement.addEventListener('click', this.handleClick);
  }

  onObjectClick(handler: (object: THREE.Object3D | null) => void): void {
    this.clickHandlers.push(handler);
  }

  private handleClick = (): void => {
    const hit = pickObject(this.camera, this.mouse.ndc, this.pickableObjects());
    this.clickHandlers.forEach((handler) => handler(hit));
  };

  dispose(): void {
    this.domElement.removeEventListener('click', this.handleClick);
    this.mouse.dispose();
    this.keyboard.dispose();
    this.clickHandlers = [];
  }
}

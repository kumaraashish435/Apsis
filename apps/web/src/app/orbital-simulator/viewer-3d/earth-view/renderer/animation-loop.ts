/** Owns the requestAnimationFrame lifecycle so components don't hand-roll
 *  their own frame id bookkeeping — start()/stop() are the whole contract,
 *  and stop() is what earth-view.ts calls from ngOnDestroy. */
export class AnimationLoop {
  private frameId = 0;
  private running = false;
  private lastTime = 0;

  constructor(private readonly onFrame: (deltaSeconds: number) => void) {}

  start(): void {
    if (this.running) return;
    this.running = true;
    this.lastTime = performance.now();
    this.tick();
  }

  stop(): void {
    this.running = false;
    cancelAnimationFrame(this.frameId);
  }

  private tick = (): void => {
    if (!this.running) return;
    const now = performance.now();
    const deltaSeconds = (now - this.lastTime) / 1000;
    this.lastTime = now;

    this.onFrame(deltaSeconds);
    this.frameId = requestAnimationFrame(this.tick);
  };
}

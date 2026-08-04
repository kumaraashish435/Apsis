export class SimClock {
  private elapsedSeconds = 0;
  private paused = false;

  tick(deltaSeconds: number): number {
    if (!this.paused) this.elapsedSeconds += deltaSeconds;
    return this.elapsedSeconds;
  }

  get elapsed(): number {
    return this.elapsedSeconds;
  }

  get isPaused(): boolean {
    return this.paused;
  }

  pause(): void {
    this.paused = true;
  }

  resume(): void {
    this.paused = false;
  }

  reset(): void {
    this.elapsedSeconds = 0;
  }
}

import { SimClock } from './clock';
import { toSimSeconds } from './time';

/**
 * Owns simulated mission time. Real elapsed seconds (from the animation
 * loop) are scaled by timeScale before advancing the sim clock — e.g.
 * timeScale=60 means one real second plays back one simulated minute.
 * Earth's rotation and every satellite's position are driven from this
 * clock, not from wall-clock time directly, so pausing/speeding up the
 * simulation moves everything in the scene together.
 */
export class Simulation {
  private readonly clock = new SimClock();

  constructor(public timeScale = 60) {}

  tick(realDeltaSeconds: number): number {
    const simDelta = toSimSeconds(realDeltaSeconds, this.timeScale);
    return this.clock.tick(simDelta);
  }

  get elapsedSeconds(): number {
    return this.clock.elapsed;
  }

  get isPaused(): boolean {
    return this.clock.isPaused;
  }

  pause(): void {
    this.clock.pause();
  }

  resume(): void {
    this.clock.resume();
  }

  toggle(): void {
    if (this.clock.isPaused) this.resume();
    else this.pause();
  }

  reset(): void {
    this.clock.reset();
  }
}

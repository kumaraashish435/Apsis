/** Real seconds elapsed -> simulated mission seconds, at a configurable speed multiplier. */
export function toSimSeconds(realDeltaSeconds: number, timeScale: number): number {
  return realDeltaSeconds * timeScale;
}

export function formatMissionTime(simSeconds: number): string {
  const hours = Math.floor(simSeconds / 3600);
  const minutes = Math.floor((simSeconds % 3600) / 60);
  const seconds = Math.floor(simSeconds % 60);
  return [hours, minutes, seconds].map((n) => n.toString().padStart(2, '0')).join(':');
}

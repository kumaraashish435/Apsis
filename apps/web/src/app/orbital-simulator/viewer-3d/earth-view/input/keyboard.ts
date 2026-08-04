export class KeyboardTracker {
  private readonly handlers = new Map<string, () => void>();

  constructor() {
    window.addEventListener('keydown', this.handleKeyDown);
  }

  on(key: string, handler: () => void): void {
    this.handlers.set(key.toLowerCase(), handler);
  }

  private handleKeyDown = (event: KeyboardEvent): void => {
    this.handlers.get(event.key.toLowerCase())?.();
  };

  dispose(): void {
    window.removeEventListener('keydown', this.handleKeyDown);
    this.handlers.clear();
  }
}

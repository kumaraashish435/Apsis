declare global {
  interface Window {
    CESIUM_BASE_URL: string;
  }
}

export function configureCesium(): void {
  if (typeof window !== 'undefined') {
    window.CESIUM_BASE_URL = '/cesium/';
  }
}
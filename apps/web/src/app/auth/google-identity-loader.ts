const SCRIPT_ID = 'google-identity-services';

let loadPromise: Promise<void> | null = null;

/** Loads accounts.google.com/gsi/client once, no matter how many times this is called. */
export function loadGoogleIdentityScript(): Promise<void> {
  if (loadPromise) {
    return loadPromise;
  }

  loadPromise = new Promise<void>((resolve, reject) => {
    if (document.getElementById(SCRIPT_ID)) {
      resolve();
      return;
    }

    const script = document.createElement('script');
    script.id = SCRIPT_ID;
    script.src = 'https://accounts.google.com/gsi/client';
    script.async = true;
    script.defer = true;
    script.onload = () => resolve();
    script.onerror = () => reject(new Error('Failed to load Google Identity Services script.'));
    document.head.appendChild(script);
  });

  return loadPromise;
}

export interface GoogleCredentialResponse {
  credential: string;
}

interface GoogleIdConfiguration {
  client_id: string;
  callback: (response: GoogleCredentialResponse) => void;
}

interface GoogleButtonOptions {
  theme?: 'outline' | 'filled_blue' | 'filled_black';
  size?: 'large' | 'medium' | 'small';
  shape?: 'rectangular' | 'pill' | 'circle' | 'square';
  width?: number;
  text?: 'signin_with' | 'signup_with' | 'continue_with';
}

// Minimal surface of the Google Identity Services script (accounts.google.com/gsi/client) —
// only what google-sign-in-button.ts actually calls, not the full SDK.
declare global {
  interface Window {
    google?: {
      accounts: {
        id: {
          initialize(config: GoogleIdConfiguration): void;
          renderButton(parent: HTMLElement, options: GoogleButtonOptions): void;
        };
      };
    };
  }
}

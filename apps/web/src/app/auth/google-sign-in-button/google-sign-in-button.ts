import { Component, ElementRef, OnInit, ViewChild, inject, output, signal } from '@angular/core';
import { Router } from '@angular/router';
import { AuthService } from '../auth.service';
import { loadGoogleIdentityScript } from '../google-identity-loader';
import '../google-identity.types';

/**
 * Renders Google's own "Sign in with Google" button once it knows the API's
 * OAuth client id. Used identically from both sign-in and sign-up — Google
 * sign-in doesn't distinguish the two, the backend finds-or-creates the
 * account either way (see AuthController.GoogleSignIn). See ../README.md.
 */
@Component({
  selector: 'app-google-sign-in-button',
  imports: [],
  templateUrl: './google-sign-in-button.html',
  styleUrl: './google-sign-in-button.scss',
})
export class GoogleSignInButton implements OnInit {
  private readonly auth = inject(AuthService);
  private readonly router = inject(Router);

  @ViewChild('container', { static: true })
  private readonly container!: ElementRef<HTMLDivElement>;

  protected readonly unavailable = signal(false);
  readonly signInError = output<string>();

  ngOnInit(): void {
    this.auth.googleClientId().subscribe({
      next: (clientId) => {
        if (!clientId) {
          this.unavailable.set(true);
          return;
        }
        this.initializeButton(clientId);
      },
      error: () => this.unavailable.set(true),
    });
  }

  private initializeButton(clientId: string): void {
    loadGoogleIdentityScript()
      .then(() => {
        if (!window.google) {
          this.unavailable.set(true);
          return;
        }
        window.google.accounts.id.initialize({
          client_id: clientId,
          callback: (response) => this.handleCredential(response.credential),
        });
        window.google.accounts.id.renderButton(this.container.nativeElement, {
          theme: 'outline',
          size: 'large',
          shape: 'pill',
          width: 320,
          text: 'continue_with',
        });
      })
      .catch(() => this.unavailable.set(true));
  }

  private handleCredential(idToken: string): void {
    this.auth.signInWithGoogle(idToken).subscribe({
      next: () => this.router.navigateByUrl('/'),
      error: () => this.signInError.emit('Google sign-in failed. Please try again.'),
    });
  }
}

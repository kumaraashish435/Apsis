import { HttpClient } from '@angular/common/http';
import { Injectable, computed, inject, signal } from '@angular/core';
import { Router } from '@angular/router';
import { Observable, map, tap } from 'rxjs';
import { AuthResponse, AuthUser } from './auth.models';

const TOKEN_KEY = 'apsis.auth.token';
const USER_KEY = 'apsis.auth.user';

/**
 * Session state lives in localStorage so a page refresh doesn't sign the user
 * out; the `user` signal is the in-memory mirror everything else reads from.
 * See apps/api/Apsis.Api/Modules/Users/README.md for the server side of this.
 */
@Injectable({ providedIn: 'root' })
export class AuthService {
  private readonly http = inject(HttpClient);
  private readonly router = inject(Router);

  private readonly userSignal = signal<AuthUser | null>(readStoredUser());
  readonly user = this.userSignal.asReadonly();
  readonly isAuthenticated = computed(() => this.userSignal() !== null);

  get token(): string | null {
    return localStorage.getItem(TOKEN_KEY);
  }

  signUp(email: string, password: string, displayName: string): Observable<AuthResponse> {
    return this.http
      .post<AuthResponse>('/api/v1/auth/register', { email, password, displayName })
      .pipe(tap((res) => this.setSession(res)));
  }

  signIn(email: string, password: string): Observable<AuthResponse> {
    return this.http
      .post<AuthResponse>('/api/v1/auth/login', { email, password })
      .pipe(tap((res) => this.setSession(res)));
  }

  /** idToken is the credential Google Identity Services hands back — see GoogleSignInButton. */
  signInWithGoogle(idToken: string): Observable<AuthResponse> {
    return this.http
      .post<AuthResponse>('/api/v1/auth/google', { idToken })
      .pipe(tap((res) => this.setSession(res)));
  }

  /** Empty string means Google sign-in isn't configured on the API — see GoogleSignInButton. */
  googleClientId(): Observable<string> {
    return this.http
      .get<{ clientId: string }>('/api/v1/auth/google/client-id')
      .pipe(map((res) => res.clientId));
  }

  signOut(): void {
    localStorage.removeItem(TOKEN_KEY);
    localStorage.removeItem(USER_KEY);
    this.userSignal.set(null);
    this.router.navigateByUrl('/auth/sign-in');
  }

  private setSession(res: AuthResponse): void {
    localStorage.setItem(TOKEN_KEY, res.accessToken);
    localStorage.setItem(USER_KEY, JSON.stringify(res.user));
    this.userSignal.set(res.user);
  }
}

function readStoredUser(): AuthUser | null {
  const raw = localStorage.getItem(USER_KEY);
  if (!raw) {
    return null;
  }
  try {
    return JSON.parse(raw) as AuthUser;
  } catch {
    return null;
  }
}

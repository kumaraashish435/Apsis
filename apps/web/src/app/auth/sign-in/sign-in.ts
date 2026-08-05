import { HttpErrorResponse } from '@angular/common/http';
import { Component, inject, signal } from '@angular/core';
import { FormBuilder, ReactiveFormsModule, Validators } from '@angular/forms';
import { Router, RouterLink } from '@angular/router';
import { AuthService } from '../auth.service';
import { GoogleSignInButton } from '../google-sign-in-button/google-sign-in-button';

@Component({
  selector: 'app-sign-in',
  imports: [ReactiveFormsModule, RouterLink, GoogleSignInButton],
  templateUrl: './sign-in.html',
  styleUrl: './sign-in.scss',
})
export class SignIn {
  private readonly fb = inject(FormBuilder);
  private readonly auth = inject(AuthService);
  private readonly router = inject(Router);

  protected readonly submitting = signal(false);
  protected readonly errorMessage = signal<string | null>(null);

  protected readonly form = this.fb.nonNullable.group({
    email: ['', [Validators.required, Validators.email]],
    password: ['', [Validators.required]],
  });

  protected submit(): void {
    if (this.form.invalid || this.submitting()) {
      this.form.markAllAsTouched();
      return;
    }

    this.submitting.set(true);
    this.errorMessage.set(null);
    const { email, password } = this.form.getRawValue();

    this.auth.signIn(email, password).subscribe({
      next: () => this.router.navigateByUrl('/'),
      error: (err: HttpErrorResponse) => {
        this.submitting.set(false);
        this.errorMessage.set(
          err.status === 401
            ? 'Invalid email or password.'
            : 'Something went wrong. Please try again.',
        );
      },
    });
  }

  protected onGoogleError(message: string): void {
    this.errorMessage.set(message);
  }
}

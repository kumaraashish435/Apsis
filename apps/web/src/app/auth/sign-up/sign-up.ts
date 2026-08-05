import { HttpErrorResponse } from '@angular/common/http';
import { Component, inject, signal } from '@angular/core';
import { FormBuilder, ReactiveFormsModule, Validators } from '@angular/forms';
import { Router, RouterLink } from '@angular/router';
import { AuthService } from '../auth.service';
import { GoogleSignInButton } from '../google-sign-in-button/google-sign-in-button';

@Component({
  selector: 'app-sign-up',
  imports: [ReactiveFormsModule, RouterLink, GoogleSignInButton],
  templateUrl: './sign-up.html',
  styleUrl: './sign-up.scss',
})
export class SignUp {
  private readonly fb = inject(FormBuilder);
  private readonly auth = inject(AuthService);
  private readonly router = inject(Router);

  protected readonly submitting = signal(false);
  protected readonly errorMessage = signal<string | null>(null);

  protected readonly form = this.fb.nonNullable.group({
    displayName: ['', [Validators.required]],
    email: ['', [Validators.required, Validators.email]],
    password: ['', [Validators.required, Validators.minLength(8)]],
  });

  protected submit(): void {
    if (this.form.invalid || this.submitting()) {
      this.form.markAllAsTouched();
      return;
    }

    this.submitting.set(true);
    this.errorMessage.set(null);
    const { displayName, email, password } = this.form.getRawValue();

    this.auth.signUp(email, password, displayName).subscribe({
      next: () => this.router.navigateByUrl('/'),
      error: (err: HttpErrorResponse) => {
        this.submitting.set(false);
        this.errorMessage.set(
          err.status === 409
            ? 'An account with this email already exists.'
            : 'Something went wrong. Please try again.',
        );
      },
    });
  }

  protected onGoogleError(message: string): void {
    this.errorMessage.set(message);
  }
}

import { HttpErrorResponse, HttpInterceptorFn } from '@angular/common/http';
import { inject } from '@angular/core';
import { catchError, throwError } from 'rxjs';
import { AuthService } from './auth.service';

const PUBLIC_PATHS = [
  '/api/v1/auth/login',
  '/api/v1/auth/register',
  '/api/v1/auth/google',
];

/** Attaches the bearer token to API calls and signs out on a 401 from the API. */
export const authInterceptor: HttpInterceptorFn = (req, next) => {
  const auth = inject(AuthService);
  const isPublic = PUBLIC_PATHS.some((path) => req.url.includes(path));
  const token = auth.token;

  const authorizedReq =
    token && !isPublic ? req.clone({ setHeaders: { Authorization: `Bearer ${token}` } }) : req;

  return next(authorizedReq).pipe(
    catchError((error: unknown) => {
      if (error instanceof HttpErrorResponse && error.status === 401 && !isPublic) {
        auth.signOut();
      }
      return throwError(() => error);
    }),
  );
};

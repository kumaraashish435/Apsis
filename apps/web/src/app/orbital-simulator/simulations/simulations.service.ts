import { HttpClient } from '@angular/common/http';
import { Injectable, inject } from '@angular/core';
import { Observable } from 'rxjs';
import { tap } from 'rxjs/operators';
import {
  CompareResponse,
  ExportFormat,
  PropagateResponse,
  SaveSimulationRequest,
  ShareResponse,
  Simulation,
} from './simulation.models';

const BASE = '/api/v1/simulations';

/**
 * Thin HTTP client over Modules/Simulations' API — see
 * apps/api/Apsis.Api/Modules/Simulations/README.md for what each endpoint
 * does server-side. Every method here has an authenticated form (uses the
 * bearer token via authInterceptor, like every other HttpClient call in this
 * app) and, for the read/export endpoints, a `Shared` counterpart that hits
 * the public `/shared/{token}` routes instead — used by the read-only shared
 * viewer page, which has no session at all.
 */
@Injectable({ providedIn: 'root' })
export class SimulationsService {
  private readonly http = inject(HttpClient);

  list(): Observable<Simulation[]> {
    return this.http.get<Simulation[]>(BASE);
  }

  get(id: string): Observable<Simulation> {
    return this.http.get<Simulation>(`${BASE}/${id}`);
  }

  create(request: SaveSimulationRequest): Observable<Simulation> {
    return this.http.post<Simulation>(BASE, request);
  }

  update(id: string, request: SaveSimulationRequest): Observable<Simulation> {
    return this.http.put<Simulation>(`${BASE}/${id}`, request);
  }

  remove(id: string): Observable<void> {
    return this.http.delete<void>(`${BASE}/${id}`);
  }

  propagate(id: string): Observable<PropagateResponse> {
    return this.http.get<PropagateResponse>(`${BASE}/${id}/propagate`);
  }

  compare(id: string): Observable<CompareResponse> {
    return this.http.get<CompareResponse>(`${BASE}/${id}/compare`);
  }

  share(id: string): Observable<ShareResponse> {
    return this.http.post<ShareResponse>(`${BASE}/${id}/share`, {});
  }

  unshare(id: string): Observable<ShareResponse> {
    return this.http.delete<ShareResponse>(`${BASE}/${id}/share`);
  }

  /** Triggers a browser download via a Blob — see downloadBlob() for why this isn't a plain link. */
  export(id: string, format: ExportFormat, fileNameHint: string): Observable<Blob> {
    return this.http
      .get(`${BASE}/${id}/export?format=${format}`, { responseType: 'blob' })
      .pipe(tap((blob) => downloadBlob(blob, `${fileNameHint}.${format}`)));
  }

  // ---- Public, unauthenticated — the shared read-only viewer ----

  getShared(token: string): Observable<Simulation> {
    return this.http.get<Simulation>(`${BASE}/shared/${token}`);
  }

  propagateShared(token: string): Observable<PropagateResponse> {
    return this.http.get<PropagateResponse>(`${BASE}/shared/${token}/propagate`);
  }

  compareShared(token: string): Observable<CompareResponse> {
    return this.http.get<CompareResponse>(`${BASE}/shared/${token}/compare`);
  }

  exportShared(token: string, format: ExportFormat, fileNameHint: string): Observable<Blob> {
    return this.http
      .get(`${BASE}/shared/${token}/export?format=${format}`, { responseType: 'blob' })
      .pipe(tap((blob) => downloadBlob(blob, `${fileNameHint}.${format}`)));
  }
}

/**
 * A plain `<a href="/api/.../export">` can't carry the Authorization header a
 * bearer-token API requires, so export goes through HttpClient like every
 * other call (picking up the interceptor's header) and the resulting Blob is
 * saved via a synthetic, immediately-revoked object URL instead.
 */
function downloadBlob(blob: Blob, fileName: string): void {
  const url = URL.createObjectURL(blob);
  const anchor = document.createElement('a');
  anchor.href = url;
  anchor.download = fileName;
  anchor.click();
  URL.revokeObjectURL(url);
}

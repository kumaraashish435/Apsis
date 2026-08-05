export type SimulationInputType = 'Tle' | 'Elements';

export interface Simulation {
  id: string;
  name: string;
  inputType: SimulationInputType;
  tleLine1: string | null;
  tleLine2: string | null;
  semiMajorAxisKm: number | null;
  eccentricity: number | null;
  inclinationDeg: number | null;
  raanDeg: number | null;
  argPerigeeDeg: number | null;
  trueAnomalyDeg: number | null;
  startTime: string;
  endTime: string;
  stepSeconds: number;
  isPublic: boolean;
  /** Only present when fetched by the owner — never on a public/shared response. */
  shareToken: string | null;
  createdAt: string;
}

export interface SaveSimulationRequest {
  name: string;
  inputType: SimulationInputType;
  tleLine1?: string;
  tleLine2?: string;
  semiMajorAxisKm?: number;
  eccentricity?: number;
  inclinationDeg?: number;
  raanDeg?: number;
  argPerigeeDeg?: number;
  trueAnomalyDeg?: number;
  startTime: string;
  endTime: string;
  stepSeconds: number;
}

export interface StateVector {
  epochSeconds: number;
  xKm: number;
  yKm: number;
  zKm: number;
  vxKmS: number;
  vyKmS: number;
  vzKmS: number;
  latDeg: number;
  lonDeg: number;
}

/** Osculating classical elements at one instant — see the backend's OrbitalElementsCalculator.cs. */
export interface OrbitalElementsReadout {
  altitudeKm: number;
  semiMajorAxisKm: number;
  eccentricity: number;
  inclinationDeg: number;
  raanDeg: number;
  argPerigeeDeg: number;
  trueAnomalyDeg: number;
  periodSeconds: number;
  apogeeAltitudeKm: number;
  perigeeAltitudeKm: number;
}

export interface PropagateResponse {
  states: StateVector[];
  /** Parallel to states. */
  elements: OrbitalElementsReadout[];
}

export interface DivergencePoint {
  epochSeconds: number;
  distanceKm: number;
}

export interface CompareResponse {
  sgp4States: StateVector[];
  keplerianStates: StateVector[];
  divergence: DivergencePoint[];
}

export interface ShareResponse {
  isPublic: boolean;
  shareToken: string;
}

export type ExportFormat = 'csv' | 'json' | 'czml';

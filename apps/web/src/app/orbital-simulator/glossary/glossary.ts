import { Component } from '@angular/core';

interface GlossaryTerm {
  term: string;
  definition: string;
}

@Component({
  selector: 'app-glossary',
  imports: [],
  templateUrl: './glossary.html',
  styleUrl: './glossary.scss',
})
export class Glossary {
  // Deliberately only the terms this app's own UI actually surfaces (the
  // simulation editor's fields, the orbital-parameter readout, the compare
  // and export pages) — not a general astrodynamics dictionary.
  protected readonly terms: GlossaryTerm[] = [
    {
      term: 'TLE (Two-Line Element set)',
      definition:
        'A compact, fixed-column text format for describing a satellite\'s orbit at a specific moment (its "epoch") — the standard input for SGP4. Published by sources like Celestrak, refreshed regularly since real orbits decay and shift over time.',
    },
    {
      term: 'SGP4 / SDP4',
      definition:
        'The standard propagator for near-Earth (SGP4) and deep-space, period > 225 min (SDP4) satellites — the model that actually flew on the TLEs it reads. Accounts for atmospheric drag and the perturbing effects of Earth\'s non-spherical shape, which is why its output drifts from a pure Keplerian orbit over time. See "Propagator comparison" to see that drift directly.',
    },
    {
      term: 'Classical orbital elements',
      definition:
        'Six numbers that fully describe an unperturbed (Keplerian) orbit: semi-major axis, eccentricity, inclination, RAAN, argument of perigee, and true anomaly — see each below.',
    },
    {
      term: 'Semi-major axis',
      definition:
        'Half the longest diameter of the orbit\'s ellipse — sets the orbit\'s size and, with nothing else, its period (a bigger orbit takes longer to complete).',
    },
    {
      term: 'Eccentricity',
      definition:
        '0 for a perfect circle, approaching 1 for an increasingly stretched ellipse. Determines how much altitude varies over one orbit — see apogee/perigee.',
    },
    {
      term: 'Inclination',
      definition:
        "The tilt of the orbital plane relative to Earth's equator, in degrees. 0° is equatorial (orbits directly above the equator); 90° is polar; near 180° orbits backwards relative to Earth's spin.",
    },
    {
      term: 'RAAN (Right Ascension of the Ascending Node)',
      definition:
        "Where the orbit crosses Earth's equatorial plane heading north, measured as an angle in the equatorial plane from a fixed reference direction. Rotates the whole orbital plane around Earth's axis without changing its tilt.",
    },
    {
      term: 'Argument of perigee',
      definition:
        'The angle, measured within the orbital plane, from the ascending node to perigee (the closest point to Earth). Rotates the ellipse within its own plane.',
    },
    {
      term: 'True anomaly',
      definition:
        "The satellite's current position along its orbit, measured as an angle from perigee. 0° is at perigee (closest approach), 180° is at apogee (farthest point) — this is the one element that keeps changing as the satellite moves, even though the other five stay ~constant for an unperturbed orbit.",
    },
    {
      term: 'Apogee / Perigee',
      definition:
        "The highest and lowest points of an orbit above Earth's surface. A circular orbit (eccentricity 0) has apogee = perigee; the more eccentric the orbit, the further apart they are.",
    },
    {
      term: 'Osculating elements',
      definition:
        'The classical elements that would exactly describe the orbit if all perturbations vanished at this instant — computed fresh at each propagated point (see the readout panel in the 3D/ground-track viewer). For a perturbed model like SGP4 they drift slightly step to step, unlike a pure Keplerian orbit\'s constant elements.',
    },
    {
      term: 'Ground track',
      definition:
        "The path traced on Earth's surface directly beneath the satellite over time — what you'd see if you projected the satellite straight down onto the ground at every instant. Requires knowing Earth's own rotation (see GMST) in addition to the satellite's orbit.",
    },
    {
      term: 'GMST (Greenwich Mean Sidereal Time)',
      definition:
        "Earth's rotation angle relative to the stars (not the Sun) at a given moment — the conversion needed to turn a satellite's position in inertial space (ECI) into a latitude/longitude on the rotating ground beneath it.",
    },
    {
      term: 'ECI / TEME (Earth-Centered Inertial)',
      definition:
        "The reference frame this app's propagators output positions in — fixed relative to the stars, not rotating with Earth. Everything except the ground track uses this frame directly; the ground track is the one place Earth's rotation gets factored back in.",
    },
    {
      term: 'CZML',
      definition:
        'A JSON-based format (from the Cesium project) for describing time-dynamic 3D scenes — one of this app\'s export formats, readable by Cesium or any other CZML-aware tool even though this app\'s own 3D viewer is built on Three.js.',
    },
  ];
}

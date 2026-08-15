#pragma once

namespace Propagate {

struct SatelliteState {
    bool ok = false;             // false if propagation failed (bad satKey, decayed orbit, etc.)
    double positionKm[3] = {};   // ECI (TEME of epoch) position, km
    double velocityKmS[3] = {};  // ECI velocity, km/s
    double latitudeDeg = 0.0;
    double longitudeDeg = 0.0;
    double heightKm = 0.0;
    double minutesSinceEpoch = 0.0;
};

// Propagates a satellite (already loaded via GpDataLoader, i.e. Sgp4InitSat
// has already succeeded for this satKey) to the current wall-clock UTC time.
SatelliteState PropagateToNow(long long satKey);

// Propagates to an arbitrary UTC calendar time.
SatelliteState PropagateToUtc(long long satKey, int year, int month, int day,
                               int hour, int minute, double second);

}

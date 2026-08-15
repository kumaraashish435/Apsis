#include "SatelliteTracker.h"

#include <array>
#include <chrono>
#include <ctime>

#include "AstroApi.hpp"

namespace Propagate {

SatelliteState PropagateToUtc(long long satKey, int year, int month, int day,
                               int hour, int minute, double second) {
    auto& astro = AstroApi::instance();

    double ds50UTC = astro.TimeFunc().TimeComps2ToUTC(year, month, day, hour, minute, second);

    SatelliteState state;
    std::array<double, 3> pos{}, vel{}, llh{};
    double mse = 0.0;

    int errCode = astro.Sgp4Prop().Sgp4PropDs50UTC(satKey, ds50UTC, mse, pos, vel, llh);
    state.ok = (errCode == 0);
    state.positionKm[0] = pos[0]; state.positionKm[1] = pos[1]; state.positionKm[2] = pos[2];
    state.velocityKmS[0] = vel[0]; state.velocityKmS[1] = vel[1]; state.velocityKmS[2] = vel[2];
    state.latitudeDeg = llh[0];
    state.longitudeDeg = llh[1];
    state.heightKm = llh[2];
    state.minutesSinceEpoch = mse;

    return state;
}

SatelliteState PropagateToNow(long long satKey) {
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm utc{};
#if defined(_WIN32)
    gmtime_s(&utc, &now);
#else
    gmtime_r(&now, &utc);
#endif

    return PropagateToUtc(satKey,
                           utc.tm_year + 1900,
                           utc.tm_mon + 1,
                           utc.tm_mday,
                           utc.tm_hour,
                           utc.tm_min,
                           static_cast<double>(utc.tm_sec));
}

}

#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "Core/Core.h"
#include "propagate/GpDataLoader.h"
#include "propagate/SatelliteTracker.h"

// Sample GP (General Perturbations) data for the ISS, in Celestrak's CSV
// format (https://celestrak.org/NORAD/elements/gp.php?CATNR=25544&FORMAT=csv).
// Used only when no file is given on the command line -- see SampleInputs/
// for one real file per supported format (TLE, 3LE, CSV, JSON, OMM KVN,
// OMM XML) to pass instead.
static const char* kSampleCsv =
    "OBJECT_NAME,OBJECT_ID,EPOCH,MEAN_MOTION,ECCENTRICITY,INCLINATION,RA_OF_ASC_NODE,"
    "ARG_OF_PERICENTER,MEAN_ANOMALY,EPHEMERIS_TYPE,CLASSIFICATION_TYPE,NORAD_CAT_ID,"
    "ELEMENT_SET_NO,REV_AT_EPOCH,BSTAR,MEAN_MOTION_DOT,MEAN_MOTION_DDOT\n"
    "ISS (ZARYA),1998-067A,2026-08-13T20:35:52.287648,15.49433609,.00074948,51.6326,"
    "14.6698,43.6909,316.4672,0,U,25544,999,58069,.85019655E-4,.4307E-4,0\n";

// A quick sanity check for ad hoc runs against arbitrary input files: is
// this a physically plausible Earth-orbit answer at all (not NaN, not
// inside the Earth, not absurdly far away)? This is deliberately loose --
// it's a "did this obviously break" signal, not a correctness check. The
// Tests target (Tests/Source/PropagateTests.cpp) is what actually verifies
// SGP4 output against known-good reference values per data format.
static bool LooksPhysicallyPlausible(const Propagate::SatelliteState& s)
{
    if (!s.ok) return false;
    double r = std::sqrt(s.positionKm[0] * s.positionKm[0] +
                          s.positionKm[1] * s.positionKm[1] +
                          s.positionKm[2] * s.positionKm[2]);
    // Earth radius ~6378 km; comfortably past GEO (~42164 km) as an upper bound.
    return r > 6400.0 && s.heightKm > 0.0 && s.heightKm < 45000.0;
}

int RunFile(const std::string& path)
{
    std::vector<long long> satKeys;
    try {
        satKeys = Propagate::LoadGpFile(path);
    } catch (const std::exception& e) {
        std::cerr << "Failed to read \"" << path << "\": " << e.what() << "\n";
        return 1;
    }

    std::cout << "Loaded " << satKeys.size() << " satellite(s) from " << path << "\n\n";

    int failures = 0;
    for (long long satKey : satKeys)
    {
        auto state = Propagate::PropagateToNow(satKey);
        bool pass = LooksPhysicallyPlausible(state);
        if (!pass) ++failures;

        std::cout << std::fixed << std::setprecision(4)
                   << (pass ? "[PASS] " : "[FAIL] ")
                   << "satKey=" << satKey
                   << "  lat=" << state.latitudeDeg << " deg"
                   << "  lon=" << state.longitudeDeg << " deg"
                   << "  alt=" << state.heightKm << " km"
                   << "  pos=(" << state.positionKm[0] << ", "
                                 << state.positionKm[1] << ", "
                                 << state.positionKm[2] << ") km\n";
    }

    std::cout << "\n" << (satKeys.size() - failures) << "/" << satKeys.size()
               << " satellite(s) propagated to a physically plausible position.\n";

    return failures > 0 ? 1 : 0;
}

int main(int argc, char* argv[])
{
    Core::PrintHelloWorld();
    std::cout << "\n";

    if (argc > 1)
    {
        return RunFile(argv[1]);
    }

    // No file given: run the built-in demo.
    auto satKeys = Propagate::LoadGpData(kSampleCsv, Propagate::GpFormat::Csv);
    std::cout << "Loaded " << satKeys.size() << " satellite(s) from the built-in sample.\n"
               << "Pass a GP data file (see SampleInputs/) to propagate your own, e.g.:\n"
               << "  " << argv[0] << " SampleInputs/iss.tle\n\n";

    for (long long satKey : satKeys)
    {
        auto state = Propagate::PropagateToNow(satKey);
        if (!state.ok)
        {
            std::cout << "satKey " << satKey << ": propagation failed\n";
            continue;
        }

        std::cout << std::fixed << std::setprecision(4)
                   << "satKey " << satKey << "  now:"
                   << "  lat=" << state.latitudeDeg << " deg"
                   << "  lon=" << state.longitudeDeg << " deg"
                   << "  alt=" << state.heightKm << " km"
                   << "  pos=(" << state.positionKm[0] << ", "
                                 << state.positionKm[1] << ", "
                                 << state.positionKm[2] << ") km\n";
    }
}

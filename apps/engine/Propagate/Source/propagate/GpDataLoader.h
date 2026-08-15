#pragma once

#include <string>
#include <vector>

namespace Propagate {

// General Perturbations (GP) element set formats served by Celestrak's
// gp.php endpoint (https://celestrak.org/NORAD/elements/gp.php?...&FORMAT=...).
enum class GpFormat {
    Auto,     // sniff the content and pick one of the formats below
    Tle,      // classic NORAD 2-line element set (FORMAT=tle), optionally
              // preceded by a name line (FORMAT=3le) -- both are handled here
    Csv,      // FORMAT=csv
    Json,     // FORMAT=json or FORMAT=json-pp (same grammar, just pretty-printed)
    OmmKvn,   // FORMAT=kvn  (CCSDS Orbit Mean-Elements Message, key/value text)
    OmmXml    // FORMAT=xml  (CCSDS Orbit Mean-Elements Message, XML)
};

// Looks at the shape of the text (leading '<', '[' / '{', "CCSDS_OMM_VERS",
// a CSV header row, or plain TLE lines) and returns the format it matches.
GpFormat DetectFormat(const std::string& text);

// Parses one or more satellites out of `text` and, for each one, loads it
// into the AstroStds Tle library and initializes it for SGP4 propagation
// (Sgp4InitSat). Returns the satKey of every satellite that parsed and
// initialized successfully, in the order encountered; entries that fail
// either step are skipped and a warning is written to stderr rather than
// aborting the whole batch.
//
// Precondition: AstroApi::instance() must already have been touched once
// (SatelliteTracker::PropagateToNow/PropagateToUtc does this for you, or
// call AstroApi::instance() yourself before loading if you propagate some
// other way).
std::vector<long long> LoadGpData(const std::string& text, GpFormat format = GpFormat::Auto);

// Convenience wrapper: reads the whole file at `path` and calls LoadGpData.
// Throws std::runtime_error if the file can't be opened.
std::vector<long long> LoadGpFile(const std::string& path, GpFormat format = GpFormat::Auto);

}

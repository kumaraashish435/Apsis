#include "GpDataLoader.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>

#include "AstroApi.hpp"

namespace Propagate {

namespace {

// The SGP4 engine's Sgp4InitSat() reads SGP4_Open_License.txt by searching
// LD_LIBRARY_PATH at call time (not the dylib's own resolved directory, and
// not the process's cwd), so it has to be set before the first Sgp4InitSat()
// call in the process -- setting it here (rather than relying on the caller
// to export it) is what lets `App`/`Tests` run standalone with no env var
// setup. On Windows the documented mechanism is PATH instead (and the DLLs
// are additionally copied next to the .exe by the build, per
// Propagate/CMakeLists.txt / premake5.lua) but LD_LIBRARY_PATH is set there
// too in case the license check itself looks for that literal name
// regardless of platform -- harmless either way.
void EnsureAstroStdsLicenseEnv() {
#if defined(ASTROSTDS_LIB_DIR)
    static bool done = false;
    if (!done) {
#if defined(_WIN32)
        _putenv_s("LD_LIBRARY_PATH", ASTROSTDS_LIB_DIR);
        std::string path = ASTROSTDS_LIB_DIR;
        if (const char* existing = std::getenv("PATH")) {
            path += ";";
            path += existing;
        }
        _putenv_s("PATH", path.c_str());
#else
        setenv("LD_LIBRARY_PATH", ASTROSTDS_LIB_DIR, 1);
#endif
        done = true;
    }
#endif
}

// ---------------------------------------------------------------------
// Small string helpers
// ---------------------------------------------------------------------

std::string Trim(const std::string& s) {
    size_t begin = s.find_first_not_of(" \t\r\n");
    if (begin == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(begin, end - begin + 1);
}

std::vector<std::string> SplitLines(const std::string& text) {
    std::vector<std::string> lines;
    std::stringstream ss(text);
    std::string line;
    while (std::getline(ss, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        lines.push_back(line);
    }
    return lines;
}

// Splits a CSV row on commas, respecting double-quoted fields.
std::vector<std::string> SplitCsvRow(const std::string& row) {
    std::vector<std::string> fields;
    std::string field;
    bool inQuotes = false;
    for (char c : row) {
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field.push_back(c);
        }
    }
    fields.push_back(field);
    return fields;
}

double ToDouble(const std::string& s, double fallback = 0.0) {
    std::string t = Trim(s);
    if (t.empty()) return fallback;
    try { return std::stod(t); } catch (...) { return fallback; }
}

long ToLong(const std::string& s, long fallback = 0) {
    std::string t = Trim(s);
    if (t.empty()) return fallback;
    try { return std::stol(t); } catch (...) { return fallback; }
}

// ---------------------------------------------------------------------
// Common GP element fields (the union of what CSV/JSON/KVN/XML all carry)
// ---------------------------------------------------------------------

struct GpFields {
    std::string objectName;
    char classification = 'U';
    long noradCatId = 0;
    std::string epochIso;       // e.g. "2026-08-13T20:35:52.287648"
    double meanMotion = 0.0;    // revs/day
    double eccentricity = 0.0;
    double inclination = 0.0;   // deg
    double raan = 0.0;          // deg
    double argPerigee = 0.0;    // deg
    double meanAnomaly = 0.0;   // deg
    int ephemerisType = 0;
    int elementSetNo = 0;
    long revAtEpoch = 0;
    double bstar = 0.0;
    double meanMotionDot = 0.0;
    double meanMotionDdot = 0.0;
};

// Loads one already-parsed element set into the Tle/Sgp4Prop libraries.
// Returns the satKey, or -1 if the satellite failed to load/initialize.
long long LoadOneGpFields(const GpFields& f) {
    auto& astro = AstroApi::instance();

    // ISO 8601 epoch -> (year, month, day, hour, minute, second), then let
    // the library itself convert that into the (year, fractional day-of-year)
    // pair TleAddSatFrFieldsGP2 expects, so leap-year/calendar handling
    // matches exactly what the propagator uses internally.
    if (f.epochIso.size() < 19) {
        std::cerr << "Warning: skipping satellite " << f.noradCatId
                   << " -- unparseable epoch \"" << f.epochIso << "\"\n";
        return -1;
    }

    int year   = std::stoi(f.epochIso.substr(0, 4));
    int month  = std::stoi(f.epochIso.substr(5, 2));
    int day    = std::stoi(f.epochIso.substr(8, 2));
    int hour   = std::stoi(f.epochIso.substr(11, 2));
    int minute = std::stoi(f.epochIso.substr(14, 2));
    double second = std::stod(f.epochIso.substr(17));

    double ds50UTC = astro.TimeFunc().TimeComps2ToUTC(year, month, day, hour, minute, second);

    int epochYr = 0;
    double epochDays = 0.0;
    astro.TimeFunc().UTCToYrDays(ds50UTC, epochYr, epochDays);

    __int64 satKey = astro.Tle().TleAddSatFrFieldsGP2(
        static_cast<int>(f.noradCatId), f.classification, f.objectName,
        epochYr, epochDays, f.bstar, f.ephemerisType, f.elementSetNo,
        f.inclination, f.raan, f.eccentricity, f.argPerigee, f.meanAnomaly,
        f.meanMotion, static_cast<int>(f.revAtEpoch), f.meanMotionDot, f.meanMotionDdot);

    int errCode = astro.Sgp4Prop().Sgp4InitSat(satKey);
    if (errCode != 0) {
        std::cerr << "Warning: SGP4 init failed (err=" << errCode << ") for NORAD "
                   << f.noradCatId << " (" << f.objectName << ")\n";
        astro.Tle().TleRemoveSat(satKey);
        return -1;
    }

    return static_cast<long long>(satKey);
}

// ---------------------------------------------------------------------
// TLE / 3LE
// ---------------------------------------------------------------------

std::vector<long long> LoadTle(const std::string& text) {
    std::vector<long long> satKeys;
    auto& astro = AstroApi::instance();

    auto lines = SplitLines(text);
    for (size_t i = 0; i < lines.size(); ++i) {
        const std::string& l1 = lines[i];
        bool isLine1 = l1.size() >= 2 && l1[0] == '1' && l1[1] == ' ';
        if (!isLine1) continue;
        if (i + 1 >= lines.size()) continue;
        const std::string& l2 = lines[i + 1];
        bool isLine2 = l2.size() >= 2 && l2[0] == '2' && l2[1] == ' ';
        if (!isLine2) continue;

        __int64 satKey = astro.Tle().TleAddSatFrLines(l1, l2);
        int errCode = astro.Sgp4Prop().Sgp4InitSat(satKey);
        if (errCode != 0) {
            std::cerr << "Warning: SGP4 init failed (err=" << errCode
                      << ") for TLE line: " << l1 << "\n";
            astro.Tle().TleRemoveSat(satKey);
        } else {
            satKeys.push_back(static_cast<long long>(satKey));
        }

        ++i; // consumed the pair
    }
    return satKeys;
}

// ---------------------------------------------------------------------
// CSV  (Celestrak FORMAT=csv column order -- looked up by header name so
// a slightly different column order/subset still works)
// ---------------------------------------------------------------------

std::vector<long long> LoadCsv(const std::string& text) {
    std::vector<long long> satKeys;
    auto lines = SplitLines(text);
    if (lines.empty()) return satKeys;

    auto header = SplitCsvRow(lines[0]);
    std::map<std::string, size_t> col;
    for (size_t i = 0; i < header.size(); ++i) col[Trim(header[i])] = i;

    auto get = [&](const std::vector<std::string>& row, const char* name) -> std::string {
        auto it = col.find(name);
        if (it == col.end() || it->second >= row.size()) return "";
        return row[it->second];
    };

    for (size_t i = 1; i < lines.size(); ++i) {
        if (Trim(lines[i]).empty()) continue;
        auto row = SplitCsvRow(lines[i]);

        GpFields f;
        f.objectName = Trim(get(row, "OBJECT_NAME"));
        std::string cls = Trim(get(row, "CLASSIFICATION_TYPE"));
        f.classification = cls.empty() ? 'U' : cls[0];
        f.noradCatId = ToLong(get(row, "NORAD_CAT_ID"));
        f.epochIso = Trim(get(row, "EPOCH"));
        f.meanMotion = ToDouble(get(row, "MEAN_MOTION"));
        f.eccentricity = ToDouble(get(row, "ECCENTRICITY"));
        f.inclination = ToDouble(get(row, "INCLINATION"));
        f.raan = ToDouble(get(row, "RA_OF_ASC_NODE"));
        f.argPerigee = ToDouble(get(row, "ARG_OF_PERICENTER"));
        f.meanAnomaly = ToDouble(get(row, "MEAN_ANOMALY"));
        f.ephemerisType = static_cast<int>(ToLong(get(row, "EPHEMERIS_TYPE")));
        f.elementSetNo = static_cast<int>(ToLong(get(row, "ELEMENT_SET_NO")));
        f.revAtEpoch = ToLong(get(row, "REV_AT_EPOCH"));
        f.bstar = ToDouble(get(row, "BSTAR"));
        f.meanMotionDot = ToDouble(get(row, "MEAN_MOTION_DOT"));
        f.meanMotionDdot = ToDouble(get(row, "MEAN_MOTION_DDOT"));

        long long satKey = LoadOneGpFields(f);
        if (satKey >= 0) satKeys.push_back(satKey);
    }
    return satKeys;
}

// ---------------------------------------------------------------------
// JSON (Celestrak FORMAT=json / json-pp -- flat array of flat objects,
// so a minimal hand-rolled tokenizer is enough; no library needed)
// ---------------------------------------------------------------------

class JsonScanner {
public:
    explicit JsonScanner(const std::string& text) : s(text) {}

    // Parses `[ {...}, {...}, ... ]` or a single `{...}` and returns one
    // string->string map per object (numbers kept as their raw text).
    std::vector<std::map<std::string, std::string>> ParseObjects() {
        std::vector<std::map<std::string, std::string>> result;
        SkipWs();
        if (Peek() == '[') {
            Advance();
            SkipWs();
            while (Peek() != ']') {
                result.push_back(ParseObject());
                SkipWs();
                if (Peek() == ',') { Advance(); SkipWs(); }
            }
        } else if (Peek() == '{') {
            result.push_back(ParseObject());
        }
        return result;
    }

private:
    const std::string& s;
    size_t pos = 0;

    char Peek() const { return pos < s.size() ? s[pos] : '\0'; }
    void Advance() { ++pos; }
    void SkipWs() { while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos]))) ++pos; }

    std::string ParseString() {
        std::string out;
        Advance(); // opening quote
        while (pos < s.size() && s[pos] != '"') {
            if (s[pos] == '\\' && pos + 1 < s.size()) {
                ++pos;
                out.push_back(s[pos]);
            } else {
                out.push_back(s[pos]);
            }
            ++pos;
        }
        Advance(); // closing quote
        return out;
    }

    std::string ParseRawToken() {
        size_t start = pos;
        while (pos < s.size() && s[pos] != ',' && s[pos] != '}' && s[pos] != ']' &&
               !std::isspace(static_cast<unsigned char>(s[pos]))) {
            ++pos;
        }
        return s.substr(start, pos - start);
    }

    std::map<std::string, std::string> ParseObject() {
        std::map<std::string, std::string> obj;
        Advance(); // '{'
        SkipWs();
        while (Peek() != '}') {
            SkipWs();
            std::string key = ParseString();
            SkipWs();
            Advance(); // ':'
            SkipWs();
            std::string value = (Peek() == '"') ? ParseString() : ParseRawToken();
            obj[key] = value;
            SkipWs();
            if (Peek() == ',') { Advance(); SkipWs(); }
        }
        Advance(); // '}'
        return obj;
    }
};

std::vector<long long> LoadJson(const std::string& text) {
    std::vector<long long> satKeys;
    JsonScanner scanner(text);
    for (auto& obj : scanner.ParseObjects()) {
        auto get = [&](const char* key) -> std::string {
            auto it = obj.find(key);
            return it == obj.end() ? "" : it->second;
        };

        GpFields f;
        f.objectName = get("OBJECT_NAME");
        std::string cls = get("CLASSIFICATION_TYPE");
        f.classification = cls.empty() ? 'U' : cls[0];
        f.noradCatId = ToLong(get("NORAD_CAT_ID"));
        f.epochIso = get("EPOCH");
        f.meanMotion = ToDouble(get("MEAN_MOTION"));
        f.eccentricity = ToDouble(get("ECCENTRICITY"));
        f.inclination = ToDouble(get("INCLINATION"));
        f.raan = ToDouble(get("RA_OF_ASC_NODE"));
        f.argPerigee = ToDouble(get("ARG_OF_PERICENTER"));
        f.meanAnomaly = ToDouble(get("MEAN_ANOMALY"));
        f.ephemerisType = static_cast<int>(ToLong(get("EPHEMERIS_TYPE")));
        f.elementSetNo = static_cast<int>(ToLong(get("ELEMENT_SET_NO")));
        f.revAtEpoch = ToLong(get("REV_AT_EPOCH"));
        f.bstar = ToDouble(get("BSTAR"));
        f.meanMotionDot = ToDouble(get("MEAN_MOTION_DOT"));
        f.meanMotionDdot = ToDouble(get("MEAN_MOTION_DDOT"));

        long long satKey = LoadOneGpFields(f);
        if (satKey >= 0) satKeys.push_back(satKey);
    }
    return satKeys;
}

// ---------------------------------------------------------------------
// OMM KVN (CCSDS Orbit Mean-Elements Message, key = value text). Celestrak
// concatenates multiple blocks -- each starts with "CCSDS_OMM_VERS" -- for
// multi-satellite (GROUP=...) queries.
// ---------------------------------------------------------------------

std::vector<long long> LoadKvn(const std::string& text) {
    std::vector<long long> satKeys;

    std::vector<size_t> starts;
    size_t pos = 0;
    while ((pos = text.find("CCSDS_OMM_VERS", pos)) != std::string::npos) {
        starts.push_back(pos);
        pos += 1;
    }
    if (starts.empty()) return satKeys;

    for (size_t i = 0; i < starts.size(); ++i) {
        size_t blockEnd = (i + 1 < starts.size()) ? starts[i + 1] : text.size();
        std::string block = text.substr(starts[i], blockEnd - starts[i]);

        std::map<std::string, std::string> kv;
        for (auto& line : SplitLines(block)) {
            size_t eq = line.find('=');
            if (eq == std::string::npos) continue;
            kv[Trim(line.substr(0, eq))] = Trim(line.substr(eq + 1));
        }

        auto get = [&](const char* key) -> std::string {
            auto it = kv.find(key);
            return it == kv.end() ? "" : it->second;
        };

        GpFields f;
        f.objectName = get("OBJECT_NAME");
        std::string cls = get("CLASSIFICATION_TYPE");
        f.classification = cls.empty() ? 'U' : cls[0];
        f.noradCatId = ToLong(get("NORAD_CAT_ID"));
        f.epochIso = get("EPOCH");
        f.meanMotion = ToDouble(get("MEAN_MOTION"));
        f.eccentricity = ToDouble(get("ECCENTRICITY"));
        f.inclination = ToDouble(get("INCLINATION"));
        f.raan = ToDouble(get("RA_OF_ASC_NODE"));
        f.argPerigee = ToDouble(get("ARG_OF_PERICENTER"));
        f.meanAnomaly = ToDouble(get("MEAN_ANOMALY"));
        f.ephemerisType = static_cast<int>(ToLong(get("EPHEMERIS_TYPE")));
        f.elementSetNo = static_cast<int>(ToLong(get("ELEMENT_SET_NO")));
        f.revAtEpoch = ToLong(get("REV_AT_EPOCH"));
        f.bstar = ToDouble(get("BSTAR"));
        f.meanMotionDot = ToDouble(get("MEAN_MOTION_DOT"));
        f.meanMotionDdot = ToDouble(get("MEAN_MOTION_DDOT"));

        long long satKey = LoadOneGpFields(f);
        if (satKey >= 0) satKeys.push_back(satKey);
    }
    return satKeys;
}

// ---------------------------------------------------------------------
// OMM XML (CCSDS Orbit Mean-Elements Message, XML). Chunked into
// <segment>...</segment> blocks (one per satellite) by a raw substring
// scan -- deliberately not a full XML parser, since the schema Celestrak
// serves is flat, un-attributed leaf tags.
// ---------------------------------------------------------------------

std::string ExtractTag(const std::string& text, const std::string& tag) {
    std::string open = "<" + tag + ">";
    std::string close = "</" + tag + ">";
    size_t start = text.find(open);
    if (start == std::string::npos) return "";
    start += open.size();
    size_t end = text.find(close, start);
    if (end == std::string::npos) return "";
    return Trim(text.substr(start, end - start));
}

std::vector<long long> LoadOmmXml(const std::string& text) {
    std::vector<long long> satKeys;

    std::vector<std::string> segments;
    size_t pos = 0;
    while (true) {
        size_t start = text.find("<segment>", pos);
        if (start == std::string::npos) break;
        size_t end = text.find("</segment>", start);
        if (end == std::string::npos) break;
        end += std::string("</segment>").size();
        segments.push_back(text.substr(start, end - start));
        pos = end;
    }
    if (segments.empty()) segments.push_back(text); // single-record fallback

    for (auto& seg : segments) {
        GpFields f;
        f.objectName = ExtractTag(seg, "OBJECT_NAME");
        std::string cls = ExtractTag(seg, "CLASSIFICATION_TYPE");
        f.classification = cls.empty() ? 'U' : cls[0];
        f.noradCatId = ToLong(ExtractTag(seg, "NORAD_CAT_ID"));
        f.epochIso = ExtractTag(seg, "EPOCH");
        if (f.epochIso.empty()) continue; // not a real segment (e.g. whole-doc fallback with no data)

        f.meanMotion = ToDouble(ExtractTag(seg, "MEAN_MOTION"));
        f.eccentricity = ToDouble(ExtractTag(seg, "ECCENTRICITY"));
        f.inclination = ToDouble(ExtractTag(seg, "INCLINATION"));
        f.raan = ToDouble(ExtractTag(seg, "RA_OF_ASC_NODE"));
        f.argPerigee = ToDouble(ExtractTag(seg, "ARG_OF_PERICENTER"));
        f.meanAnomaly = ToDouble(ExtractTag(seg, "MEAN_ANOMALY"));
        f.ephemerisType = static_cast<int>(ToLong(ExtractTag(seg, "EPHEMERIS_TYPE")));
        f.elementSetNo = static_cast<int>(ToLong(ExtractTag(seg, "ELEMENT_SET_NO")));
        f.revAtEpoch = ToLong(ExtractTag(seg, "REV_AT_EPOCH"));
        f.bstar = ToDouble(ExtractTag(seg, "BSTAR"));
        f.meanMotionDot = ToDouble(ExtractTag(seg, "MEAN_MOTION_DOT"));
        f.meanMotionDdot = ToDouble(ExtractTag(seg, "MEAN_MOTION_DDOT"));

        long long satKey = LoadOneGpFields(f);
        if (satKey >= 0) satKeys.push_back(satKey);
    }
    return satKeys;
}

} // namespace

GpFormat DetectFormat(const std::string& text) {
    std::string t = Trim(text);
    if (t.empty()) return GpFormat::Tle;

    if (t.front() == '<') return GpFormat::OmmXml;
    if (t.front() == '[' || t.front() == '{') return GpFormat::Json;
    if (t.find("CCSDS_OMM_VERS") != std::string::npos && t.find("CCSDS_OMM_VERS") < 64)
        return GpFormat::OmmKvn;
    if (t.rfind("OBJECT_NAME,", 0) == 0 || t.rfind("\"OBJECT_NAME\"", 0) == 0)
        return GpFormat::Csv;

    return GpFormat::Tle;
}

std::vector<long long> LoadGpData(const std::string& text, GpFormat format) {
    EnsureAstroStdsLicenseEnv();

    GpFormat resolved = (format == GpFormat::Auto) ? DetectFormat(text) : format;
    switch (resolved) {
        case GpFormat::Tle:    return LoadTle(text);
        case GpFormat::Csv:    return LoadCsv(text);
        case GpFormat::Json:   return LoadJson(text);
        case GpFormat::OmmKvn: return LoadKvn(text);
        case GpFormat::OmmXml: return LoadOmmXml(text);
        default:               return {};
    }
}

std::vector<long long> LoadGpFile(const std::string& path, GpFormat format) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("LoadGpFile: could not open \"" + path + "\"");
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return LoadGpData(buffer.str(), format);
}

}

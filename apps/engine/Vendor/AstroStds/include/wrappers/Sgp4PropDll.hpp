// This wrapper file was generated automatically by the GenDllWrappers program.
#pragma once

#include "utils.hpp"
#include <string>
#include <vector>
#include <array>
#include <cstring>

class Sgp4PropApi {
public:
    static Sgp4PropApi& instance() {
        static Sgp4PropApi inst;
        return inst;
    }
    Sgp4PropApi(const Sgp4PropApi&)              = delete;
    Sgp4PropApi(Sgp4PropApi&&)                   = delete;
    Sgp4PropApi& operator = (const Sgp4PropApi&) = delete;
    Sgp4PropApi& operator = (Sgp4PropApi&&)      = delete;

    // GP types (TLE ephemeris types) - They are different than ELTTYPE
    // TLE SGP4 gpType (Brouwer mean motion)
    static constexpr int GPTYPE_SGP4 =  0;
    // TLE PPT3 gpType (Brouwer mean motion)
    static constexpr int GPTYPE_PPT3 =  3;
    // TLE SGP4-XP gpType (Brouwer mean motion)
    static constexpr int GPTYPE_XP   =  4;
    // Not a GP type
    static constexpr int GPTYPE_NONE = 99;

    // Different return values of errCode from Sgp4 propagation
    // SGP4 propagates successfully
    static constexpr int GP_ERR_NONE        = 0;
    // Bad FK model (FK5 must be selected)
    static constexpr int GP_ERR_BADFK       = 1;
    // A is negative
    static constexpr int GP_ERR_ANEGATIVE   = 2;
    // A is to large
    static constexpr int GP_ERR_ATOOLARGE   = 3;
    // Eccentricity is hyperbolic
    static constexpr int GP_ERR_EHYPERPOLIC = 4;
    // Eccentricity is negative
    static constexpr int GP_ERR_ENEGATIVE   = 5;
    // Mean anomaly is too large
    static constexpr int GP_ERR_MATOOLARGE  = 6;
    // e**2 is too large
    static constexpr int GP_ERR_E2TOOLARGE  = 7;
    // Inclination out of bounds
    static constexpr int GP_ERR_INCOUTOFBND = 8;
    // Mean motion is negative
    static constexpr int GP_ERR_MNMOTNEG    = 9;
    // Satellite decayed
    static constexpr int GP_ERR_DECAY       =10;
    // Unknown GP element type
    static constexpr int GP_ERR_UNKNOWNTYPE =99;

    // Different time types for passing to Sgp4PropAll
    // propagation time is in minutes since epoch
    static constexpr int SGP4_TIMETYPE_MSE      = 0;
    // propagation time is in days since 1950, UTC
    static constexpr int SGP4_TIMETYPE_DS50UTC  = 1;

    // Sgp4 propagated output fields
    // Revolution number
    static constexpr int XF_SGP4OUT_REVNUM       = 1;
    // Nodal period, apogee, perigee
    static constexpr int XF_SGP4OUT_NODAL_AP_PER = 2;
    // Mean Keplerian
    static constexpr int XF_SGP4OUT_MEAN_KEP     = 3;
    // Osculating Keplerian
    static constexpr int XF_SGP4OUT_OSC_KEP      = 4;

    // Sgp4 propagated data
    // Propagation time in days since 1950, UTC
    static constexpr int XA_SGP4OUT_DS50UTC      =  0;
    // Propagation time in minutes since the satellite's epoch time
    static constexpr int XA_SGP4OUT_MSE          =  1;
    // ECI X position (km) in True Equator and Mean Equinox of Epoch
    static constexpr int XA_SGP4OUT_POSX         =  2;
    // ECI Y position (km) in True Equator and Mean Equinox of Epoch
    static constexpr int XA_SGP4OUT_POSY         =  3;
    // ECI Z position (km) in True Equator and Mean Equinox of Epoch
    static constexpr int XA_SGP4OUT_POSZ         =  4;
    // ECI X velocity (km/s) in True Equator and Mean Equinox of Epoch
    static constexpr int XA_SGP4OUT_VELX         =  5;
    // ECI Y velocity (km/s) in True Equator and Mean Equinox of Epoch
    static constexpr int XA_SGP4OUT_VELY         =  6;
    // ECI Z velocity (km/s) in True Equator and Mean Equinox of Epoch
    static constexpr int XA_SGP4OUT_VELZ         =  7;
    // Geodetic latitude (deg)
    static constexpr int XA_SGP4OUT_LAT          =  8;
    // Geodetic longitude (deg)
    static constexpr int XA_SGP4OUT_LON          =  9;
    // Height above geoid (km)
    static constexpr int XA_SGP4OUT_HEIGHT       = 10;
    // Revolution number
    static constexpr int XA_SGP4OUT_REVNUM       = 11;
    // Nodal period (min)
    static constexpr int XA_SGP4OUT_NODALPER     = 12;
    // Apogee (km)
    static constexpr int XA_SGP4OUT_APOGEE       = 13;
    // Perigee (km)
    static constexpr int XA_SGP4OUT_PERIGEE      = 14;
    // Mean semi-major axis (km)
    static constexpr int XA_SGP4OUT_MN_A         = 15;
    // Mean eccentricity (unitless)
    static constexpr int XA_SGP4OUT_MN_E         = 16;
    // Mean inclination (deg)
    static constexpr int XA_SGP4OUT_MN_INCLI     = 17;
    // Mean mean anomaly (deg)
    static constexpr int XA_SGP4OUT_MN_MA        = 18;
    // Mean right ascension of the asending node (deg)
    static constexpr int XA_SGP4OUT_MN_NODE      = 19;
    // Mean argument of perigee (deg)
    static constexpr int XA_SGP4OUT_MN_OMEGA     = 20;
    // Osculating semi-major axis (km)
    static constexpr int XA_SGP4OUT_OSC_A        = 21;
    // Osculating eccentricity (unitless)
    static constexpr int XA_SGP4OUT_OSC_E        = 22;
    // Osculating inclination (deg)
    static constexpr int XA_SGP4OUT_OSC_INCLI    = 23;
    // Osculating mean anomaly (deg)
    static constexpr int XA_SGP4OUT_OSC_MA       = 24;
    // Osculating right ascension of the asending node (deg)
    static constexpr int XA_SGP4OUT_OSC_NODE     = 25;
    // Osculating argument of perigee (deg)
    static constexpr int XA_SGP4OUT_OSC_OMEGA    = 26;

    static constexpr int XA_SGP4OUT_SIZE         = 64;

    // Different options for generating ephemerides from SGP4
    // ECI TEME of DATE     - 0: time in days since 1950 UTC, 1-3: pos (km), 4-6: vel (km/sec)
    static constexpr int SGP4_EPHEM_ECI   = 1;
    // MEME of J2K (4 terms)- 0: time in days since 1950 UTC, 1-3: pos (km), 4-6: vel (km/sec)
    static constexpr int SGP4_EPHEM_J2K   = 2;


    // Different dynamic step size options
    // Use a simple algorithm to determine step size based on satellite's current position
    static constexpr int DYN_SS_BASIC  = -1;

    //*******************************************************************************

    // Notes: This function has been deprecated since v9.0. 
    // Initializes the Sgp4 DLL for use in the program.
    // <br>
    // If this function returns an error, it is recommended that you stop the program immediately.
    // <br>
    // An error will occur if you forget to load and initialize all the prerequisite DLLs, as listed in the DLL Prerequisites section of the accompanying documentation, before using this DLL.
    int Sgp4Init(__int64 apAddr) const {

        int errCode = p_Sgp4Init(apAddr);

        return errCode;
    }

    // Returns information about the current version of Sgp4Prop.dll. The information is placed in the string parameter you pass in.
    // The returned string provides information about the version number, build date, and platform.
    void Sgp4GetInfo(std::string& infoStr) const {
        char infoStrBuf[129] = {};

        p_Sgp4GetInfo(infoStrBuf);

        from_c(infoStrBuf, infoStr);
    }

    // Loads SGP4-related parameters (prediction controls, JPL settings) and SGP4 elsets from a text file
    // Not available for Web Assembly.
    int Sgp4LoadFileAll(const std::string& sgp4InputFile) const {
        char sgp4InputFileBuf[513] = {};
        to_c(sgp4InputFile, sgp4InputFileBuf);

        int errCode = p_Sgp4LoadFileAll(sgp4InputFileBuf);

        return errCode;
    }

    // Saves currently loaded SGP4-related parameters (SGP4 application controls, prediction controls, integration controls) to a file
    // The purpose of this function is to save the current SGP4-related settings, usually used in GUI applications, for future use.
    // Not available for Web Assembly.
    int Sgp4SaveFile(const std::string& sgp4File, int saveMode, int saveForm) const {
        char sgp4FileBuf[513] = {};
        to_c(sgp4File, sgp4FileBuf);

        int errCode = p_Sgp4SaveFile(sgp4FileBuf, saveMode, saveForm);

        return errCode;
    }

    // Initializes an SGP4 satellite from an SGP or SGP4 TLE.
    // Internally, when this function is called, Tle.dll's set of TLEs is searched for the provided satKey. If found, the associated TLE data will be used to create an SGP4 satellite which then will be added to Sgp4Prop.dll's set of satellites. Subsequent calls to propagate this satellite will use the data in this set to compute the satellite's new state.
    // 
    // This routine should be called once for each satellite you wish to propagate before propagation begins, or any time the associated data that is stored by Tle.dll is changed.
    // 
    // The call to this routine needs to be placed before any calls to the SGP4 propagator routines (Sgp4PropMse(), Sgp4PropDs50UTC(), etc.).
    int Sgp4InitSat(__int64 satKey) const {

        int errCode = p_Sgp4InitSat(satKey);

        return errCode;
    }

    // Checks to see if this satellite is initiazlied
    int Sgp4IsSatInit(__int64 satKey) const {

        int isInitialized = p_Sgp4IsSatInit(satKey);

        return isInitialized;
    }

    // Removing a satellite from the propagator's set of satellites does not affect the corresponding TLE data loaded by calls to routines in Tle.dll.
    int Sgp4RemoveSat(__int64 satKey) const {

        int errCode = p_Sgp4RemoveSat(satKey);

        return errCode;
    }

    // Removes all currently loaded satellites from memory.
    // Calling this function removes all satellites from the set maintained by Sgp4Prop.dll. However, the TLE data loaded by Tle.dll is unaffected by this function.
    int Sgp4RemoveAllSats() const {

        int errCode = p_Sgp4RemoveAllSats();

        return errCode;
    }

    // Returns the number of GP objects currently created. 
    int Sgp4GetCount() const {

        int numSats = p_Sgp4GetCount();

        return numSats;
    }

    // Propagates a satellite, represented by the satKey, to the time expressed in minutes since the satellite's epoch time. 
    // The resulting data about the satellite is placed in the various reference parameters.
    // It is the users' responsibility to decide what to do with the returned value. For example, if the users want to check for decay or low altitude, they can put that logic into their own code.
    // 
    // This function can be called in random time requests.
    // The following cases will result in an error:
    // <ul>
    // <li>Semi major axis A &lt;= 0 or A &gt;1.0D6</li>
    // <li>Eccentricity E &gt;= 1.0 or E &lt; -1.0D-3</li>
    // <li>Mean anomaly MA&gt;=1.0D10</li>
    // <li>Hyperbolic orbit E<sup>2</sup>&gt;= 1.0</li>
    // <li>satKey doesn't exist in the set of loaded satellites</li>
    // <li>FK model not set to FK5</li>
    // </ul>
    int Sgp4PropMse(__int64 satKey, double mse, double& ds50UTC, std::array<double,3>& pos, std::array<double,3>& vel, std::array<double,3>& llh) const {

        int errCode = p_Sgp4PropMse(satKey, mse, &ds50UTC, pos.data(), vel.data(), llh.data());

        return errCode;
    }

    // Propagates a satellite, represented by the satKey, to the time expressed in days since 1950, UTC. 
    // The resulting data about the satellite is placed in the pos (position), vel (velocity), and llh (Lat/Lon/Height) parameters.
    // It is the users' responsibility to decide what to do with the returned value. For example, if the users want to check for decay or low altitude, they can put that logic into their own code.
    // The following cases will result in an error:
    // <ul>
    // <li>Semi major axis A &lt;= 0 or A &gt;1.0D6</li>
    // <li>Eccentricity E &gt;= 1.0 or E &lt; -1.0D-3</li>
    // <li>Mean anomaly MA&gt;=1.0D10</li>
    // <li>Hyperbolic orbit E<sup>2</sup>&gt;= 1.0</li>
    // <li>satKey doesn't exist in the set of loaded satellites</li>
    // <li>GEO model not set to WGS-72 and/or FK model not set to FK5</li>
    // </ul>
    int Sgp4PropDs50UTC(__int64 satKey, double ds50UTC, double& mse, std::array<double,3>& pos, std::array<double,3>& vel, std::array<double,3>& llh) const {

        int errCode = p_Sgp4PropDs50UTC(satKey, ds50UTC, &mse, pos.data(), vel.data(), llh.data());

        return errCode;
    }

    // Propagates a satellite, represented by the satKey, to the time expressed in days since 1950, UTC. 
    // The resulting data about the satellite is placed in the pos (position), vel (velocity) parameters.
    int Sgp4PropDs50UtcPosVel(__int64 satKey, double ds50UTC, std::array<double,3>& pos, std::array<double,3>& vel) const {

        int errCode = p_Sgp4PropDs50UtcPosVel(satKey, ds50UTC, pos.data(), vel.data());

        return errCode;
    }

    // Propagates a satellite, represented by the satKey, to the time expressed in days since 1950, UTC. 
    // Only the geodetic information is returned by this function.
    // It is the users' responsibility to decide what to do with the returned value. For example, if the users want to check for decay or low altitude, they can put that logic into their own code.
    // 
    // This function is similar to Sgp4PropDs50UTC but returns only LLH.  This function is designed especially for applications which plot ground traces.
    // The following cases will result in an error:
    // <ul>
    // <li>Semi major axis A &lt;= 0 or A &gt;1.0D6</li>
    // <li>Eccentricity E &gt;= 1.0 or E &lt; -1.0D-3</li>
    // <li>Mean anomaly MA&gt;=1.0D10</li>
    // <li>Hyperbolic orbit E<sup>2</sup>&gt;= 1.0</li>
    // <li>satKey doesn't exist in the set of loaded satellites</li>
    // <li>GEO model not set to WGS-72 and/or FK model not set to FK5</li>
    // </ul>
    int Sgp4PropDs50UtcLLH(__int64 satKey, double ds50UTC, std::array<double,3>& llh) const {

        int errCode = p_Sgp4PropDs50UtcLLH(satKey, ds50UTC, llh.data());

        return errCode;
    }

    // Propagates a satellite, represented by the satKey, to the time expressed in days since 1950, UTC. 
    // Only the ECI position vector is returned by this function.
    // It is the users' responsibility to decide what to do with the returned value. For example, if the users want to check for decay or low altitude, they can put that logic into their own code.
    // 
    // This function is similar to Sgp4PropDs50UTC but returns only ECI position vector.  This function is designed especially for applications which plot satellite position in 3D.
    // The following cases will result in an error:
    // <ul>
    // <li>Semi major axis A &lt;= 0 or A &gt;1.0D6</li>
    // <li>Eccentricity E &gt;= 1.0 or E &lt; -1.0D-3</li>
    // <li>Mean anomaly MA&gt;=1.0D10</li>
    // <li>Hyperbolic orbit E<sup>2</sup>&gt;= 1.0</li>
    // <li>satKey doesn't exist in the set of loaded satellites</li>
    // <li>GEO model not set to WGS-72 and/or FK model not set to FK5</li>
    // </ul>
    int Sgp4PropDs50UtcPos(__int64 satKey, double ds50UTC, std::array<double,3>& pos) const {

        int errCode = p_Sgp4PropDs50UtcPos(satKey, ds50UTC, pos.data());

        return errCode;
    }

    // Retrieves propagator's precomputed results. This function can be used to obtain results from 
    // a propagation which are not made available through calls to the propagation functions themselves.
    // <br>
    // See example in Sgp4PropMse or Sgp4PropDs50UTC.
    // <br>
    // This function should be called immediately after a successful call to Sgp4PropMse() or Sgp4PropDs50UTC() to retrieve the desired values.
    // <br>
    // It is the caller's responsibility to ensure that the array passed in the destArray parameter is large enough to hold the requested values. The required size can be found by looking at the destArray size column of the table below describing valid index values.
    // <br>
    // The destArray Arrangement column lists the order of the elements in the array. It is not necessarily the subscript of the element in the array since this is language-dependent. For example, in C/C++ the first element in every array is the zero-subscripted element. In other programming languages, the subscript of the first element is 1.
    // <br>
    // Note: This function is not thread safe, please use Sgp4PropAll() instead 
    // <br>
    // The table below shows the values for the xf_Sgp4Out parameter:
    // <table>
    // <caption>table</caption>
    // <tr>
    // <td><b>Index</b></td>
    // <td><b>Index Interpretation</b></td>
    // <td><b>DestArray size</b></td>
    // <td><b>DestArray Arrangement</b></td>
    // </tr>
    // <tr><td>1</td><td>Revolution number</td><td>1</td><td><ol><li>Revolution number (based on the Osculating Keplerian
    // Elements)</li></ol></td></tr>
    // <tr><td>2</td><td>Nodal Apogee Perigee</td><td>3</td><td><ol><li>nodal period (minutes)</li><li>apogee
    // (km)</li><li>perigee (km)</li></ol></td></tr>
    // <tr><td>3</td><td>Mean Keplerian Elements</td><td>6</td><td><ol><li>semi-major axis (km)</li><li>eccentricity
    // (unitless)</li><li>inclination (degree)</li><li>mean anomaly (degree)</li><li>right ascension of the ascending node
    // (degree)</li><li>argument of perigee (degree)</li></ol></td></tr>
    // <tr><td>4</td><td>Osculating Keplerian Elements</td><td>6</td><td>Same as Mean Keplerian Elements</td></tr>
    // </table>
    int Sgp4GetPropOut(__int64 satKey, int xf_Sgp4Out, std::vector<double>& destArr) const {
        std::vector<double> destArrBuf(destArr.size());

        int errCode = p_Sgp4GetPropOut(satKey, xf_Sgp4Out, destArrBuf.data());

        destArr = std::move(destArrBuf);
        return errCode;
    }

    // Propagates a satellite, represented by the satKey, to the time expressed in either minutes since epoch or days since 1950, UTC. 
    // All propagation data is returned by this function.
    int Sgp4PropAll(__int64 satKey, int timeType, double timeIn, std::array<double,64>& xa_Sgp4Out) const {

        int errCode = p_Sgp4PropAll(satKey, timeType, timeIn, xa_Sgp4Out.data());

        return errCode;
    }

    // Converts osculating position and velocity vectors to a set of mean Keplerian SGP4 elements.
    // The new position and velocity vectors are the results of using SGP4 propagator to propagate the computed sgp4MeanKep to the time specified in year and day of epoch time. 
    // They should be closely matched with the input osculating position and velocity vectors.
    // 
    // The mean Keplerian elements are SGP4's Brouwer mean motion not SGP's Kozai mean motion.
    // Notes: Even if the function fails, the less acurate results may still be availalbe 
    int Sgp4PosVelToKep(int yr, double day, const std::array<double,3>& pos, const std::array<double,3>& vel, std::array<double,3>& posNew, std::array<double,3>& velNew, std::array<double,6>& xa_kep) const {

        int errCode = p_Sgp4PosVelToKep(yr, day, const_cast<double*>(pos.data()), const_cast<double*>(vel.data()), posNew.data(), velNew.data(), xa_kep.data());

        return errCode;
    }

    // Converts osculating position and velocity vectors to TLE array - allows bstar/bterm, drag values to be used in the conversion if desired
    // The function is similar to Sgp4PosVelToKep but allows the user to specify agom (XP mode) and bstar/bterm values, if desired, to be used in solving for the new Keplerian elements.
    // 
    // The updated elements returned in the xa_tle array is of type SGP and the mean motion is Kozai mean motion.
    // Notes: Even if the function fails, the less acurate results may still be availalbe 
    int Sgp4PosVelToTleArr(const std::array<double,3>& pos, const std::array<double,3>& vel, std::array<double,64>& xa_tle) const {

        int errCode = p_Sgp4PosVelToTleArr(const_cast<double*>(pos.data()), const_cast<double*>(vel.data()), xa_tle.data());

        return errCode;
    }

    // Reepochs a loaded TLE, represented by the satKey, to a new epoch.
    int Sgp4ReepochTLE(__int64 satKey, double reEpochDs50UTC, std::string& line1Out, std::string& line2Out) const {
        char line1OutBuf[513] = {};
        char line2OutBuf[513] = {};

        int errCode = p_Sgp4ReepochTLE(satKey, reEpochDs50UTC, line1OutBuf, line2OutBuf);

        from_c(line1OutBuf, line1Out);
        from_c(line2OutBuf, line2Out);
        return errCode;
    }

    // Reepochs a loaded TLE, represented by the satKey, to a new epoch in Csv format.
    int Sgp4ReepochCsv(__int64 satKey, double reEpochDs50UTC, std::string& csvLine) const {
        char csvLineBuf[513] = {};

        int errCode = p_Sgp4ReepochCsv(satKey, reEpochDs50UTC, csvLineBuf);

        from_c(csvLineBuf, csvLine);
        return errCode;
    }

    // Sets DIRECTORY/FOLDER path to the Sgp4 Open License file if the license file doesn't exist in the current folder or those specified in PATH/LD_LIBRARY_PATH environment
    // Note: This function has been revised since v9.6. It's only needed if the "SGP4_Open_License.txt" isn't located in current folder or those folders specified in PATH/LD_LIBRARY_PATH environment.
    //       Also, this requires a directory/folder, not a file
    void Sgp4SetLicFilePath(const std::string& licFilePath) const {
        char licFilePathBuf[513] = {};
        to_c(licFilePath, licFilePathBuf);

        p_Sgp4SetLicFilePath(licFilePathBuf);

    }

    // Gets the current path to the Sgp4 Open License file
    // Note: This function has been revised since v9.6. It's only needed if the "SGP4_Open_License.txt" isn't located in current folder or those folders specified in PATH/LD_LIBRARY_PATH environment
    void Sgp4GetLicFilePath(std::string& licFilePath) const {
        char licFilePathBuf[513] = {};

        p_Sgp4GetLicFilePath(licFilePathBuf);

        from_c(licFilePathBuf, licFilePath);
    }

    // Generates ephemerides for the input satellite, represented by its satKey, for the specified time span and step size
    // Notes: if arrSize isn't big enough to store all the ephemeris points, the function will exit when the ephemArr reaches
    // that many points (arrSize) and the errCode is set to IDX_ERR_WARN
    int Sgp4GenEphems(__int64 satKey, double startTime, double endTime, double stepSize, int sgp4_ephem, int arrSize, std::vector<std::array<double,7>>& ephemArr, int& genEphemPts) const {
        std::vector<std::array<double,7>> ephemArrBuf(ephemArr.size());

        int errCode = p_Sgp4GenEphems(satKey, startTime, endTime, stepSize, sgp4_ephem, arrSize, reinterpret_cast<double(*)[7]>(ephemArrBuf.data()), &genEphemPts);

        ephemArr = std::move(ephemArrBuf);
        return errCode;
    }

    // Generates ephemerides for the input TLE - in an array format - for the specified time span and step size (OS - in One Step)
    // Notes: <br>
    // - This function takes in TLE data directly and doesn't need to go through loading/geting satKey/initializing steps<br>
    // - if arrSize isn't big enough to store all the ephemeris points, the function will exit when the ephemArr reaches
    //   that many points (arrSize) and the errCode is set to IDX_ERR_WARN
    int Sgp4GenEphems_OS(const std::array<double,64>& xa_tle, double startTime, double endTime, double stepSize, int sgp4_ephem, int arrSize, std::vector<std::array<double,7>>& ephemArr, int& genEphemPts) const {
        std::vector<std::array<double,7>> ephemArrBuf(ephemArr.size());

        int errCode = p_Sgp4GenEphems_OS(const_cast<double*>(xa_tle.data()), startTime, endTime, stepSize, sgp4_ephem, arrSize, reinterpret_cast<double(*)[7]>(ephemArrBuf.data()), &genEphemPts);

        ephemArr = std::move(ephemArrBuf);
        return errCode;
    }

    // Propagates all input satellites, represented by their satKeys, to the time expressed in days since 1950, UTC. 
    int Sgp4PropAllSats(const std::vector<__int64>& satKeys, int numOfSats, double ds50UTC, std::vector<std::array<double,6>>& ephemArr) const {
        std::vector<__int64> satKeysBuf = satKeys;
        std::vector<std::array<double,6>> ephemArrBuf(ephemArr.size());

        int errCode = p_Sgp4PropAllSats(satKeysBuf.data(), numOfSats, ds50UTC, reinterpret_cast<double(*)[6]>(ephemArrBuf.data()));

        ephemArr = std::move(ephemArrBuf);
        return errCode;
    }

    // Provides the native XP equinoctial elements and rates at given time
    int XpGetNativeElts(__int64 satKey, double ds50UTC, std::array<double,6>& xa_eqnx, std::array<double,6>& xa_eqnx_dot) const {

        int errCode = p_XpGetNativeElts(satKey, ds50UTC, xa_eqnx.data(), xa_eqnx_dot.data());

        return errCode;
    }

    // Reepochs to a csv and provides the native XP equinoctial elements and rates
    int XpReepochGetNativeElts(__int64 satKey, double reEpochDs50UTC, std::string& csvLine, std::array<double,6>& xa_eqnx, std::array<double,6>& xa_eqnx_dot) const {
        char csvLineBuf[513] = {};

        int errCode = p_XpReepochGetNativeElts(satKey, reEpochDs50UTC, csvLineBuf, xa_eqnx.data(), xa_eqnx_dot.data());

        from_c(csvLineBuf, csvLine);
        return errCode;
    }


private:
    Sgp4PropApi() : lib(resolveLibraryName()) {loadSymbols();}
    ~Sgp4PropApi() = default;

    using fn_Sgp4Init = int (STDCALL *)(__int64 apAddr);
    using fn_Sgp4GetInfo = void (STDCALL *)(char infoStr[128]);
    using fn_Sgp4LoadFileAll = int (STDCALL *)(char sgp4InputFile[512]);
    using fn_Sgp4SaveFile = int (STDCALL *)(char sgp4File[512], int saveMode, int saveForm);
    using fn_Sgp4InitSat = int (STDCALL *)(__int64 satKey);
    using fn_Sgp4IsSatInit = int (STDCALL *)(__int64 satKey);
    using fn_Sgp4RemoveSat = int (STDCALL *)(__int64 satKey);
    using fn_Sgp4RemoveAllSats = int (STDCALL *)();
    using fn_Sgp4GetCount = int (STDCALL *)();
    using fn_Sgp4PropMse = int (STDCALL *)(__int64 satKey, double mse, double* ds50UTC, double pos[3], double vel[3], double llh[3]);
    using fn_Sgp4PropDs50UTC = int (STDCALL *)(__int64 satKey, double ds50UTC, double* mse, double pos[3], double vel[3], double llh[3]);
    using fn_Sgp4PropDs50UtcPosVel = int (STDCALL *)(__int64 satKey, double ds50UTC, double pos[3], double vel[3]);
    using fn_Sgp4PropDs50UtcLLH = int (STDCALL *)(__int64 satKey, double ds50UTC, double llh[3]);
    using fn_Sgp4PropDs50UtcPos = int (STDCALL *)(__int64 satKey, double ds50UTC, double pos[3]);
    using fn_Sgp4GetPropOut = int (STDCALL *)(__int64 satKey, int xf_Sgp4Out, double destArr[]);
    using fn_Sgp4PropAll = int (STDCALL *)(__int64 satKey, int timeType, double timeIn, double xa_Sgp4Out[64]);
    using fn_Sgp4PosVelToKep = int (STDCALL *)(int yr, double day, double pos[3], double vel[3], double posNew[3], double velNew[3], double xa_kep[6]);
    using fn_Sgp4PosVelToTleArr = int (STDCALL *)(double pos[3], double vel[3], double xa_tle[64]);
    using fn_Sgp4ReepochTLE = int (STDCALL *)(__int64 satKey, double reEpochDs50UTC, char line1Out[512], char line2Out[512]);
    using fn_Sgp4ReepochCsv = int (STDCALL *)(__int64 satKey, double reEpochDs50UTC, char csvLine[512]);
    using fn_Sgp4SetLicFilePath = void (STDCALL *)(char licFilePath[512]);
    using fn_Sgp4GetLicFilePath = void (STDCALL *)(char licFilePath[512]);
    using fn_Sgp4GenEphems = int (STDCALL *)(__int64 satKey, double startTime, double endTime, double stepSize, int sgp4_ephem, int arrSize, double ephemArr[][7], int* genEphemPts);
    using fn_Sgp4GenEphems_OS = int (STDCALL *)(double xa_tle[64], double startTime, double endTime, double stepSize, int sgp4_ephem, int arrSize, double ephemArr[][7], int* genEphemPts);
    using fn_Sgp4PropAllSats = int (STDCALL *)(__int64 satKeys[], int numOfSats, double ds50UTC, double ephemArr[][6]);
    using fn_XpGetNativeElts = int (STDCALL *)(__int64 satKey, double ds50UTC, double xa_eqnx[6], double xa_eqnx_dot[6]);
    using fn_XpReepochGetNativeElts = int (STDCALL *)(__int64 satKey, double reEpochDs50UTC, char csvLine[512], double xa_eqnx[6], double xa_eqnx_dot[6]);

    fn_Sgp4Init p_Sgp4Init{};
    fn_Sgp4GetInfo p_Sgp4GetInfo{};
    fn_Sgp4LoadFileAll p_Sgp4LoadFileAll{};
    fn_Sgp4SaveFile p_Sgp4SaveFile{};
    fn_Sgp4InitSat p_Sgp4InitSat{};
    fn_Sgp4IsSatInit p_Sgp4IsSatInit{};
    fn_Sgp4RemoveSat p_Sgp4RemoveSat{};
    fn_Sgp4RemoveAllSats p_Sgp4RemoveAllSats{};
    fn_Sgp4GetCount p_Sgp4GetCount{};
    fn_Sgp4PropMse p_Sgp4PropMse{};
    fn_Sgp4PropDs50UTC p_Sgp4PropDs50UTC{};
    fn_Sgp4PropDs50UtcPosVel p_Sgp4PropDs50UtcPosVel{};
    fn_Sgp4PropDs50UtcLLH p_Sgp4PropDs50UtcLLH{};
    fn_Sgp4PropDs50UtcPos p_Sgp4PropDs50UtcPos{};
    fn_Sgp4GetPropOut p_Sgp4GetPropOut{};
    fn_Sgp4PropAll p_Sgp4PropAll{};
    fn_Sgp4PosVelToKep p_Sgp4PosVelToKep{};
    fn_Sgp4PosVelToTleArr p_Sgp4PosVelToTleArr{};
    fn_Sgp4ReepochTLE p_Sgp4ReepochTLE{};
    fn_Sgp4ReepochCsv p_Sgp4ReepochCsv{};
    fn_Sgp4SetLicFilePath p_Sgp4SetLicFilePath{};
    fn_Sgp4GetLicFilePath p_Sgp4GetLicFilePath{};
    fn_Sgp4GenEphems p_Sgp4GenEphems{};
    fn_Sgp4GenEphems_OS p_Sgp4GenEphems_OS{};
    fn_Sgp4PropAllSats p_Sgp4PropAllSats{};
    fn_XpGetNativeElts p_XpGetNativeElts{};
    fn_XpReepochGetNativeElts p_XpReepochGetNativeElts{};

    SharedLibrary lib;

    static const char* resolveLibraryName() {
#ifdef _WIN32
        return "Sgp4Prop.dll";
#elif __APPLE__
        return "libsgp4prop.dylib";
#else
        return "libsgp4prop.so";
#endif
    }

    void loadSymbols() {
        p_Sgp4Init = lib.load<fn_Sgp4Init>("Sgp4Init");
        p_Sgp4GetInfo = lib.load<fn_Sgp4GetInfo>("Sgp4GetInfo");
        p_Sgp4LoadFileAll = lib.load<fn_Sgp4LoadFileAll>("Sgp4LoadFileAll");
        p_Sgp4SaveFile = lib.load<fn_Sgp4SaveFile>("Sgp4SaveFile");
        p_Sgp4InitSat = lib.load<fn_Sgp4InitSat>("Sgp4InitSat");
        p_Sgp4IsSatInit = lib.load<fn_Sgp4IsSatInit>("Sgp4IsSatInit");
        p_Sgp4RemoveSat = lib.load<fn_Sgp4RemoveSat>("Sgp4RemoveSat");
        p_Sgp4RemoveAllSats = lib.load<fn_Sgp4RemoveAllSats>("Sgp4RemoveAllSats");
        p_Sgp4GetCount = lib.load<fn_Sgp4GetCount>("Sgp4GetCount");
        p_Sgp4PropMse = lib.load<fn_Sgp4PropMse>("Sgp4PropMse");
        p_Sgp4PropDs50UTC = lib.load<fn_Sgp4PropDs50UTC>("Sgp4PropDs50UTC");
        p_Sgp4PropDs50UtcPosVel = lib.load<fn_Sgp4PropDs50UtcPosVel>("Sgp4PropDs50UtcPosVel");
        p_Sgp4PropDs50UtcLLH = lib.load<fn_Sgp4PropDs50UtcLLH>("Sgp4PropDs50UtcLLH");
        p_Sgp4PropDs50UtcPos = lib.load<fn_Sgp4PropDs50UtcPos>("Sgp4PropDs50UtcPos");
        p_Sgp4GetPropOut = lib.load<fn_Sgp4GetPropOut>("Sgp4GetPropOut");
        p_Sgp4PropAll = lib.load<fn_Sgp4PropAll>("Sgp4PropAll");
        p_Sgp4PosVelToKep = lib.load<fn_Sgp4PosVelToKep>("Sgp4PosVelToKep");
        p_Sgp4PosVelToTleArr = lib.load<fn_Sgp4PosVelToTleArr>("Sgp4PosVelToTleArr");
        p_Sgp4ReepochTLE = lib.load<fn_Sgp4ReepochTLE>("Sgp4ReepochTLE");
        p_Sgp4ReepochCsv = lib.load<fn_Sgp4ReepochCsv>("Sgp4ReepochCsv");
        p_Sgp4SetLicFilePath = lib.load<fn_Sgp4SetLicFilePath>("Sgp4SetLicFilePath");
        p_Sgp4GetLicFilePath = lib.load<fn_Sgp4GetLicFilePath>("Sgp4GetLicFilePath");
        p_Sgp4GenEphems = lib.load<fn_Sgp4GenEphems>("Sgp4GenEphems");
        p_Sgp4GenEphems_OS = lib.load<fn_Sgp4GenEphems_OS>("Sgp4GenEphems_OS");
        p_Sgp4PropAllSats = lib.load<fn_Sgp4PropAllSats>("Sgp4PropAllSats");
        p_XpGetNativeElts = lib.load<fn_XpGetNativeElts>("XpGetNativeElts");
        p_XpReepochGetNativeElts = lib.load<fn_XpReepochGetNativeElts>("XpReepochGetNativeElts");
    }
};

// ========================= End of auto generated code ==========================

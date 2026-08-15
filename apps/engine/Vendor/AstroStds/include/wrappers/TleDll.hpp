// This wrapper file was generated automatically by the GenDllWrappers program.
#pragma once

#include "utils.hpp"
#include <string>
#include <vector>
#include <array>
#include <cstring>

class TleApi {
public:
    static TleApi& instance() {
        static TleApi inst;
        return inst;
    }
    TleApi(const TleApi&)              = delete;
    TleApi(TleApi&&)                   = delete;
    TleApi& operator = (const TleApi&) = delete;
    TleApi& operator = (TleApi&&)      = delete;


    // TLE types (TLE ephemeris types) - They are different than ELTTYPE
    // TLE SGP elset (Kozai mean motion)
    static constexpr int TLETYPE_SGP  = 0;
    // TLE SGP4 elset (Brouwer mean motion)
    static constexpr int TLETYPE_SGP4 = 2;
    // TLE PPT3 elset (Kozai mean motion)
    static constexpr int TLETYPE_PPT3 = 3;
    // TLE SGP4-XP elset (Brouwer mean motion)
    static constexpr int TLETYPE_XP   = 4;
    // TLE SP elset (osculating elements)
    static constexpr int TLETYPE_SP   = 6;

    // Indexes of TLE data fields
    // Satellite number
    static constexpr int XF_TLE_SATNUM     =  1;
    // Security classification U: unclass, C: confidential, S: Secret
    static constexpr int XF_TLE_CLASS      =  2;
    // Satellite name A8
    static constexpr int XF_TLE_SATNAME    =  3;
    // Satellite's epoch time "YYYYJJJ.jjjjjjjj"
    static constexpr int XF_TLE_EPOCH      =  4;
    // GP B* drag term (1/er)  (not the same as XF_TLE_BTERM)
    static constexpr int XF_TLE_BSTAR      =  5;
    // Satellite ephemeris type: 0=SGP, 2=SGP4, 4=SGP4-XP, 6=SP
    static constexpr int XF_TLE_EPHTYPE    =  6;
    // Element set number
    static constexpr int XF_TLE_ELSETNUM   =  7;
    // Orbit inclination (deg)
    static constexpr int XF_TLE_INCLI      =  8;
    // Right ascension of asending node (deg)
    static constexpr int XF_TLE_NODE       =  9;
    // Eccentricity
    static constexpr int XF_TLE_ECCEN      = 10;
    // Argument of perigee (deg)
    static constexpr int XF_TLE_OMEGA      = 11;
    // Mean anomaly (deg)
    static constexpr int XF_TLE_MNANOM     = 12;
    // Mean motion (rev/day) (ephType=0: Kozai, ephType=2: Brouwer)
    static constexpr int XF_TLE_MNMOTN     = 13;
    // Revolution number at epoch
    static constexpr int XF_TLE_REVNUM     = 14;

    // GP Mean motion derivative (rev/day**2 /2)
    static constexpr int XF_TLE_NDOT       = 15;
    // GP Mean motion second derivative (rev/day**3 /6)
    static constexpr int XF_TLE_NDOTDOT    = 16;
    // Solar radiation pressure GP (m2/kg)
    static constexpr int XF_TLE_AGOMGP     = 16;

    // SP Radiation Pressure Coefficient
    static constexpr int XF_TLE_SP_AGOM    =  5;
    // SP ballistic coefficient (m2/kg)
    static constexpr int XF_TLE_SP_BTERM   = 15;
    // SP outgassing parameter (km/s2)
    static constexpr int XF_TLE_SP_OGPARM  = 16;

    // Original satellite number
    static constexpr int XF_TLE_ORGSATNUM  = 17;
    // GP ballistic coefficient (m2/kg) (not the same as XF_TLE_BSTAR)
    static constexpr int XF_TLE_BTERM      = 18;
    // Time of last observation relative to epoch +/- fractional days
    static constexpr int XF_TLE_OBSTIME    = 19;
    // Last calculated error growth rate (km/day)
    static constexpr int XF_TLE_EGR        = 20;
    // Last calculated energy dissipation rate (w/kg)
    static constexpr int XF_TLE_EDR        = 21;
    // Median Vismag
    static constexpr int XF_TLE_VISMAG     = 22;
    // Median RCS - diameter in centimeters (cm)
    static constexpr int XF_TLE_RCS        = 23;
    // Object Type (Payload, Rocket Body, Platform, Debris, Unknown)
    static constexpr int XF_TLE_OBJTYPE    = 24;
    // Satellite name A12 (upto 12 character long)
    static constexpr int XF_TLE_SATNAME_12 = 25;


    // Indexes of TLE numerical data in an array
    // Line 1
    // Satellite number
    static constexpr int XA_TLE_SATNUM        =  0;
    // Satellite's epoch time in DS50UTC
    static constexpr int XA_TLE_EPOCH         =  1;
    // GP Mean motion derivative (rev/day**2 /2)
    static constexpr int XA_TLE_NDOT          =  2;
    // GP Mean motion second derivative (rev/day**3 /6)
    static constexpr int XA_TLE_NDOTDOT       =  3;
    // GP B* drag term (1/er)
    static constexpr int XA_TLE_BSTAR         =  4;
    // Satellite ephemeris type: 0=SGP, 2=SGP4, 4=SGP4-XP, 6=SP
    static constexpr int XA_TLE_EPHTYPE       =  5;

    // Line 2
    // Orbit inclination (deg)
    static constexpr int XA_TLE_INCLI         = 20;
    // Right ascension of asending node (deg)
    static constexpr int XA_TLE_NODE          = 21;
    // Eccentricity
    static constexpr int XA_TLE_ECCEN         = 22;
    // Argument of perigee (deg)
    static constexpr int XA_TLE_OMEGA         = 23;
    // Mean anomaly (deg)
    static constexpr int XA_TLE_MNANOM        = 24;
    // Mean motion (rev/day) (ephType=0, 4: Kozai, ephType=2: Brouwer)
    static constexpr int XA_TLE_MNMOTN        = 25;
    // Revolution number at epoch
    static constexpr int XA_TLE_REVNUM        = 26;
    // Element set number
    static constexpr int XA_TLE_ELSETNUM      = 30;

    // CSV (or TLE-XP, ephemType=4) specific fields
    // Original satellite number
    static constexpr int XA_TLE_ORGSATNUM     = 31;
    // SP/SGP4-XP ballistic coefficient (m2/kg)
    static constexpr int XA_TLE_BTERM         = 32;
    // Time of last observation relative to epoch +/- fractional days
    static constexpr int XA_TLE_OBSTIME       = 33;
    // Last calculated error growth rate (km/day)
    static constexpr int XA_TLE_EGR           = 34;
    // Last calculated energy dissipation rate (w/kg)
    static constexpr int XA_TLE_EDR           = 35;
    // Median Vismag
    static constexpr int XA_TLE_VISMAG        = 36;
    // Median RCS - diameter in centimeters (cm)
    static constexpr int XA_TLE_RCS           = 37;

    // CSV (or TLE-XP, ephemType=4)
    // Solar Radiation Pressure Coefficient GP (m2/kg)
    static constexpr int XA_TLE_AGOMGP        = 38;


    // SP specific fields
    // SP ballistic coefficient (m2/kg)
    static constexpr int XA_TLE_SP_BTERM      =  2;
    // SP outgassing parameter (km/s2)
    static constexpr int XA_TLE_SP_OGPARM     =  3;
    // SP Radiation Pressure Coefficient
    static constexpr int XA_TLE_SP_AGOM       =  4;

    static constexpr int XA_TLE_SIZE          = 64;

    // Indexes of TLE text data in an array of chars
    // Security classification of line 1 and line 2
    static constexpr int XS_TLE_SECCLASS_1   =  0;
    // Satellite name
    static constexpr int XS_TLE_SATNAME_12   =  1;
    // Object Type (Payload, Rocket Body, Platform, Debris, Unknown) - csv only
    static constexpr int XS_TLE_OBJTYPE_11   = 13;

    static constexpr int XS_TLE_SIZE         = 512;

    // TLE's text data fields - new convention (start index, string length)
    // Security classification of line 1 and line 2
    static constexpr int XS_TLE_SECCLASS_0_1  =  0;
    // Satellite name
    static constexpr int XS_TLE_SATNAME_1_12  =  1;
    // Object Type (Payload, Rocket Body, Platform, Debris, Unknown) - csv only
    static constexpr int XS_TLE_OBJTYPE_13_1  = 13;

    static constexpr int XS_TLE_LENGTH        = 512;

    // Indexes of different TLE file's formats
    // Original TLE format
    static constexpr int XF_TLEFORM_ORG    =  0;
    // CSV format
    static constexpr int XF_TLEFORM_CSV    =  1;


    // Notes: This function has been deprecated since v9.0.    
    // Initializes Tle DLL for use in the program.
    // If this function returns an error, it is recommended that you stop the program immediately.
    // 
    // An error will occur if you forget to load and initialize all the prerequisite DLLs, as listed in the DLL Prerequisites section of the accompanying documentation, before using this DLL.
    int TleInit(__int64 apAddr) const {

        int errCode = p_TleInit(apAddr);

        return errCode;
    }

    // Returns the information about the Tle DLL.
    // The returned string provides information about the version number, build date, and the platform of the Tle DLL. 
    void TleGetInfo(std::string& infoStr) const {
        char infoStrBuf[129] = {};

        p_TleGetInfo(infoStrBuf);

        from_c(infoStrBuf, infoStr);
    }

    // Loads TLEs (satellites) contained in a text file into the TLE DLL's binary tree.
    // You may use this function repeatedly to load TLEs from different input files. However, only unique satKeys are loaded. Duplicated TLEs won't be stored. 
    // 
    // TLEs can be included directly in the specified file, or they can be read from a separate file identified with "ELTFIL=[path\filename]" or "VECFIL=[path\filename]".
    // 
    // The input file is read in two passes. The function first looks for "ELTFIL=" and "VECFIL=" lines, then it looks for TLEs which were included directly. The result of this is that data entered using both methods will be processed, but the "ELTFIL=" and "VECFIL=" data will be processed first.
    int TleLoadFile(const std::string& tleFile) const {
        char tleFileBuf[513] = {};
        to_c(tleFile, tleFileBuf);

        int errCode = p_TleLoadFile(tleFileBuf);

        return errCode;
    }

    // Saves currently loaded TLEs to a file. 
    // In append mode, if the specified file does not exist it will be created.
    // If you call this routine immediately after TleLoadFile(), the TLE contents in the two files should be the same (minus duplicated TLE's or bad TLE's).
    // 
    // The purpose of this function is to save the current state of the loaded TLE's, usually used in GUI applications, for future use.
    int TleSaveFile(const std::string& tleFile, int saveMode, int xf_tleForm) const {
        char tleFileBuf[513] = {};
        to_c(tleFile, tleFileBuf);

        int errCode = p_TleSaveFile(tleFileBuf, saveMode, xf_tleForm);

        return errCode;
    }

    // Removes a TLE represented by the satKey from memory. 
    // If the users enter an invalid satKey (a non-existing satKey), the function will return a non-zero value indicating an error.
    int TleRemoveSat(__int64 satKey) const {

        int errCode = p_TleRemoveSat(satKey);

        return errCode;
    }

    // Removes all the TLEs from memory.
    int TleRemoveAllSats() const {

        int errCode = p_TleRemoveAllSats();

        return errCode;
    }

    // Returns the number of TLEs currently loaded. 
    // See TleGetLoaded for an example.
    // This function is useful for dynamically allocating memory for the array that is passed to the function TleGetLoaded().
    int TleGetCount() const {

        int numSats = p_TleGetCount();

        return numSats;
    }

    // Retrieves all of the currently loaded satKeys. These satKeys can be used to access the internal data for the TLE's.
    // It is recommended that TleGetCount() be  used to determine how many satellites are currently loaded. This value can then be used to dynamically allocate an array to hold the satKeys.
    // 
    // If you are going to pass a statically allocated array to this function, ensure it is large enough to hold all of the returned satKeys.
    void TleGetLoaded(int order, std::vector<__int64>& satKeys) const {
        std::vector<__int64> satKeysBuf(satKeys.size());

        p_TleGetLoaded(order, satKeysBuf.data());

        satKeys = std::move(satKeysBuf);
    }

    // Adds a TLE (satellite), using its directly specified first and second lines. 
    // The function will indicate an error if the specified two line element set corresponds to a satellite that is already in memory.
    // 
    // This function can be called repeatedly to add many TLEs, one at a time.
    __int64 TleAddSatFrLines(const std::string& line1, const std::string& line2) const {
        char line1Buf[513] = {};
        to_c(line1, line1Buf);
        char line2Buf[513] = {};
        to_c(line2, line2Buf);

        __int64 satKey = p_TleAddSatFrLines(line1Buf, line2Buf);

        return satKey;
    }

    // This function is similar to TleAddSatFrLines but designed to be used in Matlab.
    // Matlab doesn't correctly return the 19-digit satellite key using TleAddSatFrLines. This method is an alternative way to return the satKey output.
    void TleAddSatFrLinesML(const std::string& line1, const std::string& line2, __int64& satKey) const {
        char line1Buf[513] = {};
        to_c(line1, line1Buf);
        char line2Buf[513] = {};
        to_c(line2, line2Buf);

        p_TleAddSatFrLinesML(line1Buf, line2Buf, &satKey);

    }

    // Adds a TLE (satellite), using its CSV string format. 
    __int64 TleAddSatFrCsv(const std::string& csvLine) const {
        char csvLineBuf[513] = {};
        to_c(csvLine, csvLineBuf);

        __int64 satKey = p_TleAddSatFrCsv(csvLineBuf);

        return satKey;
    }

    // This function is similar to TleAddSatFrCsv but designed to be used in Matlab.
    void TleAddSatFrCsvML(const std::string& csvLine, __int64& satKey) const {
        char csvLineBuf[513] = {};
        to_c(csvLine, csvLineBuf);

        p_TleAddSatFrCsvML(csvLineBuf, &satKey);

    }

    // Adds a GP TLE using its individually provided field values. 
    // The function will indicate an error if the specified two line element set corresponds to a satellite that is already in memory.
    // 
    // This function can be called repeatedly to add many satellites (one satellite at a time).
    // 
    // SGP satellites (ephType = 0) use Kozai mean motion. SGP4 satellites (ephType = 2) use Brouwer mean motion.
    __int64 TleAddSatFrFieldsGP(int satNum, char secClass, const std::string& satName, int epochYr, double epochDays, double bstar, int ephType, int elsetNum, double incli, double node, double eccen, double omega, double mnAnomaly, double mnMotion, int revNum) const {
        char satNameBuf[9] = {};
        to_c(satName, satNameBuf);

        __int64 satKey = p_TleAddSatFrFieldsGP(satNum, secClass, satNameBuf, epochYr, epochDays, bstar, ephType, elsetNum, incli, node, eccen, omega, mnAnomaly, mnMotion, revNum);

        return satKey;
    }

    // This function is similar to TleAddSatFrFieldsGP but includes nDotO2 and n2DotO6. 
    // nDotO2 and n2DotO6 values are not used in the SGP4 propagator. However, some users still want to preserve the integrity of all input data.
    __int64 TleAddSatFrFieldsGP2(int satNum, char secClass, const std::string& satName, int epochYr, double epochDays, double bstar, int ephType, int elsetNum, double incli, double node, double eccen, double omega, double mnAnomaly, double mnMotion, int revNum, double nDotO2, double n2DotO6) const {
        char satNameBuf[9] = {};
        to_c(satName, satNameBuf);

        __int64 satKey = p_TleAddSatFrFieldsGP2(satNum, secClass, satNameBuf, epochYr, epochDays, bstar, ephType, elsetNum, incli, node, eccen, omega, mnAnomaly, mnMotion, revNum, nDotO2, n2DotO6);

        return satKey;
    }

    // This function is similar to TleAddSatFrFieldsGP2 but designed to be used in Matlab. 
    // Matlab doesn't seem to correctly return the 19-digit satellite key using TleAddSatFrFieldsGP2. This method is an alternative way to return the satKey output.
    void TleAddSatFrFieldsGP2ML(int satNum, char secClass, const std::string& satName, int epochYr, double epochDays, double bstar, int ephType, int elsetNum, double incli, double node, double eccen, double omega, double mnAnomaly, double mnMotion, int revNum, double nDotO2, double n2DotO6, __int64& satKey) const {
        char satNameBuf[9] = {};
        to_c(satName, satNameBuf);

        p_TleAddSatFrFieldsGP2ML(satNum, secClass, satNameBuf, epochYr, epochDays, bstar, ephType, elsetNum, incli, node, eccen, omega, mnAnomaly, mnMotion, revNum, nDotO2, n2DotO6, &satKey);

    }

    // Updates a GP satellite's data in memory by providing its individual field values. Note: satNum, year, day, and ephtype can't be updated.
    // The satellite's unique key will not be changed by this function. If you specify a satKey that does not correspond to a currently loaded satellite, the function will indicate an error.
    // 
    // Remember to use the correct mean motion depending on the satellite's ephType.
    int TleUpdateSatFrFieldsGP(__int64 satKey, char secClass, const std::string& satName, double bstar, int elsetNum, double incli, double node, double eccen, double omega, double mnAnomaly, double mnMotion, int revNum) const {
        char satNameBuf[9] = {};
        to_c(satName, satNameBuf);

        int errCode = p_TleUpdateSatFrFieldsGP(satKey, secClass, satNameBuf, bstar, elsetNum, incli, node, eccen, omega, mnAnomaly, mnMotion, revNum);

        return errCode;
    }

    // This function is similar to TleUpdateSatFrFieldsGP but includes nDotO2 and n2DotO6. Note: satNum, year, day, and ephtype can't be updated.
    // nDotO2 and n2DotO6 values are not used in the SGP4 propagator. However, some users still want to preserve the integrity of all input data.
    int TleUpdateSatFrFieldsGP2(__int64 satKey, char secClass, const std::string& satName, double bstar, int elsetNum, double incli, double node, double eccen, double omega, double mnAnomaly, double mnMotion, int revNum, double nDotO2, double n2DotO6) const {
        char satNameBuf[9] = {};
        to_c(satName, satNameBuf);

        int errCode = p_TleUpdateSatFrFieldsGP2(satKey, secClass, satNameBuf, bstar, elsetNum, incli, node, eccen, omega, mnAnomaly, mnMotion, revNum, nDotO2, n2DotO6);

        return errCode;
    }

    // Adds an SP satellite using the individually provided field values.
    // Only applies to SP propagator.
    __int64 TleAddSatFrFieldsSP(int satNum, char secClass, const std::string& satName, int epochYr, double epochDays, double bTerm, double ogParm, double agom, int elsetNum, double incli, double node, double eccen, double omega, double mnAnomaly, double mnMotion, int revNum) const {
        char satNameBuf[9] = {};
        to_c(satName, satNameBuf);

        __int64 satKey = p_TleAddSatFrFieldsSP(satNum, secClass, satNameBuf, epochYr, epochDays, bTerm, ogParm, agom, elsetNum, incli, node, eccen, omega, mnAnomaly, mnMotion, revNum);

        return satKey;
    }

    // This function is similar to TleAddSatFrFieldsSP but designed to be used in Matlab.
    // Only applies to SP propagator.
    // Matlab doesn't correctly return the 19-digit satellite key using TleAddSatFrFieldsSP. This method is an alternative way to return the satKey output.
    void TleAddSatFrFieldsSPML(int satNum, char secClass, const std::string& satName, int epochYr, double epochDays, double bTerm, double ogParm, double agom, int elsetNum, double incli, double node, double eccen, double omega, double mnAnomaly, double mnMotion, int revNum, __int64& satKey) const {
        char satNameBuf[9] = {};
        to_c(satName, satNameBuf);

        p_TleAddSatFrFieldsSPML(satNum, secClass, satNameBuf, epochYr, epochDays, bTerm, ogParm, agom, elsetNum, incli, node, eccen, omega, mnAnomaly, mnMotion, revNum, &satKey);

    }

    // Updates an SP satellite's data in memory using its individually provided field values. Note: satNum, year, day, and ephtype can't be updated.
    // Only applies to SP propagator.
    // The satellite's unique key will not be changed by this function. If you specify a satKey that does not correspond to a currently loaded TLE, the function will indicate an error. 
    int TleUpdateSatFrFieldsSP(__int64 satKey, char secClass, const std::string& satName, double bterm, double ogParm, double agom, int elsetNum, double incli, double node, double eccen, double omega, double mnAnomaly, double mnMotion, int revNum) const {
        char satNameBuf[9] = {};
        to_c(satName, satNameBuf);

        int errCode = p_TleUpdateSatFrFieldsSP(satKey, secClass, satNameBuf, bterm, ogParm, agom, elsetNum, incli, node, eccen, omega, mnAnomaly, mnMotion, revNum);

        return errCode;
    }

    // Updates the value of a field of a TLE. This function can be used for both GP and SP satellites.
    // <br>
    // The table below indicates which index values correspond to which fields. Make sure to use the appropriate field index for GP TLEs and SP TLEs.  For indexes 5, 15 and 16, the interpretation depends on the ephemeris type of the TLE.
    // satNum (1), Epoch (4), and Ephemeris Type (5) cannot be altered.
    // <table>
    // <caption>table</caption>
    // <tr>
    // <td>index</td>
    // <td>index Interpretation</td>
    // </tr>
    // <tr><td>1</td><td>Satellite number</td></tr>
    // <tr><td>2</td><td>Security classification</td></tr>
    // <tr><td>3</td><td>Satellite international designator</td></tr>
    // <tr><td>4</td><td>Epoch</td></tr>
    // <tr><td>5</td><td>Ephemeris type = 0,2: B* drag term (1/er) <br>Ephemeris type = 6   : SP radiation pressure
    // coefficient agom (m2/kg)</td></tr>
    // <tr><td>6</td><td>Ephemeris type</td></tr>
    // <tr><td>7</td><td>Element set number</td></tr>
    // <tr><td>8</td><td>Orbit inclination (degrees)</td></tr>
    // <tr><td>9</td><td>Right ascension of ascending node (degrees)</td></tr>
    // <tr><td>10</td><td>Eccentricity</td></tr>
    // <tr><td>11</td><td>Argument of perigee (degrees)</td></tr>
    // <tr><td>12</td><td>Mean anomaly (degrees)</td></tr>
    // <tr><td>13</td><td>Mean motion (rev/day)</td></tr>
    // <tr><td>14</td><td>Revolution number at epoch</td></tr>
    // <tr><td>15</td><td>Ephemeris type = 0: SGP mean motion derivative (rev/day /2) or <br>Ephemeris type = 6: SP
    // ballistic coefficient (m2/kg)</td></tr>
    // <tr><td>16</td><td>Ephemeris type = 0: SGP mean motion second derivative (rev/day**2 /6) or <br>Ephemeris type = 6:
    // SP Outgassing parameter/Thrust Acceleration (km/s2)</td></tr>
    // </table>
    int TleSetField(__int64 satKey, int xf_Tle, const std::string& valueStr) const {
        char valueStrBuf[513] = {};
        to_c(valueStr, valueStrBuf);

        int errCode = p_TleSetField(satKey, xf_Tle, valueStrBuf);

        return errCode;
    }

    // Retrieves the value of a specific field of a TLE. 
    // <br>
    // The table below indicates which index values correspond to which fields. Make sure to use the appropriate field index for GP TLEs and SP TLEs.  For indexes 5, 15 and 16, the interpretation depends on the ephemeris type of the TLE.
    // <table>
    // <caption>table</caption>
    // <tr>
    // <td>index</td>
    // <td>index Interpretation</td>
    // </tr>
    // <tr><td>1</td><td>Satellite number</td></tr>
    // <tr><td>2</td><td>Security classification</td></tr>
    // <tr><td>3</td><td>Satellite international designator</td></tr>
    // <tr><td>4</td><td>Epoch</td></tr>
    // <tr><td>5</td><td>Ephemeris type = 0,2: B* drag term (1/er) <br>Ephemeris type = 6   : SP radiation pressure
    // coefficient agom (m2/kg)</td></tr>
    // <tr><td>6</td><td>Ephemeris type</td></tr>
    // <tr><td>7</td><td>Element set number</td></tr>
    // <tr><td>8</td><td>Orbit inclination (degrees)</td></tr>
    // <tr><td>9</td><td>Right ascension of ascending node (degrees)</td></tr>
    // <tr><td>10</td><td>Eccentricity</td></tr>
    // <tr><td>11</td><td>Argument of perigee (degrees)</td></tr>
    // <tr><td>12</td><td>Mean anomaly (degrees)</td></tr>
    // <tr><td>13</td><td>Mean motion (rev/day)</td></tr>
    // <tr><td>14</td><td>Revolution number at epoch</td></tr>
    // <tr><td>15</td><td>Ephemeris type = 0: SGP mean motion derivative (rev/day /2) or <br>Ephemeris type = 6: SP
    // ballistic coefficient (m2/kg)</td></tr>
    // <tr><td>16</td><td>Ephemeris type = 0: SGP mean motion second derivative (rev/day**2 /6) or <br>Ephemeris type = 6:
    // SP Outgassing parameter/Thrust Acceleration (km/s2)</td></tr>
    // </table>
    int TleGetField(__int64 satKey, int xf_Tle, std::string& valueStr) const {
        char valueStrBuf[513] = {};

        int errCode = p_TleGetField(satKey, xf_Tle, valueStrBuf);

        from_c(valueStrBuf, valueStr);
        return errCode;
    }

    // Retrieves all of the data for a GP satellite in a single function call. 
    // This function only works for GP satellites. The field values are placed in the corresponding parameters of the function. 
    int TleGetAllFieldsGP(__int64 satKey, int& satNum, char& secClass, std::string& satName, int& epochYr, double& epochDays, double& bstar, int& ephType, int& elsetNum, double& incli, double& node, double& eccen, double& omega, double& mnAnomaly, double& mnMotion, int& revNum) const {
        char satNameBuf[9] = {};

        int errCode = p_TleGetAllFieldsGP(satKey, &satNum, &secClass, satNameBuf, &epochYr, &epochDays, &bstar, &ephType, &elsetNum, &incli, &node, &eccen, &omega, &mnAnomaly, &mnMotion, &revNum);

        from_c(satNameBuf, satName);
        return errCode;
    }

    // Retrieves all of the data (including nDotO2 and n2DotO6) for a GP satellite in a single function call. 
    // This function is similar to TleGetAllFieldsGP but also includes nDotO2 and n2DotO6. 
    // This function only works for GP satellites. The field values are placed in the corresponding parameters of the function. 
    int TleGetAllFieldsGP2(__int64 satKey, int& satNum, char& secClass, std::string& satName, int& epochYr, double& epochDays, double& bstar, int& ephType, int& elsetNum, double& incli, double& node, double& eccen, double& omega, double& mnAnomaly, double& mnMotion, int& revNum, double& nDotO2, double& n2DotO6) const {
        char satNameBuf[9] = {};

        int errCode = p_TleGetAllFieldsGP2(satKey, &satNum, &secClass, satNameBuf, &epochYr, &epochDays, &bstar, &ephType, &elsetNum, &incli, &node, &eccen, &omega, &mnAnomaly, &mnMotion, &revNum, &nDotO2, &n2DotO6);

        from_c(satNameBuf, satName);
        return errCode;
    }

    // Retrieves all of the data for an SP satellite in a single function call.
    // Only applies to SP propagator.
    // This function only works for SP satellites. The field values are placed in the corresponding parameters of the function. 
    int TleGetAllFieldsSP(__int64 satKey, int& satNum, char& secClass, std::string& satName, int& epochYr, double& epochDays, double& bTerm, double& ogParm, double& agom, int& elsetNum, double& incli, double& node, double& eccen, double& omega, double& mnAnomaly, double& mnMotion, int& revNum) const {
        char satNameBuf[9] = {};

        int errCode = p_TleGetAllFieldsSP(satKey, &satNum, &secClass, satNameBuf, &epochYr, &epochDays, &bTerm, &ogParm, &agom, &elsetNum, &incli, &node, &eccen, &omega, &mnAnomaly, &mnMotion, &revNum);

        from_c(satNameBuf, satName);
        return errCode;
    }

    // Parses GP data from the input first and second lines of a two line element set or a CSV Tle.
    // This function only parses data from the input TLE but DOES NOT load/add the input TLE to memory.
    int TleParseGP(const std::string& line1, const std::string& line2, int& satNum, char& secClass, std::string& satName, int& epochYr, double& epochDays, double& nDotO2, double& n2DotO6, double& bstar, int& ephType, int& elsetNum, double& incli, double& node, double& eccen, double& omega, double& mnAnomaly, double& mnMotion, int& revNum) const {
        char line1Buf[513] = {};
        to_c(line1, line1Buf);
        char line2Buf[513] = {};
        to_c(line2, line2Buf);
        char satNameBuf[9] = {};

        int errCode = p_TleParseGP(line1Buf, line2Buf, &satNum, &secClass, satNameBuf, &epochYr, &epochDays, &nDotO2, &n2DotO6, &bstar, &ephType, &elsetNum, &incli, &node, &eccen, &omega, &mnAnomaly, &mnMotion, &revNum);

        from_c(satNameBuf, satName);
        return errCode;
    }

    // Parses GP data from the input first and second lines of a two line element set or a CSV tle and store that data back into the output parameters.
    // This function only parses data from the input TLE but DOES NOT load/add the input TLE to memory.
    int TleLinesToArray(const std::string& line1, const std::string& line2, std::array<double,64>& xa_tle, std::string& xs_tle) const {
        char line1Buf[513] = {};
        to_c(line1, line1Buf);
        char line2Buf[513] = {};
        to_c(line2, line2Buf);
        char xs_tleBuf[513] = {};

        int errCode = p_TleLinesToArray(line1Buf, line2Buf, xa_tle.data(), xs_tleBuf);

        from_c(xs_tleBuf, xs_tle);
        return errCode;
    }

    // Parses SP data from the input first and second lines of a two line element set.
    // Only applies to SP propagator.
    // This function only parses data from the input TLE but DOES NOT load/add the input TLE to memory.
    int TleParseSP(const std::string& line1, const std::string& line2, int& satNum, char& secClass, std::string& satName, int& epochYr, double& epochDays, double& bTerm, double& ogParm, double& agom, int& elsetNum, double& incli, double& node, double& eccen, double& omega, double& mnAnomaly, double& mnMotion, int& revNum) const {
        char line1Buf[513] = {};
        to_c(line1, line1Buf);
        char line2Buf[513] = {};
        to_c(line2, line2Buf);
        char satNameBuf[9] = {};

        int errCode = p_TleParseSP(line1Buf, line2Buf, &satNum, &secClass, satNameBuf, &epochYr, &epochDays, &bTerm, &ogParm, &agom, &elsetNum, &incli, &node, &eccen, &omega, &mnAnomaly, &mnMotion, &revNum);

        from_c(satNameBuf, satName);
        return errCode;
    }

    // Returns the first and second lines representation of a TLE of a satellite. 
    int TleGetLines(__int64 satKey, std::string& line1, std::string& line2) const {
        char line1Buf[513] = {};
        char line2Buf[513] = {};

        int errCode = p_TleGetLines(satKey, line1Buf, line2Buf);

        from_c(line1Buf, line1);
        from_c(line2Buf, line2);
        return errCode;
    }

    // Returns the CSV string representation of a TLE of a satellite. 
    int TleGetCsv(__int64 satKey, std::string& csvLine) const {
        char csvLineBuf[513] = {};

        int errCode = p_TleGetCsv(satKey, csvLineBuf);

        from_c(csvLineBuf, csvLine);
        return errCode;
    }

    // Constructs a TLE from individually provided GP data fields.
    // This function only parses data from the input fields but DOES NOT load/add the TLE to memory.
    // Returned line1 and line2 will be empty if the function fails to construct the lines as requested.
    void TleGPFieldsToLines(int satNum, char secClass, const std::string& satName, int epochYr, double epochDays, double nDotO2, double n2DotO6, double bstar, int ephType, int elsetNum, double incli, double node, double eccen, double omega, double mnAnomaly, double mnMotion, int revNum, std::string& line1, std::string& line2) const {
        char satNameBuf[9] = {};
        to_c(satName, satNameBuf);
        char line1Buf[513] = {};
        char line2Buf[513] = {};

        p_TleGPFieldsToLines(satNum, secClass, satNameBuf, epochYr, epochDays, nDotO2, n2DotO6, bstar, ephType, elsetNum, incli, node, eccen, omega, mnAnomaly, mnMotion, revNum, line1Buf, line2Buf);

        from_c(line1Buf, line1);
        from_c(line2Buf, line2);
    }

    // Constructs a TLE from individually provided GP data fields.
    // This function only parses data from the input fields but DOES NOT load/add the TLE to memory.
    // Returned line1 and line2 will be empty if the function fails to construct the lines as requested.
    void TleGPFieldsToCsv(int satNum, char secClass, const std::string& satName, int epochYr, double epochDays, double nDotO2, double n2DotO6, double bstar, int ephType, int elsetNum, double incli, double node, double eccen, double omega, double mnAnomaly, double mnMotion, int revNum, std::string& csvLine) const {
        char satNameBuf[9] = {};
        to_c(satName, satNameBuf);
        char csvLineBuf[513] = {};

        p_TleGPFieldsToCsv(satNum, secClass, satNameBuf, epochYr, epochDays, nDotO2, n2DotO6, bstar, ephType, elsetNum, incli, node, eccen, omega, mnAnomaly, mnMotion, revNum, csvLineBuf);

        from_c(csvLineBuf, csvLine);
    }

    // Constructs a TLE from GP data stored in the input parameters.
    // This function only parses data from the input data but DOES NOT load/add the TLE to memory.
    // <br>
    // Returned line1 and line2 will be empty if the function fails to construct the lines as requested.
    void TleGPArrayToLines(const std::array<double,64>& xa_tle, const std::string& xs_tle, std::string& line1, std::string& line2) const {
        char xs_tleBuf[513] = {};
        to_c(xs_tle, xs_tleBuf);
        char line1Buf[513] = {};
        char line2Buf[513] = {};

        p_TleGPArrayToLines(const_cast<double*>(xa_tle.data()), xs_tleBuf, line1Buf, line2Buf);

        from_c(line1Buf, line1);
        from_c(line2Buf, line2);
    }

    // Constructs a TLE from GP data stored in the input parameters.
    // This function only parses data from the input data but DOES NOT load/add the TLE to memory.
    // Returned line1 and line2 will be empty if the function fails to construct the lines as requested.
    void TleGPArrayToCsv(const std::array<double,64>& xa_tle, const std::string& xs_tle, std::string& csvline) const {
        char xs_tleBuf[513] = {};
        to_c(xs_tle, xs_tleBuf);
        char csvlineBuf[513] = {};

        p_TleGPArrayToCsv(const_cast<double*>(xa_tle.data()), xs_tleBuf, csvlineBuf);

        from_c(csvlineBuf, csvline);
    }

    // Constructs a TLE from individually provided SP data fields.
    // Only applies to SP propagator.
    // This function only parses data from the input fields but DOES NOT load/add the TLE to memory.
    // Returned line1 and line2 will be empty if the function fails to construct the lines as requested.
    void TleSPFieldsToLines(int satNum, char secClass, const std::string& satName, int epochYr, double epochDays, double bTerm, double ogParm, double agom, int elsetNum, double incli, double node, double eccen, double omega, double mnAnomaly, double mnMotion, int revNum, std::string& line1, std::string& line2) const {
        char satNameBuf[9] = {};
        to_c(satName, satNameBuf);
        char line1Buf[513] = {};
        char line2Buf[513] = {};

        p_TleSPFieldsToLines(satNum, secClass, satNameBuf, epochYr, epochDays, bTerm, ogParm, agom, elsetNum, incli, node, eccen, omega, mnAnomaly, mnMotion, revNum, line1Buf, line2Buf);

        from_c(line1Buf, line1);
        from_c(line2Buf, line2);
    }

    // Returns the first satKey from the currently loaded set of TLEs that contains the specified satellite number.
    // This function is useful when Tle.dll is used in applications that require only one record (one TLE entry) for one satellite, and which refer to that TLE by its satellite number. This function can be used to retrieve a satKey in that situation, which is useful since the Standardized Astrodynamic Algorithms library works only with satKeys.
    // A negative value will be returned if there is an error.
    __int64 TleGetSatKey(int satNum) const {

        __int64 satKey = p_TleGetSatKey(satNum);

        return satKey;
    }

    // This function is similar to TleGetSatKey but designed to be used in Matlab. 
    // Matlab doesn't correctly return the 19-digit satellite key using TleGetSatKey. This method is an alternative way to return the satKey output.
    // This function is useful when Tle.dll is used in applications that require only one record (one TLE entry) for one satellite, and which refer to that TLE by its satellite number. This function can be used to retrieve a satKey in that situation, which is useful since the Standardized Astrodynamic Algorithms library works only with satKeys.
    // A negative value will be returned in satKey if there is an error.
    void TleGetSatKeyML(int satNum, __int64& satKey) const {

        p_TleGetSatKeyML(satNum, &satKey);

    }

    // Computes a satKey from the input data.
    // There is no need for a matching satellite to be loaded prior to using this function. The function simply computes the satKey from the provided fields.
    // 
    // This is the proper way to reconstruct a satKey from its fields. If you use your own routine to do this, the computed satKey might be different. 
    // A negative value will be returned if there is an error.
    __int64 TleFieldsToSatKey(int satNum, int epochYr, double epochDays, int ephType) const {

        __int64 satKey = p_TleFieldsToSatKey(satNum, epochYr, epochDays, ephType);

        return satKey;
    }

    // This function is similar to TleFieldsToSatKey but designed to be used in Matlab. 
    // Matlab doesn't correctly return the 19-digit satellite key using TleFieldsToSatKey. This method is an alternative way to return the satKey output.
    // There is no need for a matching satellite to be loaded prior to using this function. The function simply computes the satKey from the provided fields.
    // 
    // This is the proper way to reconstruct a satKey from its fields. If you use your own routine to do this, the computed satKey might be different.
    // A negative value will be returned in satKey if there is an error.
    void TleFieldsToSatKeyML(int satNum, int epochYr, double epochDays, int ephType, __int64& satKey) const {

        p_TleFieldsToSatKeyML(satNum, epochYr, epochDays, ephType, &satKey);

    }

    // Adds a TLE (satellite), using its data stored in the input parameters.
    __int64 TleAddSatFrArray(const std::array<double,64>& xa_tle, const std::string& xs_tle) const {
        char xs_tleBuf[513] = {};
        to_c(xs_tle, xs_tleBuf);

        __int64 satKey = p_TleAddSatFrArray(const_cast<double*>(xa_tle.data()), xs_tleBuf);

        return satKey;
    }

    // This function is similar to TleAddSatFrArray but designed to be used in Matlab. 
    void TleAddSatFrArrayML(const std::array<double,64>& xa_tle, const std::string& xs_tle, __int64& satKey) const {
        char xs_tleBuf[513] = {};
        to_c(xs_tle, xs_tleBuf);

        p_TleAddSatFrArrayML(const_cast<double*>(xa_tle.data()), xs_tleBuf, &satKey);

    }

    // Updates existing TLE data with the provided new data stored in the input parameters. Note: satNum, year, day, and ephtype can't be updated.
    // nDotO2 and n2DotO6 values are not used in the SGP4 propagator. However, some users still want to preserve the integrity of all input data.
    int TleUpdateSatFrArray(__int64 satKey, const std::array<double,64>& xa_tle, const std::string& xs_tle) const {
        char xs_tleBuf[513] = {};
        to_c(xs_tle, xs_tleBuf);

        int errCode = p_TleUpdateSatFrArray(satKey, const_cast<double*>(xa_tle.data()), xs_tleBuf);

        return errCode;
    }

    // Retrieves TLE data and stored it in the passing parameters
    int TleDataToArray(__int64 satKey, std::array<double,64>& xa_tle, std::string& xs_tle) const {
        char xs_tleBuf[513] = {};

        int errCode = p_TleDataToArray(satKey, xa_tle.data(), xs_tleBuf);

        from_c(xs_tleBuf, xs_tle);
        return errCode;
    }

    // Converts TLE two line format to CSV format
    int TleLinesToCsv(const std::string& line1, const std::string& line2, std::string& csvline) const {
        char line1Buf[513] = {};
        to_c(line1, line1Buf);
        char line2Buf[513] = {};
        to_c(line2, line2Buf);
        char csvlineBuf[513] = {};

        int errCode = p_TleLinesToCsv(line1Buf, line2Buf, csvlineBuf);

        from_c(csvlineBuf, csvline);
        return errCode;
    }

    // Converts TLE CSV format to two line format
    int TleCsvToLines(const std::string& csvLine, int newSatno, std::string& line1, std::string& line2) const {
        char csvLineBuf[513] = {};
        to_c(csvLine, csvLineBuf);
        char line1Buf[513] = {};
        char line2Buf[513] = {};

        int errCode = p_TleCsvToLines(csvLineBuf, newSatno, line1Buf, line2Buf);

        from_c(line1Buf, line1);
        from_c(line2Buf, line2);
        return errCode;
    }

    // Sets TLE key mode - This function was deprecated, please use DllMain/SetElsetKeyMode() instead
    int SetTleKeyMode(int tle_keyMode) const {

        int errCode = p_SetTleKeyMode(tle_keyMode);

        return errCode;
    }

    // Gets current TLE key mode - This function was deprecated, please use DllMain/GetElsetKeyMode() instead
    int GetTleKeyMode() const {

        int tle_keyMode = p_GetTleKeyMode();

        return tle_keyMode;
    }

    // Finds the check sums of TLE lines
    void GetCheckSums(const std::string& line1, const std::string& line2, int& chkSum1, int& chkSum2, int& errCode) const {
        char line1Buf[513] = {};
        to_c(line1, line1Buf);
        char line2Buf[513] = {};
        to_c(line2, line2Buf);

        p_GetCheckSums(line1Buf, line2Buf, &chkSum1, &chkSum2, &errCode);

    }


private:
    TleApi() : lib(resolveLibraryName()) {loadSymbols();}
    ~TleApi() = default;

    using fn_TleInit = int (STDCALL *)(__int64 apAddr);
    using fn_TleGetInfo = void (STDCALL *)(char infoStr[128]);
    using fn_TleLoadFile = int (STDCALL *)(char tleFile[512]);
    using fn_TleSaveFile = int (STDCALL *)(char tleFile[512], int saveMode, int xf_tleForm);
    using fn_TleRemoveSat = int (STDCALL *)(__int64 satKey);
    using fn_TleRemoveAllSats = int (STDCALL *)();
    using fn_TleGetCount = int (STDCALL *)();
    using fn_TleGetLoaded = void (STDCALL *)(int order, __int64 satKeys[]);
    using fn_TleAddSatFrLines = __int64 (STDCALL *)(char line1[512], char line2[512]);
    using fn_TleAddSatFrLinesML = void (STDCALL *)(char line1[512], char line2[512], __int64* satKey);
    using fn_TleAddSatFrCsv = __int64 (STDCALL *)(char csvLine[512]);
    using fn_TleAddSatFrCsvML = void (STDCALL *)(char csvLine[512], __int64* satKey);
    using fn_TleAddSatFrFieldsGP = __int64 (STDCALL *)(int satNum, char secClass, char satName[8], int epochYr, double epochDays, double bstar, int ephType, int elsetNum, double incli, double node, double eccen, double omega, double mnAnomaly, double mnMotion, int revNum);
    using fn_TleAddSatFrFieldsGP2 = __int64 (STDCALL *)(int satNum, char secClass, char satName[8], int epochYr, double epochDays, double bstar, int ephType, int elsetNum, double incli, double node, double eccen, double omega, double mnAnomaly, double mnMotion, int revNum, double nDotO2, double n2DotO6);
    using fn_TleAddSatFrFieldsGP2ML = void (STDCALL *)(int satNum, char secClass, char satName[8], int epochYr, double epochDays, double bstar, int ephType, int elsetNum, double incli, double node, double eccen, double omega, double mnAnomaly, double mnMotion, int revNum, double nDotO2, double n2DotO6, __int64* satKey);
    using fn_TleUpdateSatFrFieldsGP = int (STDCALL *)(__int64 satKey, char secClass, char satName[8], double bstar, int elsetNum, double incli, double node, double eccen, double omega, double mnAnomaly, double mnMotion, int revNum);
    using fn_TleUpdateSatFrFieldsGP2 = int (STDCALL *)(__int64 satKey, char secClass, char satName[8], double bstar, int elsetNum, double incli, double node, double eccen, double omega, double mnAnomaly, double mnMotion, int revNum, double nDotO2, double n2DotO6);
    using fn_TleAddSatFrFieldsSP = __int64 (STDCALL *)(int satNum, char secClass, char satName[8], int epochYr, double epochDays, double bTerm, double ogParm, double agom, int elsetNum, double incli, double node, double eccen, double omega, double mnAnomaly, double mnMotion, int revNum);
    using fn_TleAddSatFrFieldsSPML = void (STDCALL *)(int satNum, char secClass, char satName[8], int epochYr, double epochDays, double bTerm, double ogParm, double agom, int elsetNum, double incli, double node, double eccen, double omega, double mnAnomaly, double mnMotion, int revNum, __int64* satKey);
    using fn_TleUpdateSatFrFieldsSP = int (STDCALL *)(__int64 satKey, char secClass, char satName[8], double bterm, double ogParm, double agom, int elsetNum, double incli, double node, double eccen, double omega, double mnAnomaly, double mnMotion, int revNum);
    using fn_TleSetField = int (STDCALL *)(__int64 satKey, int xf_Tle, char valueStr[512]);
    using fn_TleGetField = int (STDCALL *)(__int64 satKey, int xf_Tle, char valueStr[512]);
    using fn_TleGetAllFieldsGP = int (STDCALL *)(__int64 satKey, int* satNum, char* secClass, char satName[8], int* epochYr, double* epochDays, double* bstar, int* ephType, int* elsetNum, double* incli, double* node, double* eccen, double* omega, double* mnAnomaly, double* mnMotion, int* revNum);
    using fn_TleGetAllFieldsGP2 = int (STDCALL *)(__int64 satKey, int* satNum, char* secClass, char satName[8], int* epochYr, double* epochDays, double* bstar, int* ephType, int* elsetNum, double* incli, double* node, double* eccen, double* omega, double* mnAnomaly, double* mnMotion, int* revNum, double* nDotO2, double* n2DotO6);
    using fn_TleGetAllFieldsSP = int (STDCALL *)(__int64 satKey, int* satNum, char* secClass, char satName[8], int* epochYr, double* epochDays, double* bTerm, double* ogParm, double* agom, int* elsetNum, double* incli, double* node, double* eccen, double* omega, double* mnAnomaly, double* mnMotion, int* revNum);
    using fn_TleParseGP = int (STDCALL *)(char line1[512], char line2[512], int* satNum, char* secClass, char satName[8], int* epochYr, double* epochDays, double* nDotO2, double* n2DotO6, double* bstar, int* ephType, int* elsetNum, double* incli, double* node, double* eccen, double* omega, double* mnAnomaly, double* mnMotion, int* revNum);
    using fn_TleLinesToArray = int (STDCALL *)(char line1[512], char line2[512], double xa_tle[64], char xs_tle[512]);
    using fn_TleParseSP = int (STDCALL *)(char line1[512], char line2[512], int* satNum, char* secClass, char satName[8], int* epochYr, double* epochDays, double* bTerm, double* ogParm, double* agom, int* elsetNum, double* incli, double* node, double* eccen, double* omega, double* mnAnomaly, double* mnMotion, int* revNum);
    using fn_TleGetLines = int (STDCALL *)(__int64 satKey, char line1[512], char line2[512]);
    using fn_TleGetCsv = int (STDCALL *)(__int64 satKey, char csvLine[512]);
    using fn_TleGPFieldsToLines = void (STDCALL *)(int satNum, char secClass, char satName[8], int epochYr, double epochDays, double nDotO2, double n2DotO6, double bstar, int ephType, int elsetNum, double incli, double node, double eccen, double omega, double mnAnomaly, double mnMotion, int revNum, char line1[512], char line2[512]);
    using fn_TleGPFieldsToCsv = void (STDCALL *)(int satNum, char secClass, char satName[8], int epochYr, double epochDays, double nDotO2, double n2DotO6, double bstar, int ephType, int elsetNum, double incli, double node, double eccen, double omega, double mnAnomaly, double mnMotion, int revNum, char csvLine[512]);
    using fn_TleGPArrayToLines = void (STDCALL *)(double xa_tle[64], char xs_tle[512], char line1[512], char line2[512]);
    using fn_TleGPArrayToCsv = void (STDCALL *)(double xa_tle[64], char xs_tle[512], char csvline[512]);
    using fn_TleSPFieldsToLines = void (STDCALL *)(int satNum, char secClass, char satName[8], int epochYr, double epochDays, double bTerm, double ogParm, double agom, int elsetNum, double incli, double node, double eccen, double omega, double mnAnomaly, double mnMotion, int revNum, char line1[512], char line2[512]);
    using fn_TleGetSatKey = __int64 (STDCALL *)(int satNum);
    using fn_TleGetSatKeyML = void (STDCALL *)(int satNum, __int64* satKey);
    using fn_TleFieldsToSatKey = __int64 (STDCALL *)(int satNum, int epochYr, double epochDays, int ephType);
    using fn_TleFieldsToSatKeyML = void (STDCALL *)(int satNum, int epochYr, double epochDays, int ephType, __int64* satKey);
    using fn_TleAddSatFrArray = __int64 (STDCALL *)(double xa_tle[64], char xs_tle[512]);
    using fn_TleAddSatFrArrayML = void (STDCALL *)(double xa_tle[64], char xs_tle[512], __int64* satKey);
    using fn_TleUpdateSatFrArray = int (STDCALL *)(__int64 satKey, double xa_tle[64], char xs_tle[512]);
    using fn_TleDataToArray = int (STDCALL *)(__int64 satKey, double xa_tle[64], char xs_tle[512]);
    using fn_TleLinesToCsv = int (STDCALL *)(char line1[512], char line2[512], char csvline[512]);
    using fn_TleCsvToLines = int (STDCALL *)(char csvLine[512], int newSatno, char line1[512], char line2[512]);
    using fn_SetTleKeyMode = int (STDCALL *)(int tle_keyMode);
    using fn_GetTleKeyMode = int (STDCALL *)();
    using fn_GetCheckSums = void (STDCALL *)(char line1[512], char line2[512], int* chkSum1, int* chkSum2, int* errCode);

    fn_TleInit p_TleInit{};
    fn_TleGetInfo p_TleGetInfo{};
    fn_TleLoadFile p_TleLoadFile{};
    fn_TleSaveFile p_TleSaveFile{};
    fn_TleRemoveSat p_TleRemoveSat{};
    fn_TleRemoveAllSats p_TleRemoveAllSats{};
    fn_TleGetCount p_TleGetCount{};
    fn_TleGetLoaded p_TleGetLoaded{};
    fn_TleAddSatFrLines p_TleAddSatFrLines{};
    fn_TleAddSatFrLinesML p_TleAddSatFrLinesML{};
    fn_TleAddSatFrCsv p_TleAddSatFrCsv{};
    fn_TleAddSatFrCsvML p_TleAddSatFrCsvML{};
    fn_TleAddSatFrFieldsGP p_TleAddSatFrFieldsGP{};
    fn_TleAddSatFrFieldsGP2 p_TleAddSatFrFieldsGP2{};
    fn_TleAddSatFrFieldsGP2ML p_TleAddSatFrFieldsGP2ML{};
    fn_TleUpdateSatFrFieldsGP p_TleUpdateSatFrFieldsGP{};
    fn_TleUpdateSatFrFieldsGP2 p_TleUpdateSatFrFieldsGP2{};
    fn_TleAddSatFrFieldsSP p_TleAddSatFrFieldsSP{};
    fn_TleAddSatFrFieldsSPML p_TleAddSatFrFieldsSPML{};
    fn_TleUpdateSatFrFieldsSP p_TleUpdateSatFrFieldsSP{};
    fn_TleSetField p_TleSetField{};
    fn_TleGetField p_TleGetField{};
    fn_TleGetAllFieldsGP p_TleGetAllFieldsGP{};
    fn_TleGetAllFieldsGP2 p_TleGetAllFieldsGP2{};
    fn_TleGetAllFieldsSP p_TleGetAllFieldsSP{};
    fn_TleParseGP p_TleParseGP{};
    fn_TleLinesToArray p_TleLinesToArray{};
    fn_TleParseSP p_TleParseSP{};
    fn_TleGetLines p_TleGetLines{};
    fn_TleGetCsv p_TleGetCsv{};
    fn_TleGPFieldsToLines p_TleGPFieldsToLines{};
    fn_TleGPFieldsToCsv p_TleGPFieldsToCsv{};
    fn_TleGPArrayToLines p_TleGPArrayToLines{};
    fn_TleGPArrayToCsv p_TleGPArrayToCsv{};
    fn_TleSPFieldsToLines p_TleSPFieldsToLines{};
    fn_TleGetSatKey p_TleGetSatKey{};
    fn_TleGetSatKeyML p_TleGetSatKeyML{};
    fn_TleFieldsToSatKey p_TleFieldsToSatKey{};
    fn_TleFieldsToSatKeyML p_TleFieldsToSatKeyML{};
    fn_TleAddSatFrArray p_TleAddSatFrArray{};
    fn_TleAddSatFrArrayML p_TleAddSatFrArrayML{};
    fn_TleUpdateSatFrArray p_TleUpdateSatFrArray{};
    fn_TleDataToArray p_TleDataToArray{};
    fn_TleLinesToCsv p_TleLinesToCsv{};
    fn_TleCsvToLines p_TleCsvToLines{};
    fn_SetTleKeyMode p_SetTleKeyMode{};
    fn_GetTleKeyMode p_GetTleKeyMode{};
    fn_GetCheckSums p_GetCheckSums{};

    SharedLibrary lib;

    static const char* resolveLibraryName() {
#ifdef _WIN32
        return "Tle.dll";
#elif __APPLE__
        return "libtle.dylib";
#else
        return "libtle.so";
#endif
    }

    void loadSymbols() {
        p_TleInit = lib.load<fn_TleInit>("TleInit");
        p_TleGetInfo = lib.load<fn_TleGetInfo>("TleGetInfo");
        p_TleLoadFile = lib.load<fn_TleLoadFile>("TleLoadFile");
        p_TleSaveFile = lib.load<fn_TleSaveFile>("TleSaveFile");
        p_TleRemoveSat = lib.load<fn_TleRemoveSat>("TleRemoveSat");
        p_TleRemoveAllSats = lib.load<fn_TleRemoveAllSats>("TleRemoveAllSats");
        p_TleGetCount = lib.load<fn_TleGetCount>("TleGetCount");
        p_TleGetLoaded = lib.load<fn_TleGetLoaded>("TleGetLoaded");
        p_TleAddSatFrLines = lib.load<fn_TleAddSatFrLines>("TleAddSatFrLines");
        p_TleAddSatFrLinesML = lib.load<fn_TleAddSatFrLinesML>("TleAddSatFrLinesML");
        p_TleAddSatFrCsv = lib.load<fn_TleAddSatFrCsv>("TleAddSatFrCsv");
        p_TleAddSatFrCsvML = lib.load<fn_TleAddSatFrCsvML>("TleAddSatFrCsvML");
        p_TleAddSatFrFieldsGP = lib.load<fn_TleAddSatFrFieldsGP>("TleAddSatFrFieldsGP");
        p_TleAddSatFrFieldsGP2 = lib.load<fn_TleAddSatFrFieldsGP2>("TleAddSatFrFieldsGP2");
        p_TleAddSatFrFieldsGP2ML = lib.load<fn_TleAddSatFrFieldsGP2ML>("TleAddSatFrFieldsGP2ML");
        p_TleUpdateSatFrFieldsGP = lib.load<fn_TleUpdateSatFrFieldsGP>("TleUpdateSatFrFieldsGP");
        p_TleUpdateSatFrFieldsGP2 = lib.load<fn_TleUpdateSatFrFieldsGP2>("TleUpdateSatFrFieldsGP2");
        p_TleAddSatFrFieldsSP = lib.load<fn_TleAddSatFrFieldsSP>("TleAddSatFrFieldsSP");
        p_TleAddSatFrFieldsSPML = lib.load<fn_TleAddSatFrFieldsSPML>("TleAddSatFrFieldsSPML");
        p_TleUpdateSatFrFieldsSP = lib.load<fn_TleUpdateSatFrFieldsSP>("TleUpdateSatFrFieldsSP");
        p_TleSetField = lib.load<fn_TleSetField>("TleSetField");
        p_TleGetField = lib.load<fn_TleGetField>("TleGetField");
        p_TleGetAllFieldsGP = lib.load<fn_TleGetAllFieldsGP>("TleGetAllFieldsGP");
        p_TleGetAllFieldsGP2 = lib.load<fn_TleGetAllFieldsGP2>("TleGetAllFieldsGP2");
        p_TleGetAllFieldsSP = lib.load<fn_TleGetAllFieldsSP>("TleGetAllFieldsSP");
        p_TleParseGP = lib.load<fn_TleParseGP>("TleParseGP");
        p_TleLinesToArray = lib.load<fn_TleLinesToArray>("TleLinesToArray");
        p_TleParseSP = lib.load<fn_TleParseSP>("TleParseSP");
        p_TleGetLines = lib.load<fn_TleGetLines>("TleGetLines");
        p_TleGetCsv = lib.load<fn_TleGetCsv>("TleGetCsv");
        p_TleGPFieldsToLines = lib.load<fn_TleGPFieldsToLines>("TleGPFieldsToLines");
        p_TleGPFieldsToCsv = lib.load<fn_TleGPFieldsToCsv>("TleGPFieldsToCsv");
        p_TleGPArrayToLines = lib.load<fn_TleGPArrayToLines>("TleGPArrayToLines");
        p_TleGPArrayToCsv = lib.load<fn_TleGPArrayToCsv>("TleGPArrayToCsv");
        p_TleSPFieldsToLines = lib.load<fn_TleSPFieldsToLines>("TleSPFieldsToLines");
        p_TleGetSatKey = lib.load<fn_TleGetSatKey>("TleGetSatKey");
        p_TleGetSatKeyML = lib.load<fn_TleGetSatKeyML>("TleGetSatKeyML");
        p_TleFieldsToSatKey = lib.load<fn_TleFieldsToSatKey>("TleFieldsToSatKey");
        p_TleFieldsToSatKeyML = lib.load<fn_TleFieldsToSatKeyML>("TleFieldsToSatKeyML");
        p_TleAddSatFrArray = lib.load<fn_TleAddSatFrArray>("TleAddSatFrArray");
        p_TleAddSatFrArrayML = lib.load<fn_TleAddSatFrArrayML>("TleAddSatFrArrayML");
        p_TleUpdateSatFrArray = lib.load<fn_TleUpdateSatFrArray>("TleUpdateSatFrArray");
        p_TleDataToArray = lib.load<fn_TleDataToArray>("TleDataToArray");
        p_TleLinesToCsv = lib.load<fn_TleLinesToCsv>("TleLinesToCsv");
        p_TleCsvToLines = lib.load<fn_TleCsvToLines>("TleCsvToLines");
        p_SetTleKeyMode = lib.load<fn_SetTleKeyMode>("SetTleKeyMode");
        p_GetTleKeyMode = lib.load<fn_GetTleKeyMode>("GetTleKeyMode");
        p_GetCheckSums = lib.load<fn_GetCheckSums>("GetCheckSums");
    }
};

// ========================= End of auto generated code ==========================

// This wrapper file was generated automatically by the GenDllWrappers program.
#pragma once

#include "utils.hpp"
#include <string>
#include <vector>
#include <array>
#include <cstring>

class EnvConstApi {
public:
    static EnvConstApi& instance() {
        static EnvConstApi inst;
        return inst;
    }
    EnvConstApi(const EnvConstApi&)              = delete;
    EnvConstApi(EnvConstApi&&)                   = delete;
    EnvConstApi& operator = (const EnvConstApi&) = delete;
    EnvConstApi& operator = (EnvConstApi&&)      = delete;


    // Indexes of Earth Constant fields
    // Earth flattening (reciprocal, unitless)
    static constexpr int XF_GEOCON_FF    = 1;
    // J2 (unitless)
    static constexpr int XF_GEOCON_J2    = 2;
    // J3 (unitless)
    static constexpr int XF_GEOCON_J3    = 3;
    // J4 (unitless)
    static constexpr int XF_GEOCON_J4    = 4;
    // Ke (er**1.5/min)
    static constexpr int XF_GEOCON_KE    = 5;
    // Earth radius (km/er)
    static constexpr int XF_GEOCON_KMPER = 6;
    // Earth rotation rate w.r.t. fixed equinox (rad/min)
    static constexpr int XF_GEOCON_RPTIM = 7;

    // J2/2 (unitless)
    static constexpr int XF_GEOCON_CK2   = 8;
    // -3/8 J4 (unitless)
    static constexpr int XF_GEOCON_CK4   = 9;
    // Converts km/sec to er/kem
    static constexpr int XF_GEOCON_KS2EK = 10;
    // Earth rotation rate w.r.t. fixed equinox (rad/kemin)
    static constexpr int XF_GEOCON_THDOT = 11;
    // J5 (unitless)
    static constexpr int XF_GEOCON_J5    = 12;
    // Gravitational parameter km^3/(solar s)^2
    static constexpr int XF_GEOCON_MU    = 13;


    // Indexes of FK Constant fields
    // Earth rotation rate w.r.t. moving equinox (rad/day)
    static constexpr int XF_FKCON_C1     = 1;
    // Earth rotation acceleration(rad/day**2)
    static constexpr int XF_FKCON_C1DOT  = 2;
    // Greenwich angle (1970  rad)
    static constexpr int XF_FKCON_THGR70 = 3;

    // Indexes represent geopotential models GEO
    // Earth constants - JGM2
    static constexpr int XF_GEOMOD_JGM2   =    2;
    // Earth constants - GEM5
    static constexpr int XF_GEOMOD_GEM5   =    5;
    // Earth constants - EGM-08
    static constexpr int XF_GEOMOD_EGM08  =    8;
    // Earth constants - GEM9
    static constexpr int XF_GEOMOD_GEM9   =    9;
    // Earth constants - STEM68
    static constexpr int XF_GEOMOD_STEM68 =   68;
    // Earth constants - WGS-72
    static constexpr int XF_GEOMOD_WGS72  =   72;
    // Earth constants - WGS-84
    static constexpr int XF_GEOMOD_WGS84  =   84;
    // Earth constants - EGM-96
    static constexpr int XF_GEOMOD_EGM96  =   96;
    // Invalid earth model
    static constexpr int XF_GEOMOD_UNKNOWN=  100;

    //*******************************************************************************

    // Indexes represent fundamental catalogue FK
    // Fundamental Catalog - FK5
    static constexpr int XF_FKMOD_4 = 4;
    // Fundamental Catalog - FK4
    static constexpr int XF_FKMOD_5 = 5;


    // Notes: This function has been deprecated since v9.0. 
    // Initializes the EnvInit DLL for use in the program.
    // If this function returns an error, it is recommended that you stop the program immediately.
    // 
    // An error will occur if you forget to load and initialize all the prerequisite DLLs, as listed in the DLL Prerequisites section of the accompanying documentation, before using this DLL.
    // 
    // When the function is called, the GEO model is set to WGS-72 and the FK model is set to FK5.  If the user plans to use the SGP4 propagator, do NOT change this default setting. Otherwise, SGP4 won't work
    int EnvInit(__int64 apAddr) const {

        int errCode = p_EnvInit(apAddr);

        return errCode;
    }

    // Returns information about the EnvConst DLL.
    // The returned string provides information about the version number, build date, and the platform of the EnvConst DLL.
    void EnvGetInfo(std::string& infoStr) const {
        char infoStrBuf[129] = {};

        p_EnvGetInfo(infoStrBuf);

        from_c(infoStrBuf, infoStr);
    }

    // Reads Earth constants (GEO) model and fundamental catalogue (FK) model settings from a file.
    // The users can use NAME=VALUE pair to setup the GEO and FK models in the input file. 
    // 
    // For GEO model, the valid names are GEOCONST, BCONST and the valid values are WGS-72, WGS72,  72, WGS-84, WGS84, 84, EGM-96, EGM96, 96, EGM-08, EGM08, 08, JGM-2, JGM2, 2, SEM68R, 68, GEM5, 5, GEM9, and 9.
    // 
    // For FK model, the valid name is FKCONST and the valid values are: FK4, 4, FK5, 5.
    // 
    // All the string literals are case-insensitive.
    int EnvLoadFile(const std::string& envFile) const {
        char envFileBuf[513] = {};
        to_c(envFile, envFileBuf);

        int errCode = p_EnvLoadFile(envFileBuf);

        return errCode;
    }

    // Saves the current Earth constants (GEO) model and fundamental catalogue (FK) model settings to a file.
    // Returns zero indicating the GEO and FK settings have been successfully saved to the file. Other values indicate an error.
    int EnvSaveFile(const std::string& envConstFile, int saveMode, int saveForm) const {
        char envConstFileBuf[513] = {};
        to_c(envConstFile, envConstFileBuf);

        int errCode = p_EnvSaveFile(envConstFileBuf, saveMode, saveForm);

        return errCode;
    }

    // Returns the current fundamental catalogue (FK) setting. 
    // The FK model is shared among all the Standardized Astrodynamic Algorithms DLLs in the program. 
    int EnvGetFkIdx() const {

        int xf_FkMod = p_EnvGetFkIdx();

        return xf_FkMod;
    }

    // Changes the fundamental catalogue (FK) setting to the specified value. 
    // If the users enter an invalid value for the fkIdx, the program will continue to use the current setting. 
    // 
    // The FK model is globally shared among the Standardized Astrodynamic Algorithms DLLs. If its setting is changed, the new setting takes effect immediately.
    // The FK model must be set to FK5 to use the SGP4 propagator.
    void EnvSetFkIdx(int xf_FkMod) const {

        p_EnvSetFkIdx(xf_FkMod);

    }

    // Returns the current Earth constants (GEO) setting. 
    // <br>
    // The GEO model is shared among all the Standardized Astrodynamic Algorithms DLLs in the program.
    // <br>
    // The following table lists possible values of the return value GEO setting:
    // <table>
    // <caption>table</caption>
    // <tr>
    // <td><b>Value</b></td>
    // <td><b>Value interpretation</b></td>
    // </tr>
    // <tr><td>84</td><td>WGS-84</td></tr>
    // <tr><td>96</td><td>EGM-96</td></tr>
    // <tr><td>08</td><td>EGM-08</td></tr>
    // <tr><td>72</td><td>WGS-72 (default)</td></tr>
    // <tr><td>2</td><td>JGM2</td></tr>
    // <tr><td>68</td><td>STEM68R, SEM68R</td></tr>
    // <tr><td>5</td><td>GEM5</td></tr>
    // <tr><td>9</td><td>GEM9</td></tr>
    // </table>
    int EnvGetGeoIdx() const {

        int xf_GeoMod = p_EnvGetGeoIdx();

        return xf_GeoMod;
    }

    // Changes the Earth constants (GEO) setting to the specified value.
    // <br>
    // If you specify an invalid value for xf_GeoMod, the program will continue to use the current setting. 
    // <br>
    // The GEO model is globally shared among the Standardized Astrodynamic Algorithms DLLs. If its setting is changed, the new setting takes effect immediately
    // <br>
    // The following table lists possible values of the parameter value GEO setting:
    // <table>
    // <caption>table</caption>
    // <tr>
    // <td><b>Value</b></td>
    // <td><b>Value interpretation</b></td>
    // </tr>
    // <tr><td>84</td><td>WGS-84</td></tr>
    // <tr><td>96</td><td>EGM-96</td></tr>
    // <tr><td>08</td><td>EGM-08</td></tr>
    // <tr><td>72</td><td>WGS-72 (default)</td></tr>
    // <tr><td>2</td><td>JGM2</td></tr>
    // <tr><td>68</td><td>STEM68R, SEM68R</td></tr>
    // <tr><td>5</td><td>GEM5</td></tr>
    // <tr><td>9</td><td>GEM9</td></tr>
    // </table>
    // <br>
    // The GEO model must be set to WGS-72 to use the SGP4 propagator.
    void EnvSetGeoIdx(int xf_GeoMod) const {

        p_EnvSetGeoIdx(xf_GeoMod);

    }

    // Returns the name of the current Earth constants (GEO) model. 
    // <br>
    // The geoStr parameter may contain one of the following values:
    // <table>
    // <caption>table</caption>
    // <tr><td>WGS-84</td></tr>
    // <tr><td>EGM-96</td></tr>
    // <tr><td>EGM-08</td></tr>
    // <tr><td>WGS-72</td></tr>
    // <tr><td>JGM2</td></tr>
    // <tr><td>SEM68R</td></tr>
    // <tr><td>GEM5</td></tr>
    // <tr><td>GEM9</td></tr>
    // </table>
    void EnvGetGeoStr(std::string& geoStr) const {
        char geoStrBuf[7] = {};

        p_EnvGetGeoStr(geoStrBuf);

        from_c(geoStrBuf, geoStr);
    }

    // Changes the Earth constants (GEO) setting to the model specified by a string literal. 
    // <br>
    // If you specify an invalid value for geoStr, the program will continue to use the current setting.
    // <br>
    // The GEO model is globally shared among the Standardized Astrodynamic Algorithms DLLs. If its setting is changed, the new setting takes effect immediately.
    // <br>
    // The following table lists possible values of the parameter value GEO setting:
    // <table>
    // <caption>table</caption>
    // <tr>
    // <td><b>geoStr (any string in the row)</b></td>
    // <td><b>Interpretation</b></td>
    // </tr>
    // <tr><td>'WGS-84', 'WGS84', '84'</td><td>WGS-84</td></tr>
    // <tr><td>'EGM-96', 'EGM96', '96'</td><td>EGM-96</td></tr>
    // <tr><td>'EGM-08', 'EGM08', '8'</td><td>EGM-08</td></tr>
    // <tr><td>'WGS-72', 'WGS72', '72'</td><td>WGS-72 (default)</td></tr>
    // <tr><td>'JGM-2, 'JGM2', '2'</td><td>JGM-2</td></tr>
    // <tr><td>'SEM68R', '68'</td><td>STEM68R, SEM68R</td></tr>
    // <tr><td>'GEM5', '5'</td><td>GEM5</td></tr>
    // <tr><td>'GEM9', '9'</td><td>GEM9</td></tr>
    // </table>   
    // <br>
    // The GEO model must be set to WGS-72 to use the SGP4 propagator.
    void EnvSetGeoStr(const std::string& geoStr) const {
        char geoStrBuf[7] = {};
        to_c(geoStr, geoStrBuf);

        p_EnvSetGeoStr(geoStrBuf);

    }

    // Retrieves the value of one of the constants from the current Earth constants (GEO) model. 
    double EnvGetGeoConst(int xf_GeoCon) const {

        double retVal = p_EnvGetGeoConst(xf_GeoCon);

        return retVal;
    }

    // Retrieves the value of one of the constants from the current fundamental catalogue (FK) model.
    double EnvGetFkConst(int xf_FkCon) const {

        double retVal = p_EnvGetFkConst(xf_FkCon);

        return retVal;
    }

    // Returns a handle that can be used to access the fundamental catalogue (FK) data structure. 
    // <br>
    // This function is needed when calling the ThetaGrnwch function from TimeFunc.dll.
    // <br>
    // The handle returned by this function is sometimes called a pointer for historical reasons. The name EnvGetFkPtr comes from the fact that the handle used to be called a pointer.
    __int64 EnvGetFkPtr() const {

        __int64 fkPtr = p_EnvGetFkPtr();

        return fkPtr;
    }

    // Specifies the shape of the earth that will be used by the Astro Standards software, either spherical earth or oblate earth
    void EnvSetEarthShape(int earthShape) const {

        p_EnvSetEarthShape(earthShape);

    }

    // Returns the value representing the shape of the earth being used by the Astro Standards software, either spherical earth or oblate earth
    int EnvGetEarthShape() const {

        int earthShape = p_EnvGetEarthShape();

        return earthShape;
    }


private:
    EnvConstApi() : lib(resolveLibraryName()) {loadSymbols();}
    ~EnvConstApi() = default;

    using fn_EnvInit = int (STDCALL *)(__int64 apAddr);
    using fn_EnvGetInfo = void (STDCALL *)(char infoStr[128]);
    using fn_EnvLoadFile = int (STDCALL *)(char envFile[512]);
    using fn_EnvSaveFile = int (STDCALL *)(char envConstFile[512], int saveMode, int saveForm);
    using fn_EnvGetFkIdx = int (STDCALL *)();
    using fn_EnvSetFkIdx = void (STDCALL *)(int xf_FkMod);
    using fn_EnvGetGeoIdx = int (STDCALL *)();
    using fn_EnvSetGeoIdx = void (STDCALL *)(int xf_GeoMod);
    using fn_EnvGetGeoStr = void (STDCALL *)(char geoStr[6]);
    using fn_EnvSetGeoStr = void (STDCALL *)(char geoStr[6]);
    using fn_EnvGetGeoConst = double (STDCALL *)(int xf_GeoCon);
    using fn_EnvGetFkConst = double (STDCALL *)(int xf_FkCon);
    using fn_EnvGetFkPtr = __int64 (STDCALL *)();
    using fn_EnvSetEarthShape = void (STDCALL *)(int earthShape);
    using fn_EnvGetEarthShape = int (STDCALL *)();

    fn_EnvInit p_EnvInit{};
    fn_EnvGetInfo p_EnvGetInfo{};
    fn_EnvLoadFile p_EnvLoadFile{};
    fn_EnvSaveFile p_EnvSaveFile{};
    fn_EnvGetFkIdx p_EnvGetFkIdx{};
    fn_EnvSetFkIdx p_EnvSetFkIdx{};
    fn_EnvGetGeoIdx p_EnvGetGeoIdx{};
    fn_EnvSetGeoIdx p_EnvSetGeoIdx{};
    fn_EnvGetGeoStr p_EnvGetGeoStr{};
    fn_EnvSetGeoStr p_EnvSetGeoStr{};
    fn_EnvGetGeoConst p_EnvGetGeoConst{};
    fn_EnvGetFkConst p_EnvGetFkConst{};
    fn_EnvGetFkPtr p_EnvGetFkPtr{};
    fn_EnvSetEarthShape p_EnvSetEarthShape{};
    fn_EnvGetEarthShape p_EnvGetEarthShape{};

    SharedLibrary lib;

    static const char* resolveLibraryName() {
#ifdef _WIN32
        return "EnvConst.dll";
#elif __APPLE__
        return "libenvconst.dylib";
#else
        return "libenvconst.so";
#endif
    }

    void loadSymbols() {
        p_EnvInit = lib.load<fn_EnvInit>("EnvInit");
        p_EnvGetInfo = lib.load<fn_EnvGetInfo>("EnvGetInfo");
        p_EnvLoadFile = lib.load<fn_EnvLoadFile>("EnvLoadFile");
        p_EnvSaveFile = lib.load<fn_EnvSaveFile>("EnvSaveFile");
        p_EnvGetFkIdx = lib.load<fn_EnvGetFkIdx>("EnvGetFkIdx");
        p_EnvSetFkIdx = lib.load<fn_EnvSetFkIdx>("EnvSetFkIdx");
        p_EnvGetGeoIdx = lib.load<fn_EnvGetGeoIdx>("EnvGetGeoIdx");
        p_EnvSetGeoIdx = lib.load<fn_EnvSetGeoIdx>("EnvSetGeoIdx");
        p_EnvGetGeoStr = lib.load<fn_EnvGetGeoStr>("EnvGetGeoStr");
        p_EnvSetGeoStr = lib.load<fn_EnvSetGeoStr>("EnvSetGeoStr");
        p_EnvGetGeoConst = lib.load<fn_EnvGetGeoConst>("EnvGetGeoConst");
        p_EnvGetFkConst = lib.load<fn_EnvGetFkConst>("EnvGetFkConst");
        p_EnvGetFkPtr = lib.load<fn_EnvGetFkPtr>("EnvGetFkPtr");
        p_EnvSetEarthShape = lib.load<fn_EnvSetEarthShape>("EnvSetEarthShape");
        p_EnvGetEarthShape = lib.load<fn_EnvGetEarthShape>("EnvGetEarthShape");
    }
};

// ========================= End of auto generated code ==========================

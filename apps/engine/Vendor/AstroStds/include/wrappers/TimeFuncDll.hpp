// This wrapper file was generated automatically by the GenDllWrappers program.
#pragma once

#include "utils.hpp"
#include <string>
#include <vector>
#include <array>
#include <cstring>

class TimeFuncApi {
public:
    static TimeFuncApi& instance() {
        static TimeFuncApi inst;
        return inst;
    }
    TimeFuncApi(const TimeFuncApi&)              = delete;
    TimeFuncApi(TimeFuncApi&&)                   = delete;
    TimeFuncApi& operator = (const TimeFuncApi&) = delete;
    TimeFuncApi& operator = (TimeFuncApi&&)      = delete;


    // Available output date time group (DTG) formats
    // YYDDDHHMMSS.SSS              (DTG15)
    static constexpr int DTGFMT_DTG15   =  1;
    // YYYY/DDD.DDDDDDDD            (DTG17)
    static constexpr int DTGFMT_DTG17   =  2;
    // YYYYMonDDHHMMSS.SSS          (DTG19)
    static constexpr int DTGFMT_DTG19   =  3;
    // YYYY/DDD HHMM SS.SSS         (DTG20)
    static constexpr int DTGFMT_DTG20   =  4;
    // YYYY ddd HH MM SS.SSS        (DTG21)
    static constexpr int DTGFMT_DTG21   =  5;
    // YYYY-DDDTHH:MM:SS.SSSZ       (DTG22)
    static constexpr int DTGFMT_DTG22   =  6;
    // DD Mon YYYY HH:MM:SS.SSS     (DTG24)
    static constexpr int DTGFMT_DTG24   =  7;
    // YYYY/mm/DD HH:MM:SS.SSSSSS   (DTG25A)
    static constexpr int DTGFMT_DTG25A  =  8;
    // YYYY-mm-DD HH:MM:SS.SSSSSS   (DTG25B)
    static constexpr int DTGFMT_DTG25B  =  9;
    // YYYY ddd (DD Mon) HH:MM:SS.SSS  (DTG30)
    static constexpr int DTGFMT_DTG30   = 10;
    // ISO-8601 YYYY-MM-DDTHH:MM:SS.SSZ  (DTGISO)
    static constexpr int DTGFMT_DTGISO  = 11;

    // Notes: This function has been deprecated since v9.0. 
    // Initializes the TimeFunc DLL for use in the program. 
    // If this function returns an error, it is recommended that you stop the program immediately.
    // 
    // An error will occur if you forget to load and initialize all the prerequisite DLLs, as listed in the DLL Prerequisites section of the accompanying documentation, before using this DLL.
    int TimeFuncInit(__int64 apAddr) const {

        int errCode = p_TimeFuncInit(apAddr);

        return errCode;
    }

    // Returns the information about the TimeFunc DLL.  The information is placed in the string parameter you pass in.
    // The returned string provides information about the version number, build date, and the platform of the TimeFunc DLL.
    void TimeFuncGetInfo(std::string& infoStr) const {
        char infoStrBuf[129] = {};

        p_TimeFuncGetInfo(infoStrBuf);

        from_c(infoStrBuf, infoStr);
    }

    // Loads timing constants data from an input file.
    // Time constants can be included directly in the main input file or they can be read from a separate file identified with "TIMFIL=[pathname\filename]".
    // 
    // The input file is read in two passes. The function first looks for "TIMFIL=" lines, then it looks for timing constant data which was included directly. The result of this is that data entered using both methods will be processed, but the "TIMFIL=" data will be processed first.
    // 
    // The time constants are also read in from each VCM. However, only the most recent time constants among VCMs are stored in the memory, see VCM.dll documentation.
    // See the "Time Constants Data Description" section in the accompanying TimeFunc documentation file for supported formats.
    int TConLoadFile(const std::string& tconFile) const {
        char tconFileBuf[513] = {};
        to_c(tconFile, tconFileBuf);

        int errCode = p_TConLoadFile(tconFileBuf);

        return errCode;
    }

    // Loads timing constants data and prediction control (6P-card) from an input file.
    // Time constants can be included directly in the main input file or they can be read from a separate file identified with "TIMFIL=[pathname\filename]".
    // 
    // The input file is read in two passes. The function first looks for "TIMFIL=" lines, then it looks for timing constant data which was included directly. The result of this is that data entered using both methods will be processed, but the "TIMFIL=" data will be processed first.
    // 
    // The time constants are also read in from each VCM. However, only the most recent time constants among VCMs are stored in the memory, see VCM.dll documentation.
    int TimeFuncLoadFile(const std::string& tconFile) const {
        char tconFileBuf[513] = {};
        to_c(tconFile, tconFileBuf);

        int errCode = p_TimeFuncLoadFile(tconFileBuf);

        return errCode;
    }

    // Checks to see if timing constants have been loaded into memory.
    // The timing constants can be loaded from a timing constants file or from VCM(s).  See TConLoadFile, TConAddOne, and TConAddARec functions.
    int IsTConFileLoaded() const {

        int isFileLoaded = p_IsTConFileLoaded();

        return isFileLoaded;
    }

    // Saves currently loaded timing constants data to a file.
    // The data will be saved in the format specified by the form parameter, regardless of the format or method originally used to load it.
    int TConSaveFile(const std::string& tconFile, int saveMode, int saveForm) const {
        char tconFileBuf[513] = {};
        to_c(tconFile, tconFileBuf);

        int errCode = p_TConSaveFile(tconFileBuf, saveMode, saveForm);

        return errCode;
    }

    // Adds a timing constant record to memory. Note that this function is solely for backward compatible with legacy software.
    // Notes: only the latest timing record is stored in memory using this method. Input timing record will be skipped/ignored if it's earlier than the existing one
    // The users should use TConLoadFile or TimeFuncLoadFile to load timing constants file instead.
    int TConAddARec(double refDs50UTC, double leapDs50UTC, double taiMinusUTC, double ut1MinusUTC, double ut1Rate, double polarX, double polarY) const {

        int errCode = p_TConAddARec(refDs50UTC, leapDs50UTC, taiMinusUTC, ut1MinusUTC, ut1Rate, polarX, polarY);

        return errCode;
    }

    // Adds one timing constant record to memory. This API can be used to avoid TConLoadFile's file I/O
    int TConAddOne(double refDs50UTC, double taiMinusUTC, double ut1MinusUTC, double ut1Rate, double polarX, double polarY) const {

        int errCode = p_TConAddOne(refDs50UTC, taiMinusUTC, ut1MinusUTC, ut1Rate, polarX, polarY);

        return errCode;
    }

    // Retrieves the timing constants record, if exists, at the requested input time in ds50UTC.
    // If the requested record is not found, 0's will be returned for all of the constants. You can use this fact to determine whether the record was found or not. Simply check the taiMinusUTC value after calling this function. Since that value can never be 0 for a valid record, if it is 0 the record was not found.
    void UTCToTConRec(double ds50UTC, double& taiMinusUTC, double& ut1MinusUTC, double& ut1Rate, double& polarX, double& polarY) const {

        p_UTCToTConRec(ds50UTC, &taiMinusUTC, &ut1MinusUTC, &ut1Rate, &polarX, &polarY);

    }

    // Removes all the timing constants records in memory.
    int TConRemoveAll() const {

        int errCode = p_TConRemoveAll();

        return errCode;
    }

    // Converts an internal time in ds50UTC to a string in DTG20 format. The resulting string takes the form "YYYY/DDD HHMM SS.SSS".
    // The input ds50UTC must be greater than 2192.0, which corresponds to a time later than 1st Jan 1956. Any input value less than or equal to 2192.0 will yield "1956/001 0000 00.000".
    void UTCToDTG20(double ds50UTC, std::string& dtg20) const {
        char dtg20Buf[21] = {};

        p_UTCToDTG20(ds50UTC, dtg20Buf);

        from_c(dtg20Buf, dtg20);
    }

    // Converts a time in ds50UTC to a time in DTG19 format. The resulting string takes the form "YYYYMonDDHHMMSS.SSS".
    // See "UTCToDTG20" for an example.
    // The input ds50UTC must be greater than 2192.0, which corresponds to a time later than 1st Jan 1956. Any input value less than or equal to 2192.0 will yield "1956Jan01000000.000".
    // Note, the return value is in the DTG19 format "YYYYMonDDHHMMSS.SSS", not the "YY DDD HH MM SS.SSS" format.
    void UTCToDTG19(double ds50UTC, std::string& dtg19) const {
        char dtg19Buf[20] = {};

        p_UTCToDTG19(ds50UTC, dtg19Buf);

        from_c(dtg19Buf, dtg19);
    }

    // Converts a time in ds50UTC to a time in DTG17 format. The resulting string takes the form "YYYY/DDD.DDDDDDDD" format.
    // See "UTCToDTG20" for an example.
    // The input ds50UTC must be greater than 2192.0, which corresponds to a time later than 1st Jan 1956. Any input value less than or equal to 2192.0 will yield "1956/001.00000000".
    void UTCToDTG17(double ds50UTC, std::string& dtg17) const {
        char dtg17Buf[18] = {};

        p_UTCToDTG17(ds50UTC, dtg17Buf);

        from_c(dtg17Buf, dtg17);
    }

    // Converts a time in ds50UTC to a time in DTG15 format. The resulting string takes the form "YYDDDHHMMSS.SSS".
    // See "UTCToDTG20" for an example.
    // The input ds50UTC must be greater than 2192.0, which corresponds to a time later than 1st Jan 1956. Any input value less than or equal to 2192.0 will yield "56001000000.000".
    void UTCToDTG15(double ds50UTC, std::string& dtg15) const {
        char dtg15Buf[16] = {};

        p_UTCToDTG15(ds50UTC, dtg15Buf);

        from_c(dtg15Buf, dtg15);
    }

    // Converts an internal time in ds50UTC to the specified DTG format - dtgFmt 
    // The input ds50UTC must be greater than 2192.0, which corresponds to a time later than 1st Jan 1956. Any input value less than or equal to 2192.0 will yield "1956/001 0000 00.000".
    void UTCToDTG(double ds50UTC, int dtgFmt, std::string& outDtg) const {
        char outDtgBuf[33] = {};

        p_UTCToDTG(ds50UTC, dtgFmt, outDtgBuf);

        from_c(outDtgBuf, outDtg);
    }

    // Converts a time in one of the DTG formats to a time in ds50UTC. DTG15, DTG17, DTG19, and DTG20 formats are accepted.
    // See "UTCToDTG20" for an example.
    // During the conversion, this function processes only numbers and the '.' character. This means that you can format dtgStr in a format that makes sense. You can use spaces and the '/' character for example if you wish.
    // 
    // The function can process dates from 1950 to 2049. Any input outside this range will yield "0d0".
    // This function supports DTG19 inputs in both "YY DDD HH MM SS.SSS" and "YYYYMonDDHHMMSS.SSS" formats.
    double DTGToUTC(const std::string& dtg) const {
        char dtgBuf[21] = {};
        to_c(dtg, dtgBuf);

        double ds50UTC = p_DTGToUTC(dtgBuf);

        return ds50UTC;
    }

    // An extension to DTGToUTC to support newer time formats (up to 32-character long)
    // DD Mon YYYY HH:MM:SS.SSS
    // YYYY/mm/DD HH:MM:SS.SSS or YYYY-mm-DDTHH:MM:SS.SSS
    // YYYY/mm/DD HH:MM:SS.SSSSSS or YYYY-mm-DDTHH:MM:SS.SSSSSS
    // YYYY ddd (DD Mon) HH:MM:SS.SSS
    // YYYY-DDDTHH:MM:SS.SSSZ or YYYY ddd HH MM SS.SSS
    double DTGToUTCExt(const std::string& dtg) const {
        char dtgBuf[33] = {};
        to_c(dtg, dtgBuf);

        double ds50UTC = p_DTGToUTCExt(dtgBuf);

        return ds50UTC;
    }

    // Converts a time in ds50UTC to a time in ds50TAI using timing constants records in memory. 
    // If no timing constants records were loaded, ds50UTC and ds50TAI are the same.
    // Partial days may be returned.
    double UTCToTAI(double ds50UTC) const {

        double ds50TAI = p_UTCToTAI(ds50UTC);

        return ds50TAI;
    }

    // Converts a time in ds50UTC to a time in ds50UT1 using timing constants records in memory. 
    // If no timing constants records were loaded, ds50UTC and ds50UT1 are the same. 
    // Partial days may be returned.
    double UTCToUT1(double ds50UTC) const {

        double ds50UT1 = p_UTCToUT1(ds50UTC);

        return ds50UT1;
    }

    // Converts a time in ds50UTC to a time in ds50ET (Ephemeris Time/Terrestrial Time) using timing constants records in memory. 
    double UTCToET(double ds50UTC) const {

        double ds50ET = p_UTCToET(ds50UTC);

        return ds50ET;
    }

    // Converts a time in ds50TAI to a time in ds50UTC using timing constants records in memory. 
    // If no timing constants records were loaded, ds50TAI and ds50UTC are the same. 
    // Partial days may be returned.
    double TAIToUTC(double ds50TAI) const {

        double ds50UTC = p_TAIToUTC(ds50TAI);

        return ds50UTC;
    }

    // Converts a time in ds50TAI to a time in ds50UT1 using timing constants records in memory. 
    // If no timing constants records were loaded, ds50TAI and ds50UT1 are the same. 
    // Partial days may be returned.
    double TAIToUT1(double ds50TAI) const {

        double ds50UT1 = p_TAIToUT1(ds50TAI);

        return ds50UT1;
    }

    // Converts a year and a number of days to a time in ds50UTC. 
    // Partial days may be returned.
    double YrDaysToUTC(int year, double dayOfYear) const {

        double ds50UTC = p_YrDaysToUTC(year, dayOfYear);

        return ds50UTC;
    }

    // Converts a time in ds50UTC to a year and day of year.
    // The input ds50UTC must be greater than 2192.0, which corresponds to a time later than 1st Jan 1956. Any input value less than or equal to 2192.0 will yield Year=1956, Day=1.0.
    void UTCToYrDays(double ds50UTC, int& year, double& dayOfYear) const {

        p_UTCToYrDays(ds50UTC, &year, &dayOfYear);

    }

    // Converts a set of time components (year, day of year, hour, minute, second) to a time in ds50UTC. 
    // Partial days may be returned.
    // See "TimeComps2ToUTC" for a function which takes a month and day instead of a day of year value.
    double TimeComps1ToUTC(int year, int dayOfYear, int hh, int mm, double sss) const {

        double ds50UTC = p_TimeComps1ToUTC(year, dayOfYear, hh, mm, sss);

        return ds50UTC;
    }

    // Converts a time in ds50UTC to its individual components (year, day of year, hour, minute, second).
    // See "TimeComps1ToUTC" for an example.
    // The input ds50UTC must be greater than 2192.0, which corresponds to a time later than 1st Jan 1956. Any input value less than or equal to 2192.0 will be reset to that value.
    void UTCToTimeComps1(double ds50UTC, int& year, int& dayOfYear, int& hh, int& mm, double& sss) const {

        p_UTCToTimeComps1(ds50UTC, &year, &dayOfYear, &hh, &mm, &sss);

    }

    // Converts a set of time components (year, month, day of month, hour, minute, second) to a time in ds50UTC. 
    // Partial days may be returned.
    // See "TimeComps1ToUTC" for a function which takes a day of year value instead of a month and day.
    double TimeComps2ToUTC(int year, int mon, int dayOfMonth, int hh, int mm, double sss) const {

        double ds50UTC = p_TimeComps2ToUTC(year, mon, dayOfMonth, hh, mm, sss);

        return ds50UTC;
    }

    // Converts a time in ds50UTC to its individual components (year, month, day of month, hour, minute, second).
    // See "TimeComps1ToUTC" for an example.
    // The input ds50UTC must be greater than 2192.0, which corresponds to a time later than 1st Jan 1956. Any input value less than or equal to 2192.0 will be reset to that value.
    void UTCToTimeComps2(double ds50UTC, int& year, int& month, int& dayOfMonth, int& hh, int& mm, double& sss) const {

        p_UTCToTimeComps2(ds50UTC, &year, &month, &dayOfMonth, &hh, &mm, &sss);

    }

    // Computes right ascension of Greenwich at the specified time in ds50UT1. 
    // The Fk constants as you currently have them set up in EnvConst.dll are used.
    // EnvConst.dll is not marked as a direct dependency of TimeFunc.dll. However, it obviously must be loaded in order to be able to use this function since you must first obtain a handle via the EnvGetFkPtr() function.
    double ThetaGrnwch(double ds50UT1, __int64 lenvFk) const {

        double thetaG = p_ThetaGrnwch(ds50UT1, lenvFk);

        return thetaG;
    }

    // Computes right ascension of Greenwich at the specified time in ds50UT1 using the Fourth Fundamental Catalogue (FK4).
    // There is no need to load or initialize EnvConst.dll when computing right ascension using this function.
    double ThetaGrnwchFK4(double ds50UT1) const {

        double thetaG = p_ThetaGrnwchFK4(ds50UT1);

        return thetaG;
    }

    // Computes right ascension of Greenwich at the specified time in ds50UT1 using the Fifth Fundamental Catalogue (FK5).
    // There is no need to load or initialize EnvConst.dll when computing right ascension using this function.
    double ThetaGrnwchFK5(double ds50UT1) const {

        double thetaG = p_ThetaGrnwchFK5(ds50UT1);

        return thetaG;
    }

    // This function is intended for future use.  No information is currently available.
    // This function is intended for future use.  No information is currently available.
    void TimeConvFrTo(int funcIdx, const std::vector<double>& frArr, std::vector<double>& toArr) const {
        std::vector<double> frArrBuf = frArr;
        std::vector<double> toArrBuf(toArr.size());

        p_TimeConvFrTo(funcIdx, frArrBuf.data(), toArrBuf.data());

        toArr = std::move(toArrBuf);
    }

    // Returns prediction control parameters. The parameters are placed in the reference variables passed to this function.
    void Get6P(int& startFrEpoch, int& stopFrEpoch, double& startTime, double& stopTime, double& interval) const {

        p_Get6P(&startFrEpoch, &stopFrEpoch, &startTime, &stopTime, &interval);

    }

    // Sets prediction control parameters.
    void Set6P(int startFrEpoch, int stopFrEpoch, double startTime, double stopTime, double interval) const {

        p_Set6P(startFrEpoch, stopFrEpoch, startTime, stopTime, interval);

    }

    // Returns current prediction control parameters in form of a 6P-Card string.
    void Get6PCardLine(std::string& card6PLine) const {
        char card6PLineBuf[513] = {};

        p_Get6PCardLine(card6PLineBuf);

        from_c(card6PLineBuf, card6PLine);
    }

    // Returns the time span of the loaded timing constants - the earliest and latest of loaded timing constant records
    void TConTimeSpan(int& numOfRecs, double& frTimeDs50UTC, double& toTimeDs50UTC) const {

        p_TConTimeSpan(&numOfRecs, &frTimeDs50UTC, &toTimeDs50UTC);

    }


private:
    TimeFuncApi() : lib(resolveLibraryName()) {loadSymbols();}
    ~TimeFuncApi() = default;

    using fn_TimeFuncInit = int (STDCALL *)(__int64 apAddr);
    using fn_TimeFuncGetInfo = void (STDCALL *)(char infoStr[128]);
    using fn_TConLoadFile = int (STDCALL *)(char tconFile[512]);
    using fn_TimeFuncLoadFile = int (STDCALL *)(char tconFile[512]);
    using fn_IsTConFileLoaded = int (STDCALL *)();
    using fn_TConSaveFile = int (STDCALL *)(char tconFile[512], int saveMode, int saveForm);
    using fn_TConAddARec = int (STDCALL *)(double refDs50UTC, double leapDs50UTC, double taiMinusUTC, double ut1MinusUTC, double ut1Rate, double polarX, double polarY);
    using fn_TConAddOne = int (STDCALL *)(double refDs50UTC, double taiMinusUTC, double ut1MinusUTC, double ut1Rate, double polarX, double polarY);
    using fn_UTCToTConRec = void (STDCALL *)(double ds50UTC, double* taiMinusUTC, double* ut1MinusUTC, double* ut1Rate, double* polarX, double* polarY);
    using fn_TConRemoveAll = int (STDCALL *)();
    using fn_UTCToDTG20 = void (STDCALL *)(double ds50UTC, char dtg20[20]);
    using fn_UTCToDTG19 = void (STDCALL *)(double ds50UTC, char dtg19[19]);
    using fn_UTCToDTG17 = void (STDCALL *)(double ds50UTC, char dtg17[17]);
    using fn_UTCToDTG15 = void (STDCALL *)(double ds50UTC, char dtg15[15]);
    using fn_UTCToDTG = void (STDCALL *)(double ds50UTC, int dtgFmt, char outDtg[32]);
    using fn_DTGToUTC = double (STDCALL *)(char dtg[20]);
    using fn_DTGToUTCExt = double (STDCALL *)(char dtg[32]);
    using fn_UTCToTAI = double (STDCALL *)(double ds50UTC);
    using fn_UTCToUT1 = double (STDCALL *)(double ds50UTC);
    using fn_UTCToET = double (STDCALL *)(double ds50UTC);
    using fn_TAIToUTC = double (STDCALL *)(double ds50TAI);
    using fn_TAIToUT1 = double (STDCALL *)(double ds50TAI);
    using fn_YrDaysToUTC = double (STDCALL *)(int year, double dayOfYear);
    using fn_UTCToYrDays = void (STDCALL *)(double ds50UTC, int* year, double* dayOfYear);
    using fn_TimeComps1ToUTC = double (STDCALL *)(int year, int dayOfYear, int hh, int mm, double sss);
    using fn_UTCToTimeComps1 = void (STDCALL *)(double ds50UTC, int* year, int* dayOfYear, int* hh, int* mm, double* sss);
    using fn_TimeComps2ToUTC = double (STDCALL *)(int year, int mon, int dayOfMonth, int hh, int mm, double sss);
    using fn_UTCToTimeComps2 = void (STDCALL *)(double ds50UTC, int* year, int* month, int* dayOfMonth, int* hh, int* mm, double* sss);
    using fn_ThetaGrnwch = double (STDCALL *)(double ds50UT1, __int64 lenvFk);
    using fn_ThetaGrnwchFK4 = double (STDCALL *)(double ds50UT1);
    using fn_ThetaGrnwchFK5 = double (STDCALL *)(double ds50UT1);
    using fn_TimeConvFrTo = void (STDCALL *)(int funcIdx, double frArr[], double toArr[]);
    using fn_Get6P = void (STDCALL *)(int* startFrEpoch, int* stopFrEpoch, double* startTime, double* stopTime, double* interval);
    using fn_Set6P = void (STDCALL *)(int startFrEpoch, int stopFrEpoch, double startTime, double stopTime, double interval);
    using fn_Get6PCardLine = void (STDCALL *)(char card6PLine[512]);
    using fn_TConTimeSpan = void (STDCALL *)(int* numOfRecs, double* frTimeDs50UTC, double* toTimeDs50UTC);

    fn_TimeFuncInit p_TimeFuncInit{};
    fn_TimeFuncGetInfo p_TimeFuncGetInfo{};
    fn_TConLoadFile p_TConLoadFile{};
    fn_TimeFuncLoadFile p_TimeFuncLoadFile{};
    fn_IsTConFileLoaded p_IsTConFileLoaded{};
    fn_TConSaveFile p_TConSaveFile{};
    fn_TConAddARec p_TConAddARec{};
    fn_TConAddOne p_TConAddOne{};
    fn_UTCToTConRec p_UTCToTConRec{};
    fn_TConRemoveAll p_TConRemoveAll{};
    fn_UTCToDTG20 p_UTCToDTG20{};
    fn_UTCToDTG19 p_UTCToDTG19{};
    fn_UTCToDTG17 p_UTCToDTG17{};
    fn_UTCToDTG15 p_UTCToDTG15{};
    fn_UTCToDTG p_UTCToDTG{};
    fn_DTGToUTC p_DTGToUTC{};
    fn_DTGToUTCExt p_DTGToUTCExt{};
    fn_UTCToTAI p_UTCToTAI{};
    fn_UTCToUT1 p_UTCToUT1{};
    fn_UTCToET p_UTCToET{};
    fn_TAIToUTC p_TAIToUTC{};
    fn_TAIToUT1 p_TAIToUT1{};
    fn_YrDaysToUTC p_YrDaysToUTC{};
    fn_UTCToYrDays p_UTCToYrDays{};
    fn_TimeComps1ToUTC p_TimeComps1ToUTC{};
    fn_UTCToTimeComps1 p_UTCToTimeComps1{};
    fn_TimeComps2ToUTC p_TimeComps2ToUTC{};
    fn_UTCToTimeComps2 p_UTCToTimeComps2{};
    fn_ThetaGrnwch p_ThetaGrnwch{};
    fn_ThetaGrnwchFK4 p_ThetaGrnwchFK4{};
    fn_ThetaGrnwchFK5 p_ThetaGrnwchFK5{};
    fn_TimeConvFrTo p_TimeConvFrTo{};
    fn_Get6P p_Get6P{};
    fn_Set6P p_Set6P{};
    fn_Get6PCardLine p_Get6PCardLine{};
    fn_TConTimeSpan p_TConTimeSpan{};

    SharedLibrary lib;

    static const char* resolveLibraryName() {
#ifdef _WIN32
        return "TimeFunc.dll";
#elif __APPLE__
        return "libtimefunc.dylib";
#else
        return "libtimefunc.so";
#endif
    }

    void loadSymbols() {
        p_TimeFuncInit = lib.load<fn_TimeFuncInit>("TimeFuncInit");
        p_TimeFuncGetInfo = lib.load<fn_TimeFuncGetInfo>("TimeFuncGetInfo");
        p_TConLoadFile = lib.load<fn_TConLoadFile>("TConLoadFile");
        p_TimeFuncLoadFile = lib.load<fn_TimeFuncLoadFile>("TimeFuncLoadFile");
        p_IsTConFileLoaded = lib.load<fn_IsTConFileLoaded>("IsTConFileLoaded");
        p_TConSaveFile = lib.load<fn_TConSaveFile>("TConSaveFile");
        p_TConAddARec = lib.load<fn_TConAddARec>("TConAddARec");
        p_TConAddOne = lib.load<fn_TConAddOne>("TConAddOne");
        p_UTCToTConRec = lib.load<fn_UTCToTConRec>("UTCToTConRec");
        p_TConRemoveAll = lib.load<fn_TConRemoveAll>("TConRemoveAll");
        p_UTCToDTG20 = lib.load<fn_UTCToDTG20>("UTCToDTG20");
        p_UTCToDTG19 = lib.load<fn_UTCToDTG19>("UTCToDTG19");
        p_UTCToDTG17 = lib.load<fn_UTCToDTG17>("UTCToDTG17");
        p_UTCToDTG15 = lib.load<fn_UTCToDTG15>("UTCToDTG15");
        p_UTCToDTG = lib.load<fn_UTCToDTG>("UTCToDTG");
        p_DTGToUTC = lib.load<fn_DTGToUTC>("DTGToUTC");
        p_DTGToUTCExt = lib.load<fn_DTGToUTCExt>("DTGToUTCExt");
        p_UTCToTAI = lib.load<fn_UTCToTAI>("UTCToTAI");
        p_UTCToUT1 = lib.load<fn_UTCToUT1>("UTCToUT1");
        p_UTCToET = lib.load<fn_UTCToET>("UTCToET");
        p_TAIToUTC = lib.load<fn_TAIToUTC>("TAIToUTC");
        p_TAIToUT1 = lib.load<fn_TAIToUT1>("TAIToUT1");
        p_YrDaysToUTC = lib.load<fn_YrDaysToUTC>("YrDaysToUTC");
        p_UTCToYrDays = lib.load<fn_UTCToYrDays>("UTCToYrDays");
        p_TimeComps1ToUTC = lib.load<fn_TimeComps1ToUTC>("TimeComps1ToUTC");
        p_UTCToTimeComps1 = lib.load<fn_UTCToTimeComps1>("UTCToTimeComps1");
        p_TimeComps2ToUTC = lib.load<fn_TimeComps2ToUTC>("TimeComps2ToUTC");
        p_UTCToTimeComps2 = lib.load<fn_UTCToTimeComps2>("UTCToTimeComps2");
        p_ThetaGrnwch = lib.load<fn_ThetaGrnwch>("ThetaGrnwch");
        p_ThetaGrnwchFK4 = lib.load<fn_ThetaGrnwchFK4>("ThetaGrnwchFK4");
        p_ThetaGrnwchFK5 = lib.load<fn_ThetaGrnwchFK5>("ThetaGrnwchFK5");
        p_TimeConvFrTo = lib.load<fn_TimeConvFrTo>("TimeConvFrTo");
        p_Get6P = lib.load<fn_Get6P>("Get6P");
        p_Set6P = lib.load<fn_Set6P>("Set6P");
        p_Get6PCardLine = lib.load<fn_Get6PCardLine>("Get6PCardLine");
        p_TConTimeSpan = lib.load<fn_TConTimeSpan>("TConTimeSpan");
    }
};

// ========================= End of auto generated code ==========================

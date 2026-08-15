// This wrapper file was generated automatically by the GenDllWrappers program.
#pragma once

#include "utils.hpp"
#include <string>
#include <vector>
#include <array>
#include <cstring>

class DllMainApi {
public:
    static DllMainApi& instance() {
        static DllMainApi inst;
        return inst;
    }
    DllMainApi(const DllMainApi&)              = delete;
    DllMainApi(DllMainApi&&)                   = delete;
    DllMainApi& operator = (const DllMainApi&) = delete;
    DllMainApi& operator = (DllMainApi&&)      = delete;


    // log message string length
    static constexpr int LOGMSGLEN = 128;

    // DHN 06Feb12 - Increase file path length to 512 characters from 128 characters to handle longer file path
    static constexpr int FILEPATHLEN = 512;

    // DHN 10Feb12 - Uniformally using 512 characters to passing/receiving string in all Get/Set Field functions
    static constexpr int GETSETSTRLEN = 512;

    static constexpr int INFOSTRLEN = 128;

    // DHN 10Feb12 - All input card types' (elsets, ob, sensors, ...) can now have maximum of 512 characters
    static constexpr int INPUTCARDLEN = 512;

    // Different orbital element types
    // Element type - SGP Tle type 0
    static constexpr int ELTTYPE_TLE_SGP   = 1;
    // Element type - SGP4 Tle type 2
    static constexpr int ELTTYPE_TLE_SGP4  = 2;
    // Element type - SP Tle type 6
    static constexpr int ELTTYPE_TLE_SP    = 3;
    // Element type - SP Vector
    static constexpr int ELTTYPE_SPVEC_B1P = 4;
    // Element type - VCM
    static constexpr int ELTTYPE_VCM       = 5;
    // Element type - External ephemeris
    static constexpr int ELTTYPE_EXTEPH    = 6;
    // Element type - SGP Tle type 4 - XP
    static constexpr int ELTTYPE_TLE_XP    = 7;
    // Element type - PPT3 Tle type 3 - XP
    static constexpr int ELTTYPE_TLE_PPT3  = 8;

    //*******************************************************************************

    // Propagation types
    // GP/SGP4/SGP4-XP propagator
    static constexpr int PROPTYPE_GP  = 1;
    // SP propagator
    static constexpr int PROPTYPE_SP  = 2;
    // External ephemeris
    static constexpr int PROPTYPE_X   = 3;
    // Unknown
    static constexpr int PROPTYPE_UK  = 4;
    //*******************************************************************************

    // Add sat error
    // Bad satellite key
    static constexpr int BADSATKEY = -1;
    // Bad satellite number
    static constexpr int BADSATNUM = -1;
    // Duplicate satellite key
    static constexpr int DUPSATKEY =  0;

    //*******************************************************************************

    // satellite/observation/sensor key possible errors
    // Bad (satellite/observation/sensor) key
    static constexpr int BADKEY = -1;
    // Duplicate (satellite/observation/sensor) key
    static constexpr int DUPKEY =  0;

    //*******************************************************************************

    // Options used in GetLoaded()
    // ascending order
    static constexpr int IDX_ORDER_ASC   = 0;
    // descending order
    static constexpr int IDX_ORDER_DES   = 1;
    // order as read
    static constexpr int IDX_ORDER_READ  = 2;
    // tree traversal order
    static constexpr int IDX_ORDER_QUICK = 9;

    //*******************************************************************************

    // Different key mode options for all elset-satKey/obs-obsKey/sensor-senKey
    // Default - duplicate elsets/observations/sensors can not be loaded in their binary trees
    static constexpr int ALL_KEYMODE_NODUP  = 0;
    // Allow duplicate elsets/obs/sensor to be loaded and have direct memory access (DMA - no duplication check and no binary tree)
    static constexpr int ALL_KEYMODE_DMA    = 1;

    //*******************************************************************************


    // Different key mode options for elset satKey
    // Default - duplicate elsets can not be loaded in binary tree
    static constexpr int ELSET_KEYMODE_NODUP  = 0;
    // Allow duplicate elsets to be loaded and have direct memory access (DMA - no duplication check and no binary tree)
    static constexpr int ELSET_KEYMODE_DMA    = 1;

    //*******************************************************************************

    // Different duplication key mode options (apply to non DMA mode only)
    // Returning (satellite/sensor/obs) key is zero to signify the existing data/key was already in memory
    static constexpr int DUPKEY_ZERO   = 0;
    // Return actual (satellite/sensor/obs) key regardless of the key/data duplication
    static constexpr int DUPKEY_ACTUAL = 1;

    //*******************************************************************************


    // Input time is in minutes since epoch
    static constexpr int TIME_IS_MSE = 1;
    // Input time is in days since 1950 TAI
    static constexpr int TIME_IS_TAI = 2;
    // Input time is in days since 1950 UTC
    static constexpr int TIME_IS_UTC = 3;

    //*******************************************************************************

    // Notes: This function has been deprecated since v9.0. 
    // Initializes DllMain program (host of Astro Standards libraries) for use in any Astro Standards applications
    __int64 DllMainInit() const {

        __int64 apAddr = p_DllMainInit();

        return apAddr;
    }

    // Returns information about the DllMain DLL. 
    // The returned string provides information about the version number, build date, and the platform. 
    void DllMainGetInfo(std::string& infoStr) const {
        char infoStrBuf[129] = {};

        p_DllMainGetInfo(infoStrBuf);

        from_c(infoStrBuf, infoStr);
    }

    // Loads DllMain-related parameters (AS_MOIC) from a text file
    int DllMainLoadFile(const std::string& dllMainFile) const {
        char dllMainFileBuf[513] = {};
        to_c(dllMainFile, dllMainFileBuf);

        int errCode = p_DllMainLoadFile(dllMainFileBuf);

        return errCode;
    }

    // Opens a log file and enables the writing of diagnostic information into it. 
    // All of the DLLs in the library will write diagnostic information into the log file once this function has been called.
    // If the file specified by logFileName already exists, its contents are erased.
    // 
    // Enabling logging can potentially result in large amounts of diagnostic information being generated, which can lead to large amounts of storage being consumed as well as performance decreases. For this reason, it is recommended that this function only be used for debugging purposes.
    int OpenLogFile(const std::string& fileName) const {
        char fileNameBuf[513] = {};
        to_c(fileName, fileNameBuf);

        int errCode = p_OpenLogFile(fileNameBuf);

        return errCode;
    }

    // Closes the currently open log file and reset the last logged error message to null. 
    // Remember to close the log file before exiting the program.
    void CloseLogFile() const {

        p_CloseLogFile();

    }

    // Writes a message into the log file.
    // Make sure the log file is open by calling OpenLogFile before using this function.
    // 
    // The message is limited to 128 characters. If the message is longer than this, it will be truncated.
    void LogMessage(const std::string& msgStr) const {
        char msgStrBuf[129] = {};
        to_c(msgStr, msgStrBuf);

        p_LogMessage(msgStrBuf);

    }

    // Returns a character string describing the last error that occurred. 
    // As a common practice, this function is called to retrieve the error message when an error occurs.
    // 
    // This function works with or without an opened log file.
    // 
    // If you call this function before you have called DllMainInit(), the function will return an invalid string. This could result in undefined behavior.
    void GetLastErrMsg(std::string& lastErrMsg) const {
        char lastErrMsgBuf[129] = {};

        p_GetLastErrMsg(lastErrMsgBuf);

        from_c(lastErrMsgBuf, lastErrMsg);
    }

    // Returns a character string describing the last informational message that was recorded. 
    // This function is usually called right after space objects (TLEs, VCMs, sensors, observations, etc.) in an input text file were loaded. It gives information about how many records were successfully loaded, how many were bad, and how many were duplicated.
    // 
    // This function works with or without an opened log file.
    // 
    // If you call this function before you have called DllMainInit(), the function will return an invalid string. This could result in undefined behavior.
    // This function provides a quick way to check whether all of the prerequisite DLLs have been loaded and initialized correctly. Improper initialization of the Standardized Astrodynamic Algorithms DLLs is one of the most common causes of program crashes.
    void GetLastInfoMsg(std::string& lastInfoMsg) const {
        char lastInfoMsgBuf[129] = {};

        p_GetLastInfoMsg(lastInfoMsgBuf);

        from_c(lastInfoMsgBuf, lastInfoMsg);
    }

    // Notes: This function has been deprecated since v9.0. 
    // Returns a list of names of the Standardized Astrodynamic Algorithms DLLs that were initialized successfully.
    void GetInitDllNames(std::string& initDllNames) const {
        char initDllNamesBuf[513] = {};

        p_GetInitDllNames(initDllNamesBuf);

        from_c(initDllNamesBuf, initDllNames);
    }

    // Tests different input/output data types that are supported by the Astrodynamic Standards library.
    void TestInterface(char cIn, char& cOut, int intIn, int& intOut, __int64 longIn, __int64& longOut, double realIn, double& realOut, const std::string& strIn, std::string& strOut, const std::array<int,3>& int1DIn, std::array<int,3>& int1DOut, const std::array<__int64,3>& long1DIn, std::array<__int64,3>& long1DOut, const std::array<double,3>& real1DIn, std::array<double,3>& real1DOut, const std::array<std::array<int,3>,2>& int2DIn, std::array<std::array<int,3>,2>& int2DOut, const std::array<std::array<__int64,3>,2>& long2DIn, std::array<std::array<__int64,3>,2>& long2DOut, const std::array<std::array<double,3>,2>& real2DIn, std::array<std::array<double,3>,2>& real2DOut) const {
        char strInBuf[513] = {};
        to_c(strIn, strInBuf);
        char strOutBuf[513] = {};

        p_TestInterface(cIn, &cOut, intIn, &intOut, longIn, &longOut, realIn, &realOut, strInBuf, strOutBuf, const_cast<int*>(int1DIn.data()), int1DOut.data(), const_cast<__int64*>(long1DIn.data()), long1DOut.data(), const_cast<double*>(real1DIn.data()), real1DOut.data(), const_cast<int(*)[3]>(reinterpret_cast<const int(*)[3]>(int2DIn.data())), reinterpret_cast<int(*)[3]>(int2DOut.data()), const_cast<__int64(*)[3]>(reinterpret_cast<const __int64(*)[3]>(long2DIn.data())), reinterpret_cast<__int64(*)[3]>(long2DOut.data()), const_cast<double(*)[3]>(reinterpret_cast<const double(*)[3]>(real2DIn.data())), reinterpret_cast<double(*)[3]>(real2DOut.data()));

        from_c(strOutBuf, strOut);
    }

    // Tests different input/output data types that are supported by the Astrodynamic Standards library.
    void TestInterface2(char& cInOut, int& intInOut, __int64& longInOut, double& realInOut, std::string& strInOut, std::array<int,3>& int1DInOut, std::array<__int64,3>& long1DInOut, std::array<double,3>& real1DInOut, std::array<std::array<int,3>,2>& int2DInOut, std::array<std::array<__int64,3>,2>& long2DInOut, std::array<std::array<double,3>,2>& real2DInOut) const {
        char strInOutBuf[513] = {};
        to_c(strInOut, strInOutBuf);

        p_TestInterface2(&cInOut, &intInOut, &longInOut, &realInOut, strInOutBuf, int1DInOut.data(), long1DInOut.data(), real1DInOut.data(), reinterpret_cast<int(*)[3]>(int2DInOut.data()), reinterpret_cast<__int64(*)[3]>(long2DInOut.data()), reinterpret_cast<double(*)[3]>(real2DInOut.data()));

        from_c(strInOutBuf, strInOut);
    }

    // Tests input and output arrays with unknown length that are supported by the Astrodynamic Standards library.
    void TestInterface3(const std::vector<int>& unk1DIn, std::vector<int>& unk1DOut, const std::vector<std::array<int,3>>& unk2DIn, std::vector<std::array<int,3>>& unk2DOut) const {
        std::vector<int> unk1DInBuf = unk1DIn;
        std::vector<int> unk1DOutBuf(unk1DOut.size());
        std::vector<std::array<int,3>> unk2DInBuf = unk2DIn;
        std::vector<std::array<int,3>> unk2DOutBuf(unk2DOut.size());

        p_TestInterface3(unk1DInBuf.data(), unk1DOutBuf.data(), reinterpret_cast<int(*)[3]>(unk2DInBuf.data()), reinterpret_cast<int(*)[3]>(unk2DOutBuf.data()));

        unk1DOut = std::move(unk1DOutBuf);
        unk2DOut = std::move(unk2DOutBuf);
    }

    // Returns data parsed from user's AS_MOIC-typed input card - up to 128 fields are allowed.
    void GetMOICData(int arrSize, std::vector<double>& xa_moic) const {
        std::vector<double> xa_moicBuf(xa_moic.size());

        p_GetMOICData(arrSize, xa_moicBuf.data());

        xa_moic = std::move(xa_moicBuf);
    }

    // Sets ELSET key mode
    // This mode can also be turned on if the user loads an input text file that includes this line - "AS_DMA_ON" -
    // and is currently calling any of these methods: DllMainLoadFile(), TleLoadFile(), SpVecLoadFile(), or VcmLoadFile()
    int SetElsetKeyMode(int elset_keyMode) const {

        int errCode = p_SetElsetKeyMode(elset_keyMode);

        return errCode;
    }

    // Gets current ELSET key mode
    int GetElsetKeyMode() const {

        int elset_keyMode = p_GetElsetKeyMode();

        return elset_keyMode;
    }

    // Sets key mode for ALL (elsets/obs/sensors). This takes precedence over individual elset/obs/sensor key mode
    // This mode can also be turned on if the user loads an input text file that includes this line - "AS_DMA_ALL_ON"
    int SetAllKeyMode(int all_keyMode) const {

        int errCode = p_SetAllKeyMode(all_keyMode);

        return errCode;
    }

    // Gets current ALL (elsets/obs/sensors) key mode
    int GetAllKeyMode() const {

        int all_keyMode = p_GetAllKeyMode();

        return all_keyMode;
    }

    // Resets ALL (elsets/obs/sensors) key mode to its default value which then allows individual elsets/obs/sensors to use their own key mode settings.
    // Also reset DUPLICATION key mode to its default value.
    void ResetAllKeyMode() const {

        p_ResetAllKeyMode();

    }

    // Sets DUPLICATION key mode - change the default behavior of returning a key which already exists in memory: zero versus actual value
    int SetDupKeyMode(int dupKeyMode) const {

        int errCode = p_SetDupKeyMode(dupKeyMode);

        return errCode;
    }

    // Gets current DUPLICATION key mode
    int GetDupKeyMode() const {

        int dupKeyMode = p_GetDupKeyMode();

        return dupKeyMode;
    }

    // Retrieves error message associated with the input errCode. 
    // As a common practice, this function is called to retrieve the error message when an error occurs.
    void GetErrMsg(int errCode, std::string& errMsg) const {
        char errMsgBuf[129] = {};

        p_GetErrMsg(errCode, errMsgBuf);

        from_c(errMsgBuf, errMsg);
    }

    // Sets error message mode 
    // mode = 0: Only last error message is stored and then retrieved via GetLastErrMsg(). Appropriate for single-thread applications
    // mode = 1: Each thread stores and retrieves its own error message via GetErrMsg(). Appropriate for multi-thread applications
    int SetErrMsgMode(int mode) const {

        int errCode = p_SetErrMsgMode(mode);

        return errCode;
    }

    // Gets error message mode 
    // mode = 0: Only last error message is stored and then retrieved via GetLastErrMsg(). Appropriate for single-thread applications
    // mode = 1: Each thread stores and retrieves its own error message via GetErrMsg(). Appropriate for multi-thread applications
    int GetErrMsgMode() const {

        int mode = p_GetErrMsgMode();

        return mode;
    }


private:
    DllMainApi() : lib(resolveLibraryName()) {loadSymbols();}
    ~DllMainApi() = default;

    using fn_DllMainInit = __int64 (STDCALL *)();
    using fn_DllMainGetInfo = void (STDCALL *)(char infoStr[128]);
    using fn_DllMainLoadFile = int (STDCALL *)(char dllMainFile[512]);
    using fn_OpenLogFile = int (STDCALL *)(char fileName[512]);
    using fn_CloseLogFile = void (STDCALL *)();
    using fn_LogMessage = void (STDCALL *)(char msgStr[128]);
    using fn_GetLastErrMsg = void (STDCALL *)(char lastErrMsg[128]);
    using fn_GetLastInfoMsg = void (STDCALL *)(char lastInfoMsg[128]);
    using fn_GetInitDllNames = void (STDCALL *)(char initDllNames[512]);
    using fn_TestInterface = void (STDCALL *)(char cIn, char* cOut, int intIn, int* intOut, __int64 longIn, __int64* longOut, double realIn, double* realOut, char strIn[512], char strOut[512], int int1DIn[3], int int1DOut[3], __int64 long1DIn[3], __int64 long1DOut[3], double real1DIn[3], double real1DOut[3], int int2DIn[2][3], int int2DOut[2][3], __int64 long2DIn[2][3], __int64 long2DOut[2][3], double real2DIn[2][3], double real2DOut[2][3]);
    using fn_TestInterface2 = void (STDCALL *)(char* cInOut, int* intInOut, __int64* longInOut, double* realInOut, char strInOut[512], int int1DInOut[3], __int64 long1DInOut[3], double real1DInOut[3], int int2DInOut[2][3], __int64 long2DInOut[2][3], double real2DInOut[2][3]);
    using fn_TestInterface3 = void (STDCALL *)(int unk1DIn[], int unk1DOut[], int unk2DIn[][3], int unk2DOut[][3]);
    using fn_GetMOICData = void (STDCALL *)(int arrSize, double xa_moic[]);
    using fn_SetElsetKeyMode = int (STDCALL *)(int elset_keyMode);
    using fn_GetElsetKeyMode = int (STDCALL *)();
    using fn_SetAllKeyMode = int (STDCALL *)(int all_keyMode);
    using fn_GetAllKeyMode = int (STDCALL *)();
    using fn_ResetAllKeyMode = void (STDCALL *)();
    using fn_SetDupKeyMode = int (STDCALL *)(int dupKeyMode);
    using fn_GetDupKeyMode = int (STDCALL *)();
    using fn_GetErrMsg = void (STDCALL *)(int errCode, char errMsg[128]);
    using fn_SetErrMsgMode = int (STDCALL *)(int mode);
    using fn_GetErrMsgMode = int (STDCALL *)();

    fn_DllMainInit p_DllMainInit{};
    fn_DllMainGetInfo p_DllMainGetInfo{};
    fn_DllMainLoadFile p_DllMainLoadFile{};
    fn_OpenLogFile p_OpenLogFile{};
    fn_CloseLogFile p_CloseLogFile{};
    fn_LogMessage p_LogMessage{};
    fn_GetLastErrMsg p_GetLastErrMsg{};
    fn_GetLastInfoMsg p_GetLastInfoMsg{};
    fn_GetInitDllNames p_GetInitDllNames{};
    fn_TestInterface p_TestInterface{};
    fn_TestInterface2 p_TestInterface2{};
    fn_TestInterface3 p_TestInterface3{};
    fn_GetMOICData p_GetMOICData{};
    fn_SetElsetKeyMode p_SetElsetKeyMode{};
    fn_GetElsetKeyMode p_GetElsetKeyMode{};
    fn_SetAllKeyMode p_SetAllKeyMode{};
    fn_GetAllKeyMode p_GetAllKeyMode{};
    fn_ResetAllKeyMode p_ResetAllKeyMode{};
    fn_SetDupKeyMode p_SetDupKeyMode{};
    fn_GetDupKeyMode p_GetDupKeyMode{};
    fn_GetErrMsg p_GetErrMsg{};
    fn_SetErrMsgMode p_SetErrMsgMode{};
    fn_GetErrMsgMode p_GetErrMsgMode{};

    SharedLibrary lib;

    static const char* resolveLibraryName() {
#ifdef _WIN32
        return "DllMain.dll";
#elif __APPLE__
        return "libdllmain.dylib";
#else
        return "libdllmain.so";
#endif
    }

    void loadSymbols() {
        p_DllMainInit = lib.load<fn_DllMainInit>("DllMainInit");
        p_DllMainGetInfo = lib.load<fn_DllMainGetInfo>("DllMainGetInfo");
        p_DllMainLoadFile = lib.load<fn_DllMainLoadFile>("DllMainLoadFile");
        p_OpenLogFile = lib.load<fn_OpenLogFile>("OpenLogFile");
        p_CloseLogFile = lib.load<fn_CloseLogFile>("CloseLogFile");
        p_LogMessage = lib.load<fn_LogMessage>("LogMessage");
        p_GetLastErrMsg = lib.load<fn_GetLastErrMsg>("GetLastErrMsg");
        p_GetLastInfoMsg = lib.load<fn_GetLastInfoMsg>("GetLastInfoMsg");
        p_GetInitDllNames = lib.load<fn_GetInitDllNames>("GetInitDllNames");
        p_TestInterface = lib.load<fn_TestInterface>("TestInterface");
        p_TestInterface2 = lib.load<fn_TestInterface2>("TestInterface2");
        p_TestInterface3 = lib.load<fn_TestInterface3>("TestInterface3");
        p_GetMOICData = lib.load<fn_GetMOICData>("GetMOICData");
        p_SetElsetKeyMode = lib.load<fn_SetElsetKeyMode>("SetElsetKeyMode");
        p_GetElsetKeyMode = lib.load<fn_GetElsetKeyMode>("GetElsetKeyMode");
        p_SetAllKeyMode = lib.load<fn_SetAllKeyMode>("SetAllKeyMode");
        p_GetAllKeyMode = lib.load<fn_GetAllKeyMode>("GetAllKeyMode");
        p_ResetAllKeyMode = lib.load<fn_ResetAllKeyMode>("ResetAllKeyMode");
        p_SetDupKeyMode = lib.load<fn_SetDupKeyMode>("SetDupKeyMode");
        p_GetDupKeyMode = lib.load<fn_GetDupKeyMode>("GetDupKeyMode");
        p_GetErrMsg = lib.load<fn_GetErrMsg>("GetErrMsg");
        p_SetErrMsgMode = lib.load<fn_SetErrMsgMode>("SetErrMsgMode");
        p_GetErrMsgMode = lib.load<fn_GetErrMsgMode>("GetErrMsgMode");
    }
};

// ========================= End of auto generated code ==========================

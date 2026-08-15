#pragma once

#include "DllMainDll.hpp"
#include "EnvConstDll.hpp"
#include "TimeFuncDll.hpp"
#include "AstroFuncDll.hpp"
#include "TleDll.hpp"
#include "Sgp4PropDll.hpp"
// Add others as needed

// ------------------------------------------------------------
// Unified Astro API bundle (RAII)
// ------------------------------------------------------------
class AstroApi {
public:
    // Singleton Accessor
    static AstroApi& instance() {
        static AstroApi inst;
        return inst;
    }
    AstroApi(const AstroApi&)              = delete;
    AstroApi(AstroApi&&)                   = delete;
    AstroApi& operator = (const AstroApi&) = delete;
    AstroApi& operator = (AstroApi&&)      = delete;

    // Public accessor for each Dll wrapper
    DllMainApi&   DllMain()   {return DllMainApi::instance(); };
    EnvConstApi&  EnvConst()  {return EnvConstApi::instance(); };
    TimeFuncApi&  TimeFunc()  {return TimeFuncApi::instance(); };
    AstroFuncApi& AstroFunc() {return AstroFuncApi::instance(); };
    TleApi&       Tle()       {return TleApi::instance(); };
    Sgp4PropApi&  Sgp4Prop()  {return Sgp4PropApi::instance(); };
    
private:
    AstroApi() {
        DllMainApi::instance();
        EnvConstApi::instance();
        TimeFuncApi::instance();
        AstroFuncApi::instance();
        TleApi::instance();
        Sgp4PropApi::instance();
    }
};


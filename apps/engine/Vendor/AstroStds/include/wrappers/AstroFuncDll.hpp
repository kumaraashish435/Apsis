// This wrapper file was generated automatically by the GenDllWrappers program.
#pragma once

#include "utils.hpp"
#include <string>
#include <vector>
#include <array>
#include <cstring>

class AstroFuncApi {
public:
    static AstroFuncApi& instance() {
        static AstroFuncApi inst;
        return inst;
    }
    AstroFuncApi(const AstroFuncApi&)              = delete;
    AstroFuncApi(AstroFuncApi&&)                   = delete;
    AstroFuncApi& operator = (const AstroFuncApi&) = delete;
    AstroFuncApi& operator = (AstroFuncApi&&)      = delete;


    // Index of Keplerian elements
    // semi-major axis (km)
    static constexpr int XA_KEP_A     =   0;
    // eccentricity (unitless)
    static constexpr int XA_KEP_E     =   1;
    // inclination (deg)
    static constexpr int XA_KEP_INCLI =   2;
    // mean anomaly (deg)
    static constexpr int XA_KEP_MA    =   3;
    // right ascension of the asending node (deg)
    static constexpr int XA_KEP_NODE  =   4;
    // argument of perigee (deg)
    static constexpr int XA_KEP_OMEGA =   5;
    static constexpr int XA_KEP_SIZE  =   6;

    // Index of classical elements
    // N mean motion (revs/day)
    static constexpr int XA_CLS_N     =   0;
    // eccentricity (unitless)
    static constexpr int XA_CLS_E     =   1;
    // inclination (deg)
    static constexpr int XA_CLS_INCLI =   2;
    // mean anomaly (deg)
    static constexpr int XA_CLS_MA    =   3;
    // right ascension of the asending node (deg)
    static constexpr int XA_CLS_NODE  =   4;
    // argument of perigee (deg)
    static constexpr int XA_CLS_OMEGA =   5;
    static constexpr int XA_CLS_SIZE  =   6;

    // Index of equinoctial elements
    // Af (unitless)
    static constexpr int XA_EQNX_AF   =   0;
    // Ag (unitless)
    static constexpr int XA_EQNX_AG   =   1;
    // chi (unitless)
    static constexpr int XA_EQNX_CHI  =   2;
    // psi (unitless)
    static constexpr int XA_EQNX_PSI  =   3;
    // L mean longitude (deg)
    static constexpr int XA_EQNX_L    =   4;
    // N mean motion (revs/day)
    static constexpr int XA_EQNX_N    =   5;
    static constexpr int XA_EQNX_SIZE =   6;

    // Indexes of AstroConvFrTo
    // SGP4 (A, E, Incli, BStar) to SGP (nDot, n2Dot)
    static constexpr int XF_CONV_SGP42SGP = 101;


    // Indexes for topocentric components
    // Right ascension (deg)
    static constexpr int XA_TOPO_RA    = 0;
    // Declination (deg)
    static constexpr int XA_TOPO_DEC   = 1;
    // Azimuth (deg)
    static constexpr int XA_TOPO_AZ    = 2;
    // Elevation (deg)
    static constexpr int XA_TOPO_EL    = 3;
    // Range (km)
    static constexpr int XA_TOPO_RANGE = 4;
    // Right ascension dot (deg/s)
    static constexpr int XA_TOPO_RADOT = 5;
    // Declincation dot (deg/s)
    static constexpr int XA_TOPO_DECDOT= 6;
    // Azimuth dot (deg/s)
    static constexpr int XA_TOPO_AZDOT = 7;
    // Elevation dot (deg/s)
    static constexpr int XA_TOPO_ELDOT = 8;
    // Range dot (km/s)
    static constexpr int XA_TOPO_RANGEDOT = 9;
    static constexpr int XA_TOPO_SIZE  = 10;


    // Indexes for RAE components
    // Range (km)
    static constexpr int XA_RAE_RANGE   = 0;
    // Azimuth (deg)
    static constexpr int XA_RAE_AZ      = 1;
    // Elevation (deg)
    static constexpr int XA_RAE_EL      = 2;
    // Range dot (km/s)
    static constexpr int XA_RAE_RANGEDOT= 3;
    // Azimuth dot (deg/s)
    static constexpr int XA_RAE_AZDOT   = 4;
    // Elevation dot (deg/s)
    static constexpr int XA_RAE_ELDOT   = 5;
    static constexpr int XA_RAE_SIZE    = 6;


    // Year of Equinox indicator
    // Date of observation
    static constexpr int YROFEQNX_OBTIME = 0;
    // 0 Jan of Date
    static constexpr int YROFEQNX_CURR   = 1;
    // J2000
    static constexpr int YROFEQNX_2000   = 2;
    // B1950
    static constexpr int YROFEQNX_1950   = 3;

    // Notes: This function has been deprecated since v9.0. 
    // Initializes AstroFunc DLL for use in the program.
    // If this function returns an error, it is recommended that you stop the program immediately.
    // 
    // An error will occur if you forget to load and initialize all the prerequisite DLLs, as listed in the DLL Prerequisites section of the accompanying documentation, before using this DLL.
    int AstroFuncInit(__int64 apAddr) const {

        int errCode = p_AstroFuncInit(apAddr);

        return errCode;
    }

    // Retrieves information about the current version of AstroFunc.dll. The information is placed in the string parameter you pass in.
    // The returned string provides information about the version number, build date, and platform.
    void AstroFuncGetInfo(std::string& infoStr) const {
        char infoStrBuf[129] = {};

        p_AstroFuncGetInfo(infoStrBuf);

        from_c(infoStrBuf, infoStr);
    }

    // Converts a set of Keplerian elements to a set of equinoctial elements. 
    void KepToEqnx(const std::array<double,6>& xa_kep, std::array<double,6>& xa_eqnx) const {

        p_KepToEqnx(const_cast<double*>(xa_kep.data()), xa_eqnx.data());

    }

    // Converts a set of osculating Keplerian elements to osculating position and velocity vectors.
    void KepToPosVel(const std::array<double,6>& xa_kep, std::array<double,3>& pos, std::array<double,3>& vel) const {

        p_KepToPosVel(const_cast<double*>(xa_kep.data()), pos.data(), vel.data());

    }

    // Converts a set of Keplerian elements to Ubar, Vbar, and Wbar vectors.
    void KepToUVW(const std::array<double,6>& xa_kep, std::array<double,3>& uBar, std::array<double,3>& vBar, std::array<double,3>& wBar) const {

        p_KepToUVW(const_cast<double*>(xa_kep.data()), uBar.data(), vBar.data(), wBar.data());

    }

    // Converts a set of classical elements to a set of equinoctial elements. 
    void ClassToEqnx(const std::array<double,6>& xa_cls, std::array<double,6>& xa_eqnx) const {

        p_ClassToEqnx(const_cast<double*>(xa_cls.data()), xa_eqnx.data());

    }

    // Converts a set of equinoctial elements to a set of classical elements.
    void EqnxToClass(const std::array<double,6>& xa_eqnx, std::array<double,6>& xa_cls) const {

        p_EqnxToClass(const_cast<double*>(xa_eqnx.data()), xa_cls.data());

    }

    // Converts a set of equinoctial elements to a set of Keplerian elements. 
    void EqnxToKep(const std::array<double,6>& xa_eqnx, std::array<double,6>& xa_kep) const {

        p_EqnxToKep(const_cast<double*>(xa_eqnx.data()), xa_kep.data());

    }

    // Converts a set of equinoctial elements to position and velocity vectors.
    void EqnxToPosVel(const std::array<double,6>& xa_eqnx, std::array<double,3>& pos, std::array<double,3>& vel) const {

        p_EqnxToPosVel(const_cast<double*>(xa_eqnx.data()), pos.data(), vel.data());

    }

    // Converts position and velocity vectors to a set of equinoctial elements.
    void PosVelToEqnx(const std::array<double,3>& pos, const std::array<double,3>& vel, std::array<double,6>& xa_eqnx) const {

        p_PosVelToEqnx(const_cast<double*>(pos.data()), const_cast<double*>(vel.data()), xa_eqnx.data());

    }

    // Converts position and velocity vectors to a set of equinoctial elements with the given mu value. 
    // This function is used when working with the SP propagator to get a more accurate set of equinoctial elements.
    void PosVelMuToEqnx(const std::array<double,3>& pos, const std::array<double,3>& vel, double mu, std::array<double,6>& xa_eqnx) const {

        p_PosVelMuToEqnx(const_cast<double*>(pos.data()), const_cast<double*>(vel.data()), mu, xa_eqnx.data());

    }

    // Converts osculating position and velocity vectors to a set of osculating Keplerian elements.
    void PosVelToKep(const std::array<double,3>& pos, const std::array<double,3>& vel, std::array<double,6>& xa_kep) const {

        p_PosVelToKep(const_cast<double*>(pos.data()), const_cast<double*>(vel.data()), xa_kep.data());

    }

    // Converts osculating position and velocity vectors to a set of osculating Keplerian elements with the given value of mu.
    // This function is used when working with the SP propagator to get a more accurate set of Keplerian elements.
    void PosVelMuToKep(const std::array<double,3>& pos, const std::array<double,3>& vel, double mu, std::array<double,6>& xa_kep) const {

        p_PosVelMuToKep(const_cast<double*>(pos.data()), const_cast<double*>(vel.data()), mu, xa_kep.data());

    }

    // Converts position and velocity vectors to U, V, W vectors. See the remarks section for details.
    // The resulting vectors have the following meanings.
    // U vector: along radial direction
    // V vector: W x U
    // W vector: pos x vel
    void PosVelToUUVW(const std::array<double,3>& pos, const std::array<double,3>& vel, std::array<double,3>& uvec, std::array<double,3>& vVec, std::array<double,3>& wVec) const {

        p_PosVelToUUVW(const_cast<double*>(pos.data()), const_cast<double*>(vel.data()), uvec.data(), vVec.data(), wVec.data());

    }

    // Converts position and velocity vectors to U, V, W vectors. See the remarks section for details.
    // The resulting vectors have the following meanings.
    // U vector: V x W
    // V vector: along velocity direction
    // W vector: pos x vel
    void PosVelToPTW(const std::array<double,3>& pos, const std::array<double,3>& vel, std::array<double,3>& uvec, std::array<double,3>& vVec, std::array<double,3>& wVec) const {

        p_PosVelToPTW(const_cast<double*>(pos.data()), const_cast<double*>(vel.data()), uvec.data(), vVec.data(), wVec.data());

    }

    // Solves Kepler's equation (M = E - e sin(E)) for the eccentric anomaly, E, by iteration.
    double SolveKepEqtn(const std::array<double,6>& xa_kep) const {

        double eccAnomaly = p_SolveKepEqtn(const_cast<double*>(xa_kep.data()));

        return eccAnomaly;
    }

    // Computes true anomaly from a set of Keplerian elements.
    double CompTrueAnomaly(const std::array<double,6>& xa_kep) const {

        double trueAnomaly = p_CompTrueAnomaly(const_cast<double*>(xa_kep.data()));

        return trueAnomaly;
    }

    // Converts mean motion N to semi-major axis A.
    double NToA(double n) const {

        double a = p_NToA(n);

        return a;
    }

    // Converts semi-major axis A to mean motion N.
    double AToN(double a) const {

        double n = p_AToN(a);

        return n;
    }

    // Converts Kozai mean motion to Brouwer mean motion.
    // SGP TLE's use Kozai mean motion while SGP4/SGP4-XP TLE's use Brouwer mean motion.
    double KozaiToBrouwer(double eccen, double incli, double nKozai) const {

        double nBrouwer = p_KozaiToBrouwer(eccen, incli, nKozai);

        return nBrouwer;
    }

    // Converts Brouwer mean motion to Kozai mean motion.
    // SGP TLE's use Kozai mean motion while SGP4/SGP4-XP TLE's use Brouwer mean motion.
    double BrouwerToKozai(double eccen, double incli, double nBrouwer) const {

        double nKozai = p_BrouwerToKozai(eccen, incli, nBrouwer);

        return nKozai;
    }

    // Converts a set of osculating Keplerian elements to a set of mean Keplerian elements using method 9 algorithm.
    void KepOscToMean(const std::array<double,6>& xa_OscKep, std::array<double,6>& xa_MeanKep) const {

        p_KepOscToMean(const_cast<double*>(xa_OscKep.data()), xa_MeanKep.data());

    }

    // Converts an ECI position vector XYZ to geodetic latitude, longitude, and height.
    void XYZToLLH(double thetaG, const std::array<double,3>& metricPos, std::array<double,3>& metricLLH) const {

        p_XYZToLLH(thetaG, const_cast<double*>(metricPos.data()), metricLLH.data());

    }

    // Converts an ECI position vector XYZ to geodetic latitude, longitude, and height at the specified time.
    void XYZToLLHTime(double ds50UTC, const std::array<double,3>& metricPos, std::array<double,3>& metricLLH) const {

        p_XYZToLLHTime(ds50UTC, const_cast<double*>(metricPos.data()), metricLLH.data());

    }

    // Converts geodetic latitude, longitude, and height to an ECI position vector XYZ.
    void LLHToXYZ(double thetaG, const std::array<double,3>& metricLLH, std::array<double,3>& metricXYZ) const {

        p_LLHToXYZ(thetaG, const_cast<double*>(metricLLH.data()), metricXYZ.data());

    }

    // Converts geodetic latitude, longitude, and height to an ECI position vector XYZ at the specified time.
    void LLHToXYZTime(double ds50UTC, const std::array<double,3>& metricLLH, std::array<double,3>& metricXYZ) const {

        p_LLHToXYZTime(ds50UTC, const_cast<double*>(metricLLH.data()), metricXYZ.data());

    }

    // Converts EFG position and velocity vectors to ECI position and velocity vectors.
    void EFGToECI(double thetaG, const std::array<double,3>& posEFG, const std::array<double,3>& velEFG, std::array<double,3>& posECI, std::array<double,3>& velECI) const {

        p_EFGToECI(thetaG, const_cast<double*>(posEFG.data()), const_cast<double*>(velEFG.data()), posECI.data(), velECI.data());

    }

    // Converts EFG position and velocity vectors to ECI position and velocity vectors at the specified time.
    void EFGToECITime(double ds50UTC, const std::array<double,3>& posEFG, const std::array<double,3>& velEFG, std::array<double,3>& posECI, std::array<double,3>& velECI) const {

        p_EFGToECITime(ds50UTC, const_cast<double*>(posEFG.data()), const_cast<double*>(velEFG.data()), posECI.data(), velECI.data());

    }

    // Converts ECI position and velocity vectors to EFG position and velocity vectors.
    void ECIToEFG(double thetaG, const std::array<double,3>& posECI, const std::array<double,3>& velECI, std::array<double,3>& posEFG, std::array<double,3>& velEFG) const {

        p_ECIToEFG(thetaG, const_cast<double*>(posECI.data()), const_cast<double*>(velECI.data()), posEFG.data(), velEFG.data());

    }

    // Converts ECI position and velocity vectors to EFG position and velocity vectors at the specified time.
    void ECIToEFGTime(double ds50UTC, const std::array<double,3>& posECI, const std::array<double,3>& velECI, std::array<double,3>& posEFG, std::array<double,3>& velEFG) const {

        p_ECIToEFGTime(ds50UTC, const_cast<double*>(posECI.data()), const_cast<double*>(velECI.data()), posEFG.data(), velEFG.data());

    }

    // Converts ECR position and velocity vectors to EFG position and velocity vectors.
    void ECRToEFG(double polarX, double polarY, const std::array<double,3>& posECR, const std::array<double,3>& velECR, std::array<double,3>& posEFG, std::array<double,3>& velEFG) const {

        p_ECRToEFG(polarX, polarY, const_cast<double*>(posECR.data()), const_cast<double*>(velECR.data()), posEFG.data(), velEFG.data());

    }

    // Converts ECR position and velocity vectors to EFG position and velocity vectors at the specified time.
    // Note: Without timing constant data, the accuracy of the conversion results will be reduced
    void ECRToEFGTime(double ds50UTC, const std::array<double,3>& posECR, const std::array<double,3>& velECR, std::array<double,3>& posEFG, std::array<double,3>& velEFG) const {

        p_ECRToEFGTime(ds50UTC, const_cast<double*>(posECR.data()), const_cast<double*>(velECR.data()), posEFG.data(), velEFG.data());

    }

    // Converts EFG position and velocity vectors to ECR position and velocity vectors.
    void EFGToECR(double polarX, double polarY, const std::array<double,3>& posEFG, const std::array<double,3>& velEFG, std::array<double,3>& posECR, std::array<double,3>& velECR) const {

        p_EFGToECR(polarX, polarY, const_cast<double*>(posEFG.data()), const_cast<double*>(velEFG.data()), posECR.data(), velECR.data());

    }

    // Converts EFG position and velocity vectors to ECR position and velocity vectors at the specified time.
    // Note: Without timing constant data, the accuracy of the conversion results will be reduced
    void EFGToECRTime(double ds50UTC, const std::array<double,3>& posEFG, const std::array<double,3>& velEFG, std::array<double,3>& posECR, std::array<double,3>& velECR) const {

        p_EFGToECRTime(ds50UTC, const_cast<double*>(posEFG.data()), const_cast<double*>(velEFG.data()), posECR.data(), velECR.data());

    }

    // Converts an EFG position vector to geodetic latitude, longitude, and height.
    void EFGPosToLLH(const std::array<double,3>& posEFG, std::array<double,3>& metricLLH) const {

        p_EFGPosToLLH(const_cast<double*>(posEFG.data()), metricLLH.data());

    }

    // Converts geodetic latitude, longitude, and height to an EFG position vector.
    void LLHToEFGPos(const std::array<double,3>& metricLLH, std::array<double,3>& posEFG) const {

        p_LLHToEFGPos(const_cast<double*>(metricLLH.data()), posEFG.data());

    }

    // Rotates position and velocity vectors from J2000 to coordinates of the specified date, expressed in ds50TAI.
    void RotJ2KToDate(int spectr, int nutationTerms, double ds50TAI, const std::array<double,3>& posJ2K, const std::array<double,3>& velJ2K, std::array<double,3>& posDate, std::array<double,3>& velDate) const {

        p_RotJ2KToDate(spectr, nutationTerms, ds50TAI, const_cast<double*>(posJ2K.data()), const_cast<double*>(velJ2K.data()), posDate.data(), velDate.data());

    }

    // Rotates position and velocity vectors from coordinates of date to J2000.
    void RotDateToJ2K(int spectr, int nutationTerms, double ds50TAI, const std::array<double,3>& posDate, const std::array<double,3>& velDate, std::array<double,3>& posJ2K, std::array<double,3>& velJ2K) const {

        p_RotDateToJ2K(spectr, nutationTerms, ds50TAI, const_cast<double*>(posDate.data()), const_cast<double*>(velDate.data()), posJ2K.data(), velJ2K.data());

    }

    // Computes the Sun and Moon position at the specified time.
    void CompSunMoonPos(double ds50ET, std::array<double,3>& uvecSun, double& sunVecMag, std::array<double,3>& uvecMoon, double& moonVecMag) const {

        p_CompSunMoonPos(ds50ET, uvecSun.data(), &sunVecMag, uvecMoon.data(), &moonVecMag);

    }

    // Computes the Sun position at the specified time.
    void CompSunPos(double ds50ET, std::array<double,3>& uvecSun, double& sunVecMag) const {

        p_CompSunPos(ds50ET, uvecSun.data(), &sunVecMag);

    }

    // Computes the Moon position at the specified time.
    void CompMoonPos(double ds50ET, std::array<double,3>& uvecMoon, double& moonVecMag) const {

        p_CompMoonPos(ds50ET, uvecMoon.data(), &moonVecMag);

    }

    // This function is intended for future use.  No information is currently available.
    void AstroConvFrTo(int xf_Conv, const std::array<double,128>& frArr, std::array<double,128>& toArr) const {

        p_AstroConvFrTo(xf_Conv, const_cast<double*>(frArr.data()), toArr.data());

    }

    // Converts right ascension and declination to vector triad LAD in topocentric equatorial coordinate system.
    void RADecToLAD(double ra, double dec, std::array<double,3>& l, std::array<double,3>& a_tilde, std::array<double,3>& d_tilde) const {

        p_RADecToLAD(ra, dec, l.data(), a_tilde.data(), d_tilde.data());

    }

    // Converts azimuth and elevation to vector triad LAD in topocentric horizontal coordinate system.
    void AzElToLAD(double az, double el, std::array<double,3>& lh, std::array<double,3>& ah, std::array<double,3>& dh) const {

        p_AzElToLAD(az, el, lh.data(), ah.data(), dh.data());

    }

    // Converts satellite ECI position/velocity vectors and sensor location to topocentric components.
    // The xa_topo array has the following structure:
    // [0]: Resulting right ascension (RA) (deg)
    // [1]: Declination (deg)
    // [2]: Azimuth (deg)
    // [3]: Elevation (deg)
    // [4]: Range (km)
    // [5]: RAdot (first derivative of right ascension) (deg/s)
    // [6]: DecDot (first derivative of declination) (deg/s)
    // [7]: AzDot (first derivative of azimuth) (deg/s)
    // [8]: ElDot (first derivative of elevation) (deg/s)
    // [9]: RangeDot (first derivative of range) (km/s)   
    void ECIToTopoComps(double theta, double lat, const std::array<double,3>& senPos, const std::array<double,3>& satPos, const std::array<double,3>& satVel, std::array<double,10>& xa_topo) const {

        p_ECIToTopoComps(theta, lat, const_cast<double*>(senPos.data()), const_cast<double*>(satPos.data()), const_cast<double*>(satVel.data()), xa_topo.data());

    }

    // Converts right ascension and declination in the topocentric reference frame to Azimuth/Elevation in the local horizon reference frame.
    void RaDecToAzEl(double thetaG, double lat, double lon, double ra, double dec, double& az, double& el) const {

        p_RaDecToAzEl(thetaG, lat, lon, ra, dec, &az, &el);

    }

    // Converts right ascension and declination in the topocentric reference frame to Azimuth/Elevation in the local horizon reference frame.
    void RaDecToAzElTime(double ds50UTC, double lat, double lon, double ra, double dec, double& az, double& el) const {

        p_RaDecToAzElTime(ds50UTC, lat, lon, ra, dec, &az, &el);

    }

    // Converts Azimuth/Elevation in the local horizon reference frame to Right ascension/Declination in the topocentric reference frame
    void AzElToRaDec(double thetaG, double lat, double lon, double az, double el, double& ra, double& dec) const {

        p_AzElToRaDec(thetaG, lat, lon, az, el, &ra, &dec);

    }

    // Converts Azimuth/Elevation in the local horizon reference frame to Right ascension/Declination in the topocentric reference frame
    void AzElToRaDecTime(double ds50UTC, double lat, double lon, double az, double el, double& ra, double& dec) const {

        p_AzElToRaDecTime(ds50UTC, lat, lon, az, el, &ra, &dec);

    }

    // Converts full state RAE (range, az, el, and their rates) to full state ECI (position and velocity)
    // The xa_rae array has the following structure:
    // [0]: Range (km)
    // [1]: Azimuth (deg)
    // [2]: Elevation (deg)
    // [3]: Range Dot (km/s)
    // [4]: Azimuth Dot (deg/s)
    // [5]: Elevation Dot (deg/s)
    void RAEToECI(double theta, double astroLat, const std::array<double,6>& xa_rae, const std::array<double,3>& senPos, std::array<double,3>& satPos, std::array<double,3>& satVel) const {

        p_RAEToECI(theta, astroLat, const_cast<double*>(xa_rae.data()), const_cast<double*>(senPos.data()), satPos.data(), satVel.data());

    }

    // Computes initial values for the SGP drag term nDot and the SGP4 drag term BSTAR based upon eccentricity and semi-major axis.
    void GetInitialDrag(double semiMajorAxis, double eccen, double& nDot, double& bstar) const {

        p_GetInitialDrag(semiMajorAxis, eccen, &nDot, &bstar);

    }

    // Converts covariance matrix PTW to UVW.
    // PTW = P: TxW, T: along velocity direction, W: pos x vel.
    // UVW = U: radial direction, V: in plane, perpendicular to U, W: pos x vel.
    void CovMtxPTWToUVW(const std::array<double,3>& pos, const std::array<double,3>& vel, const std::array<std::array<double,6>,6>& ptwCovMtx, std::array<std::array<double,6>,6>& uvwCovMtx) const {

        p_CovMtxPTWToUVW(const_cast<double*>(pos.data()), const_cast<double*>(vel.data()), const_cast<double(*)[6]>(reinterpret_cast<const double(*)[6]>(ptwCovMtx.data())), reinterpret_cast<double(*)[6]>(uvwCovMtx.data()));

    }

    // Converts covariance matrix UVW to PTW.
    // PTW = P: TxW, T: along velocity direction, W: pos x vel.
    // UVW = U: radial direction, V: in plane, perpendicular to U, W: pos x vel.
    void CovMtxUVWToPTW(const std::array<double,3>& pos, const std::array<double,3>& vel, const std::array<std::array<double,6>,6>& uvwCovMtx, std::array<std::array<double,6>,6>& ptwCovMtx) const {

        p_CovMtxUVWToPTW(const_cast<double*>(pos.data()), const_cast<double*>(vel.data()), const_cast<double(*)[6]>(reinterpret_cast<const double(*)[6]>(uvwCovMtx.data())), reinterpret_cast<double(*)[6]>(ptwCovMtx.data()));

    }

    // Computes Earth/Sensor/Earth Limb and Earth/Sensor/Satellite angles.
    void EarthObstructionAngles(double earthLimb, const std::array<double,3>& satECI, const std::array<double,3>& senECI, double& earthSenLimb, double& earthSenSat, double& satEarthSen) const {

        p_EarthObstructionAngles(earthLimb, const_cast<double*>(satECI.data()), const_cast<double*>(senECI.data()), &earthSenLimb, &earthSenSat, &satEarthSen);

    }

    // Determines if a point in space is sunlit at the input time ds50ET
    int IsPointSunlit(double ds50ET, const std::array<double,3>& ptEci) const {

        int yesNo = p_IsPointSunlit(ds50ET, const_cast<double*>(ptEci.data()));

        return yesNo;
    }

    // Rotates Right Ascension and Declination to specified epoch
    void RotRADecl(int nutationTerms, int dir, double ds50UTCIn, double raIn, double declIn, double ds50UTCOut, double& raOut, double& declOut) const {

        p_RotRADecl(nutationTerms, dir, ds50UTCIn, raIn, declIn, ds50UTCOut, &raOut, &declOut);

    }

    // Rotates Right Ascension and Declination from TEME of Date to MEME of the specified year of equinox
    int RotRADec_DateToEqnx(int nutationTerms, int yrOfEqnx, double ds50UTCIn, double raIn, double declIn, double& raOut, double& declOut) const {

        int errCode = p_RotRADec_DateToEqnx(nutationTerms, yrOfEqnx, ds50UTCIn, raIn, declIn, &raOut, &declOut);

        return errCode;
    }

    // Rotates Right Ascension and Declination from MEME of the specified year of equinox to TEME of Date
    int RotRADec_EqnxToDate(int nutationTerms, int yrOfEqnx, double ds50UTCIn, double raIn, double declIn, double& raOut, double& declOut) const {

        int errCode = p_RotRADec_EqnxToDate(nutationTerms, yrOfEqnx, ds50UTCIn, raIn, declIn, &raOut, &declOut);

        return errCode;
    }

    // Rotates the Equinoctial covariance to UVW
    // Note: This method uses the global Earth constants so make sure that you select the right Earth model by calling the EnvConst/EnvSetGeoIdx method
    // The n terms must be normalized by n
    // The input position, velocity and covariance must all have the same reference equator and equinox.
    void CovMtxEqnxToUVW(const std::array<double,3>& pos, const std::array<double,3>& vel, const std::array<std::array<double,6>,6>& covMtxEqnx, std::array<std::array<double,6>,6>& covMtxUVW) const {

        p_CovMtxEqnxToUVW(const_cast<double*>(pos.data()), const_cast<double*>(vel.data()), const_cast<double(*)[6]>(reinterpret_cast<const double(*)[6]>(covMtxEqnx.data())), reinterpret_cast<double(*)[6]>(covMtxUVW.data()));

    }

    // Rotates the UVW covariance to Equinoctial
    // Note: This method uses the global Earth constants so make sure that you select the right Earth model by calling the EnvConst/EnvSetGeoIdx method
    // The n terms are normalized by n
    // The input position, velocity reference equator and equinox determine the output covariance reference frame.
    void CovMtxUVWToEqnx(const std::array<double,3>& pos, const std::array<double,3>& vel, const std::array<std::array<double,6>,6>& covMtxUVW, std::array<std::array<double,6>,6>& covMtxEqnx) const {

        p_CovMtxUVWToEqnx(const_cast<double*>(pos.data()), const_cast<double*>(vel.data()), const_cast<double(*)[6]>(reinterpret_cast<const double(*)[6]>(covMtxUVW.data())), reinterpret_cast<double(*)[6]>(covMtxEqnx.data()));

    }

    // Rotates the ECI covariance to UVW
    // Note: This method uses the global Earth constants so make sure that you select the proper Earth model by calling the EnvConst/EnvSetGeoIdx method
    void CovMtxECIToUVW(const std::array<double,3>& pos, const std::array<double,3>& vel, const std::array<std::array<double,6>,6>& covMtxECI, std::array<std::array<double,6>,6>& covMtxUVW) const {

        p_CovMtxECIToUVW(const_cast<double*>(pos.data()), const_cast<double*>(vel.data()), const_cast<double(*)[6]>(reinterpret_cast<const double(*)[6]>(covMtxECI.data())), reinterpret_cast<double(*)[6]>(covMtxUVW.data()));

    }

    // Rotates the UVW covariance to ECI
    // Note: This method uses the global Earth constants so make sure that you select the proper Earth model by calling the EnvConst/EnvSetGeoIdx method
    void CovMtxUVWToECI(const std::array<double,3>& pos, const std::array<double,3>& vel, const std::array<std::array<double,6>,6>& covMtxUVW, std::array<std::array<double,6>,6>& covMtxECI) const {

        p_CovMtxUVWToECI(const_cast<double*>(pos.data()), const_cast<double*>(vel.data()), const_cast<double(*)[6]>(reinterpret_cast<const double(*)[6]>(covMtxUVW.data())), reinterpret_cast<double(*)[6]>(covMtxECI.data()));

    }

    // Converts covariance matrix ECI to EFG.
    // EFG = Earth Fixed Greenwich
    // ECI = Earth Centered Inertial - need to determine TEME or J2K
    void CovMtxECIToEFG(double thetaG, const std::array<std::array<double,6>,6>& covECI, std::array<std::array<double,6>,6>& covEFG) const {

        p_CovMtxECIToEFG(thetaG, const_cast<double(*)[6]>(reinterpret_cast<const double(*)[6]>(covECI.data())), reinterpret_cast<double(*)[6]>(covEFG.data()));

    }

    // Converts covariance matrix EFG to ECI.
    // EFG = Earth Fixed Greenwich
    // ECI = Earth Centered Inertial - need to determine TEME or J2K
    void CovMtxEFGToECI(double thetaG, const std::array<std::array<double,6>,6>& covEFG, std::array<std::array<double,6>,6>& covECI) const {

        p_CovMtxEFGToECI(thetaG, const_cast<double(*)[6]>(reinterpret_cast<const double(*)[6]>(covEFG.data())), reinterpret_cast<double(*)[6]>(covECI.data()));

    }

    // Converts covariance matrix ECI of Date to ECI of J2K.
    // J2k = Earth Centered Inertial MEME of J2K
    // ECI = Earth Centered Inertial TEME of Date
    void CovMtxECIToJ2k(double ds50Utc, int nTerms, const std::array<std::array<double,6>,6>& covECI, std::array<std::array<double,6>,6>& covJ2k) const {

        p_CovMtxECIToJ2k(ds50Utc, nTerms, const_cast<double(*)[6]>(reinterpret_cast<const double(*)[6]>(covECI.data())), reinterpret_cast<double(*)[6]>(covJ2k.data()));

    }

    // Converts covariance matrix ECI of J2K to ECI of Date.
    // J2k = Earth Centered Inertial MEME of J2K
    // ECI = Earth Centered Inertial TEME of Date
    void CovMtxJ2kToECI(double ds50Utc, int nTerms, const std::array<std::array<double,6>,6>& covJ2k, std::array<std::array<double,6>,6>& covECI) const {

        p_CovMtxJ2kToECI(ds50Utc, nTerms, const_cast<double(*)[6]>(reinterpret_cast<const double(*)[6]>(covJ2k.data())), reinterpret_cast<double(*)[6]>(covECI.data()));

    }

    // Converts 6x6 symmetric Matrix/2D array to 1D array of 21 elements (lower triangular of a 6x6 symmetric matrix)
    void Mtx6x6ToLTA21(const std::array<std::array<double,6>,6>& symMtx6x6, std::array<double,21>& lta21) const {

        p_Mtx6x6ToLTA21(const_cast<double(*)[6]>(reinterpret_cast<const double(*)[6]>(symMtx6x6.data())), lta21.data());

    }

    // Converts 1D array of 21 elements (lower triangular of a 6x6 symmetric matrix) to a 6x6 symmetric matrix
    void LTA21ToMtx6x6(const std::array<double,21>& lta21, std::array<std::array<double,6>,6>& symMtx6x6) const {

        p_LTA21ToMtx6x6(const_cast<double*>(lta21.data()), reinterpret_cast<double(*)[6]>(symMtx6x6.data()));

    }

    // Converts 9x9 symmetric Matrix/2D array to 1D array of 45 elements (lower triangular of a 9x9 symmetric matrix)
    void Mtx9x9ToLTA45(const std::array<std::array<double,9>,9>& symMtx9x9, std::array<double,45>& lta45) const {

        p_Mtx9x9ToLTA45(const_cast<double(*)[9]>(reinterpret_cast<const double(*)[9]>(symMtx9x9.data())), lta45.data());

    }

    // Converts 1D array of 45 elements (lower triangular of a 9x9 symmetric matrix) to a 9x9 symmetric matrix
    void LTA45ToMtx9x9(const std::array<double,45>& lta45, std::array<std::array<double,9>,9>& symMtx9x9) const {

        p_LTA45ToMtx9x9(const_cast<double*>(lta45.data()), reinterpret_cast<double(*)[9]>(symMtx9x9.data()));

    }

    // Propagate xyzDate covariance forward to the propagation time
    void PropCovFrState(double rms, double consider, const std::array<double,54>& stateArray, const std::array<std::array<double,9>,9>& cov, std::array<std::array<double,6>,6>& propCov) const {

        p_PropCovFrState(rms, consider, const_cast<double*>(stateArray.data()), const_cast<double(*)[9]>(reinterpret_cast<const double(*)[9]>(cov.data())), reinterpret_cast<double(*)[6]>(propCov.data()));

    }

    // Rotates the ECI covariance to UVW
    // Note: This method uses the global Earth constants so make sure that you select the proper Earth model by calling the EnvConst/EnvSetGeoIdx method
    void CovMtxECIToEqnx(const std::array<double,3>& pos, const std::array<double,3>& vel, const std::array<std::array<double,9>,9>& covMtxECI, std::array<std::array<double,9>,9>& covMtxEqnx) const {

        p_CovMtxECIToEqnx(const_cast<double*>(pos.data()), const_cast<double*>(vel.data()), const_cast<double(*)[9]>(reinterpret_cast<const double(*)[9]>(covMtxECI.data())), reinterpret_cast<double(*)[9]>(covMtxEqnx.data()));

    }

    // Rotates the UVW covariance to ECI
    // Note: This method uses the global Earth constants so make sure that you select the proper Earth model by calling the EnvConst/EnvSetGeoIdx method
    void CovMtxEqnxToECI9x9(const std::array<double,3>& pos, const std::array<double,3>& vel, const std::array<std::array<double,9>,9>& covEqnx, std::array<std::array<double,9>,9>& covMtxECI) const {

        p_CovMtxEqnxToECI9x9(const_cast<double*>(pos.data()), const_cast<double*>(vel.data()), const_cast<double(*)[9]>(reinterpret_cast<const double(*)[9]>(covEqnx.data())), reinterpret_cast<double(*)[9]>(covMtxECI.data()));

    }

    // Rotates the UVW covariance to ECI
    // Note: This method uses the global Earth constants so make sure that you select the proper Earth model by calling the EnvConst/EnvSetGeoIdx method
    void CovMtxEqnxToUVW9x9(const std::array<double,3>& pos, const std::array<double,3>& vel, const std::array<std::array<double,9>,9>& covEqnx, std::array<std::array<double,9>,9>& covMtxUVW) const {

        p_CovMtxEqnxToUVW9x9(const_cast<double*>(pos.data()), const_cast<double*>(vel.data()), const_cast<double(*)[9]>(reinterpret_cast<const double(*)[9]>(covEqnx.data())), reinterpret_cast<double(*)[9]>(covMtxUVW.data()));

    }

    // Update (propagate) covariance to a future time with a supplied covariance, state transition matrix
    // consider parameter and RMS. Consider parameter is applied to the drag term only.
    // Full covariance matrix is multiplied by RMS squared.  State transition matrix can be obtained from 
    // SpProp.SpGetStateMtx or supplying your own. State matrix, input and output covariance must be in 
    // matching coordinate systems.
    void CovMtxUpdate(double rmsIn, double consider, const std::array<std::array<double,9>,9>& cov, const std::array<double,54>& stateArray, std::array<std::array<double,6>,6>& propCov) const {

        p_CovMtxUpdate(rmsIn, consider, const_cast<double(*)[9]>(reinterpret_cast<const double(*)[9]>(cov.data())), const_cast<double*>(stateArray.data()), reinterpret_cast<double(*)[6]>(propCov.data()));

    }

    // Annual Aberration calculated using equations from Astronomical Algorithms, Jean Meeus, 2nd Edition with Corrections as of June 15, 2005
    void AberrationAnnual(double ra, double decl, double dS50UTC, double& raDelta, double& decDelta) const {

        p_AberrationAnnual(ra, decl, dS50UTC, &raDelta, &decDelta);

    }

    // Diurnal Aberration is due to the rotation of the Earth about it's axis. This is only valid for ground based sensors.
    // Diurnal Aberration calculated using equations from Explanatory Supplement to the Astronomical Almanac 3rd Edition, 2013
    void AberrationDiurnal(double ra, double decl, double dS50UTC, const std::array<double,3>& senPos, double& raDelta, double& decDelta) const {

        p_AberrationDiurnal(ra, decl, dS50UTC, const_cast<double*>(senPos.data()), &raDelta, &decDelta);

    }

    // Sets JPL parameters
    // Notes: Set JPL parameters will be used by SP, SPG4-XP, and anything that requires access to JPL data
    void JplSetParameters(const std::string& jplFile, double ds50Start, double ds50Stop) const {
        char jplFileBuf[513] = {};
        to_c(jplFile, jplFileBuf);

        p_JplSetParameters(jplFileBuf, ds50Start, ds50Stop);

    }

    // Gets JPL parameters
    void JplGetParameters(std::string& jplFile, double& ds50Start, double& ds50Stop) const {
        char jplFileBuf[513] = {};

        p_JplGetParameters(jplFileBuf, &ds50Start, &ds50Stop);

        from_c(jplFileBuf, jplFile);
    }

    // Resets JPL parameters & removes JPL ephemeris data
    void JplReset() const {

        p_JplReset();

    }

    // Computes various Sun and Moon vectors base on loaded JPL data at the specified time.
    // Note: if JPL data isn't loaded or available, all output parameters are set to zero
    void JplCompSunMoonVec(double ds50UTC, std::array<double,3>& uvecSun, double& sunVecMag, std::array<double,3>& uvecMoon, double& moonVecMag) const {

        p_JplCompSunMoonVec(ds50UTC, uvecSun.data(), &sunVecMag, uvecMoon.data(), &moonVecMag);

    }

    // Computes Sun and Moon position vectors base on loaded JPL data at the specified time.
    // Note: if JPL data isn't loaded or available, all output parameters are set to zero
    void JplCompSunMoonPos(double ds50UTC, std::array<double,3>& sunVec, std::array<double,3>& moonVec) const {

        p_JplCompSunMoonPos(ds50UTC, sunVec.data(), moonVec.data());

    }

    // Removes the JPL ephemeris from memory
    void RemoveJpl() const {

        p_RemoveJpl();

    }

    // Rotates position and velocity vectors from TEME of Epoch to TEME of Date
    void TemeEpochToDate(int nutationTerms, double epochDs50TAI, double dateDs50TAI, const std::array<double,3>& posEpoch, const std::array<double,3>& velEpoch, std::array<double,3>& posDate, std::array<double,3>& velDate) const {

        p_TemeEpochToDate(nutationTerms, epochDs50TAI, dateDs50TAI, const_cast<double*>(posEpoch.data()), const_cast<double*>(velEpoch.data()), posDate.data(), velDate.data());

    }

    // Converts ECR position and velocity vectors to ECI (TEME of Date) position and velocity vectors.
    // Note: Without timing constant data, the accuracy of the conversion results will be reduced
    void ECRToECITime(double ds50UTC, const std::array<double,3>& posECR, const std::array<double,3>& velECR, std::array<double,3>& posECI, std::array<double,3>& velECI) const {

        p_ECRToECITime(ds50UTC, const_cast<double*>(posECR.data()), const_cast<double*>(velECR.data()), posECI.data(), velECI.data());

    }

    // Converts ECI (TEME of Date) position and velocity vectors to ECR position and velocity vectors.
    // Note: Without timing constant data, the accuracy of the conversion results will be reduced
    void ECIToECRTime(double ds50UTC, const std::array<double,3>& posECI, const std::array<double,3>& velECI, std::array<double,3>& posECR, std::array<double,3>& velECR) const {

        p_ECIToECRTime(ds50UTC, const_cast<double*>(posECI.data()), const_cast<double*>(velECI.data()), posECR.data(), velECR.data());

    }

    // Converts ECR position and velocity vectors to J2K (MEME of J2000) position and velocity vectors.
    // Note: Without timing constant data, the accuracy of the conversion results will be reduced
    void ECRToJ2KTime(int spectr, int nutationTerms, double ds50UTC, const std::array<double,3>& posECR, const std::array<double,3>& velECR, std::array<double,3>& posJ2K, std::array<double,3>& velJ2K) const {

        p_ECRToJ2KTime(spectr, nutationTerms, ds50UTC, const_cast<double*>(posECR.data()), const_cast<double*>(velECR.data()), posJ2K.data(), velJ2K.data());

    }

    // Converts J2K (MEME of J2000) position and velocity vectors to ECR position and velocity vectors.
    // Note: Without timing constant data, the accuracy of the conversion results will be reduced
    void J2KToECRTime(int spectr, int nutationTerms, double ds50UTC, const std::array<double,3>& posJ2K, const std::array<double,3>& velJ2K, std::array<double,3>& posECR, std::array<double,3>& velECR) const {

        p_J2KToECRTime(spectr, nutationTerms, ds50UTC, const_cast<double*>(posJ2K.data()), const_cast<double*>(velJ2K.data()), posECR.data(), velECR.data());

    }


private:
    AstroFuncApi() : lib(resolveLibraryName()) {loadSymbols();}
    ~AstroFuncApi() = default;

    using fn_AstroFuncInit = int (STDCALL *)(__int64 apAddr);
    using fn_AstroFuncGetInfo = void (STDCALL *)(char infoStr[128]);
    using fn_KepToEqnx = void (STDCALL *)(double xa_kep[6], double xa_eqnx[6]);
    using fn_KepToPosVel = void (STDCALL *)(double xa_kep[6], double pos[3], double vel[3]);
    using fn_KepToUVW = void (STDCALL *)(double xa_kep[6], double uBar[3], double vBar[3], double wBar[3]);
    using fn_ClassToEqnx = void (STDCALL *)(double xa_cls[6], double xa_eqnx[6]);
    using fn_EqnxToClass = void (STDCALL *)(double xa_eqnx[6], double xa_cls[6]);
    using fn_EqnxToKep = void (STDCALL *)(double xa_eqnx[6], double xa_kep[6]);
    using fn_EqnxToPosVel = void (STDCALL *)(double xa_eqnx[6], double pos[3], double vel[3]);
    using fn_PosVelToEqnx = void (STDCALL *)(double pos[3], double vel[3], double xa_eqnx[6]);
    using fn_PosVelMuToEqnx = void (STDCALL *)(double pos[3], double vel[3], double mu, double xa_eqnx[6]);
    using fn_PosVelToKep = void (STDCALL *)(double pos[3], double vel[3], double xa_kep[6]);
    using fn_PosVelMuToKep = void (STDCALL *)(double pos[3], double vel[3], double mu, double xa_kep[6]);
    using fn_PosVelToUUVW = void (STDCALL *)(double pos[3], double vel[3], double uvec[3], double vVec[3], double wVec[3]);
    using fn_PosVelToPTW = void (STDCALL *)(double pos[3], double vel[3], double uvec[3], double vVec[3], double wVec[3]);
    using fn_SolveKepEqtn = double (STDCALL *)(double xa_kep[6]);
    using fn_CompTrueAnomaly = double (STDCALL *)(double xa_kep[6]);
    using fn_NToA = double (STDCALL *)(double n);
    using fn_AToN = double (STDCALL *)(double a);
    using fn_KozaiToBrouwer = double (STDCALL *)(double eccen, double incli, double nKozai);
    using fn_BrouwerToKozai = double (STDCALL *)(double eccen, double incli, double nBrouwer);
    using fn_KepOscToMean = void (STDCALL *)(double xa_OscKep[6], double xa_MeanKep[6]);
    using fn_XYZToLLH = void (STDCALL *)(double thetaG, double metricPos[3], double metricLLH[3]);
    using fn_XYZToLLHTime = void (STDCALL *)(double ds50UTC, double metricPos[3], double metricLLH[3]);
    using fn_LLHToXYZ = void (STDCALL *)(double thetaG, double metricLLH[3], double metricXYZ[3]);
    using fn_LLHToXYZTime = void (STDCALL *)(double ds50UTC, double metricLLH[3], double metricXYZ[3]);
    using fn_EFGToECI = void (STDCALL *)(double thetaG, double posEFG[3], double velEFG[3], double posECI[3], double velECI[3]);
    using fn_EFGToECITime = void (STDCALL *)(double ds50UTC, double posEFG[3], double velEFG[3], double posECI[3], double velECI[3]);
    using fn_ECIToEFG = void (STDCALL *)(double thetaG, double posECI[3], double velECI[3], double posEFG[3], double velEFG[3]);
    using fn_ECIToEFGTime = void (STDCALL *)(double ds50UTC, double posECI[3], double velECI[3], double posEFG[3], double velEFG[3]);
    using fn_ECRToEFG = void (STDCALL *)(double polarX, double polarY, double posECR[3], double velECR[3], double posEFG[3], double velEFG[3]);
    using fn_ECRToEFGTime = void (STDCALL *)(double ds50UTC, double posECR[3], double velECR[3], double posEFG[3], double velEFG[3]);
    using fn_EFGToECR = void (STDCALL *)(double polarX, double polarY, double posEFG[3], double velEFG[3], double posECR[3], double velECR[3]);
    using fn_EFGToECRTime = void (STDCALL *)(double ds50UTC, double posEFG[3], double velEFG[3], double posECR[3], double velECR[3]);
    using fn_EFGPosToLLH = void (STDCALL *)(double posEFG[3], double metricLLH[3]);
    using fn_LLHToEFGPos = void (STDCALL *)(double metricLLH[3], double posEFG[3]);
    using fn_RotJ2KToDate = void (STDCALL *)(int spectr, int nutationTerms, double ds50TAI, double posJ2K[3], double velJ2K[3], double posDate[3], double velDate[3]);
    using fn_RotDateToJ2K = void (STDCALL *)(int spectr, int nutationTerms, double ds50TAI, double posDate[3], double velDate[3], double posJ2K[3], double velJ2K[3]);
    using fn_CompSunMoonPos = void (STDCALL *)(double ds50ET, double uvecSun[3], double* sunVecMag, double uvecMoon[3], double* moonVecMag);
    using fn_CompSunPos = void (STDCALL *)(double ds50ET, double uvecSun[3], double* sunVecMag);
    using fn_CompMoonPos = void (STDCALL *)(double ds50ET, double uvecMoon[3], double* moonVecMag);
    using fn_AstroConvFrTo = void (STDCALL *)(int xf_Conv, double frArr[128], double toArr[128]);
    using fn_RADecToLAD = void (STDCALL *)(double ra, double dec, double l[3], double a_tilde[3], double d_tilde[3]);
    using fn_AzElToLAD = void (STDCALL *)(double az, double el, double lh[3], double ah[3], double dh[3]);
    using fn_ECIToTopoComps = void (STDCALL *)(double theta, double lat, double senPos[3], double satPos[3], double satVel[3], double xa_topo[10]);
    using fn_RaDecToAzEl = void (STDCALL *)(double thetaG, double lat, double lon, double ra, double dec, double* az, double* el);
    using fn_RaDecToAzElTime = void (STDCALL *)(double ds50UTC, double lat, double lon, double ra, double dec, double* az, double* el);
    using fn_AzElToRaDec = void (STDCALL *)(double thetaG, double lat, double lon, double az, double el, double* ra, double* dec);
    using fn_AzElToRaDecTime = void (STDCALL *)(double ds50UTC, double lat, double lon, double az, double el, double* ra, double* dec);
    using fn_RAEToECI = void (STDCALL *)(double theta, double astroLat, double xa_rae[6], double senPos[3], double satPos[3], double satVel[3]);
    using fn_GetInitialDrag = void (STDCALL *)(double semiMajorAxis, double eccen, double* nDot, double* bstar);
    using fn_CovMtxPTWToUVW = void (STDCALL *)(double pos[3], double vel[3], double ptwCovMtx[6][6], double uvwCovMtx[6][6]);
    using fn_CovMtxUVWToPTW = void (STDCALL *)(double pos[3], double vel[3], double uvwCovMtx[6][6], double ptwCovMtx[6][6]);
    using fn_EarthObstructionAngles = void (STDCALL *)(double earthLimb, double satECI[3], double senECI[3], double* earthSenLimb, double* earthSenSat, double* satEarthSen);
    using fn_IsPointSunlit = int (STDCALL *)(double ds50ET, double ptEci[3]);
    using fn_RotRADecl = void (STDCALL *)(int nutationTerms, int dir, double ds50UTCIn, double raIn, double declIn, double ds50UTCOut, double* raOut, double* declOut);
    using fn_RotRADec_DateToEqnx = int (STDCALL *)(int nutationTerms, int yrOfEqnx, double ds50UTCIn, double raIn, double declIn, double* raOut, double* declOut);
    using fn_RotRADec_EqnxToDate = int (STDCALL *)(int nutationTerms, int yrOfEqnx, double ds50UTCIn, double raIn, double declIn, double* raOut, double* declOut);
    using fn_CovMtxEqnxToUVW = void (STDCALL *)(double pos[3], double vel[3], double covMtxEqnx[6][6], double covMtxUVW[6][6]);
    using fn_CovMtxUVWToEqnx = void (STDCALL *)(double pos[3], double vel[3], double covMtxUVW[6][6], double covMtxEqnx[6][6]);
    using fn_CovMtxECIToUVW = void (STDCALL *)(double pos[3], double vel[3], double covMtxECI[6][6], double covMtxUVW[6][6]);
    using fn_CovMtxUVWToECI = void (STDCALL *)(double pos[3], double vel[3], double covMtxUVW[6][6], double covMtxECI[6][6]);
    using fn_CovMtxECIToEFG = void (STDCALL *)(double thetaG, double covECI[6][6], double covEFG[6][6]);
    using fn_CovMtxEFGToECI = void (STDCALL *)(double thetaG, double covEFG[6][6], double covECI[6][6]);
    using fn_CovMtxECIToJ2k = void (STDCALL *)(double ds50Utc, int nTerms, double covECI[6][6], double covJ2k[6][6]);
    using fn_CovMtxJ2kToECI = void (STDCALL *)(double ds50Utc, int nTerms, double covJ2k[6][6], double covECI[6][6]);
    using fn_Mtx6x6ToLTA21 = void (STDCALL *)(double symMtx6x6[6][6], double lta21[21]);
    using fn_LTA21ToMtx6x6 = void (STDCALL *)(double lta21[21], double symMtx6x6[6][6]);
    using fn_Mtx9x9ToLTA45 = void (STDCALL *)(double symMtx9x9[9][9], double lta45[45]);
    using fn_LTA45ToMtx9x9 = void (STDCALL *)(double lta45[45], double symMtx9x9[9][9]);
    using fn_PropCovFrState = void (STDCALL *)(double rms, double consider, double stateArray[54], double cov[9][9], double propCov[6][6]);
    using fn_CovMtxECIToEqnx = void (STDCALL *)(double pos[3], double vel[3], double covMtxECI[9][9], double covMtxEqnx[9][9]);
    using fn_CovMtxEqnxToECI9x9 = void (STDCALL *)(double pos[3], double vel[3], double covEqnx[9][9], double covMtxECI[9][9]);
    using fn_CovMtxEqnxToUVW9x9 = void (STDCALL *)(double pos[3], double vel[3], double covEqnx[9][9], double covMtxUVW[9][9]);
    using fn_CovMtxUpdate = void (STDCALL *)(double rmsIn, double consider, double cov[9][9], double stateArray[54], double propCov[6][6]);
    using fn_AberrationAnnual = void (STDCALL *)(double ra, double decl, double dS50UTC, double* raDelta, double* decDelta);
    using fn_AberrationDiurnal = void (STDCALL *)(double ra, double decl, double dS50UTC, double senPos[3], double* raDelta, double* decDelta);
    using fn_JplSetParameters = void (STDCALL *)(char jplFile[512], double ds50Start, double ds50Stop);
    using fn_JplGetParameters = void (STDCALL *)(char jplFile[512], double* ds50Start, double* ds50Stop);
    using fn_JplReset = void (STDCALL *)();
    using fn_JplCompSunMoonVec = void (STDCALL *)(double ds50UTC, double uvecSun[3], double* sunVecMag, double uvecMoon[3], double* moonVecMag);
    using fn_JplCompSunMoonPos = void (STDCALL *)(double ds50UTC, double sunVec[3], double moonVec[3]);
    using fn_RemoveJpl = void (STDCALL *)();
    using fn_TemeEpochToDate = void (STDCALL *)(int nutationTerms, double epochDs50TAI, double dateDs50TAI, double posEpoch[3], double velEpoch[3], double posDate[3], double velDate[3]);
    using fn_ECRToECITime = void (STDCALL *)(double ds50UTC, double posECR[3], double velECR[3], double posECI[3], double velECI[3]);
    using fn_ECIToECRTime = void (STDCALL *)(double ds50UTC, double posECI[3], double velECI[3], double posECR[3], double velECR[3]);
    using fn_ECRToJ2KTime = void (STDCALL *)(int spectr, int nutationTerms, double ds50UTC, double posECR[3], double velECR[3], double posJ2K[3], double velJ2K[3]);
    using fn_J2KToECRTime = void (STDCALL *)(int spectr, int nutationTerms, double ds50UTC, double posJ2K[3], double velJ2K[3], double posECR[3], double velECR[3]);

    fn_AstroFuncInit p_AstroFuncInit{};
    fn_AstroFuncGetInfo p_AstroFuncGetInfo{};
    fn_KepToEqnx p_KepToEqnx{};
    fn_KepToPosVel p_KepToPosVel{};
    fn_KepToUVW p_KepToUVW{};
    fn_ClassToEqnx p_ClassToEqnx{};
    fn_EqnxToClass p_EqnxToClass{};
    fn_EqnxToKep p_EqnxToKep{};
    fn_EqnxToPosVel p_EqnxToPosVel{};
    fn_PosVelToEqnx p_PosVelToEqnx{};
    fn_PosVelMuToEqnx p_PosVelMuToEqnx{};
    fn_PosVelToKep p_PosVelToKep{};
    fn_PosVelMuToKep p_PosVelMuToKep{};
    fn_PosVelToUUVW p_PosVelToUUVW{};
    fn_PosVelToPTW p_PosVelToPTW{};
    fn_SolveKepEqtn p_SolveKepEqtn{};
    fn_CompTrueAnomaly p_CompTrueAnomaly{};
    fn_NToA p_NToA{};
    fn_AToN p_AToN{};
    fn_KozaiToBrouwer p_KozaiToBrouwer{};
    fn_BrouwerToKozai p_BrouwerToKozai{};
    fn_KepOscToMean p_KepOscToMean{};
    fn_XYZToLLH p_XYZToLLH{};
    fn_XYZToLLHTime p_XYZToLLHTime{};
    fn_LLHToXYZ p_LLHToXYZ{};
    fn_LLHToXYZTime p_LLHToXYZTime{};
    fn_EFGToECI p_EFGToECI{};
    fn_EFGToECITime p_EFGToECITime{};
    fn_ECIToEFG p_ECIToEFG{};
    fn_ECIToEFGTime p_ECIToEFGTime{};
    fn_ECRToEFG p_ECRToEFG{};
    fn_ECRToEFGTime p_ECRToEFGTime{};
    fn_EFGToECR p_EFGToECR{};
    fn_EFGToECRTime p_EFGToECRTime{};
    fn_EFGPosToLLH p_EFGPosToLLH{};
    fn_LLHToEFGPos p_LLHToEFGPos{};
    fn_RotJ2KToDate p_RotJ2KToDate{};
    fn_RotDateToJ2K p_RotDateToJ2K{};
    fn_CompSunMoonPos p_CompSunMoonPos{};
    fn_CompSunPos p_CompSunPos{};
    fn_CompMoonPos p_CompMoonPos{};
    fn_AstroConvFrTo p_AstroConvFrTo{};
    fn_RADecToLAD p_RADecToLAD{};
    fn_AzElToLAD p_AzElToLAD{};
    fn_ECIToTopoComps p_ECIToTopoComps{};
    fn_RaDecToAzEl p_RaDecToAzEl{};
    fn_RaDecToAzElTime p_RaDecToAzElTime{};
    fn_AzElToRaDec p_AzElToRaDec{};
    fn_AzElToRaDecTime p_AzElToRaDecTime{};
    fn_RAEToECI p_RAEToECI{};
    fn_GetInitialDrag p_GetInitialDrag{};
    fn_CovMtxPTWToUVW p_CovMtxPTWToUVW{};
    fn_CovMtxUVWToPTW p_CovMtxUVWToPTW{};
    fn_EarthObstructionAngles p_EarthObstructionAngles{};
    fn_IsPointSunlit p_IsPointSunlit{};
    fn_RotRADecl p_RotRADecl{};
    fn_RotRADec_DateToEqnx p_RotRADec_DateToEqnx{};
    fn_RotRADec_EqnxToDate p_RotRADec_EqnxToDate{};
    fn_CovMtxEqnxToUVW p_CovMtxEqnxToUVW{};
    fn_CovMtxUVWToEqnx p_CovMtxUVWToEqnx{};
    fn_CovMtxECIToUVW p_CovMtxECIToUVW{};
    fn_CovMtxUVWToECI p_CovMtxUVWToECI{};
    fn_CovMtxECIToEFG p_CovMtxECIToEFG{};
    fn_CovMtxEFGToECI p_CovMtxEFGToECI{};
    fn_CovMtxECIToJ2k p_CovMtxECIToJ2k{};
    fn_CovMtxJ2kToECI p_CovMtxJ2kToECI{};
    fn_Mtx6x6ToLTA21 p_Mtx6x6ToLTA21{};
    fn_LTA21ToMtx6x6 p_LTA21ToMtx6x6{};
    fn_Mtx9x9ToLTA45 p_Mtx9x9ToLTA45{};
    fn_LTA45ToMtx9x9 p_LTA45ToMtx9x9{};
    fn_PropCovFrState p_PropCovFrState{};
    fn_CovMtxECIToEqnx p_CovMtxECIToEqnx{};
    fn_CovMtxEqnxToECI9x9 p_CovMtxEqnxToECI9x9{};
    fn_CovMtxEqnxToUVW9x9 p_CovMtxEqnxToUVW9x9{};
    fn_CovMtxUpdate p_CovMtxUpdate{};
    fn_AberrationAnnual p_AberrationAnnual{};
    fn_AberrationDiurnal p_AberrationDiurnal{};
    fn_JplSetParameters p_JplSetParameters{};
    fn_JplGetParameters p_JplGetParameters{};
    fn_JplReset p_JplReset{};
    fn_JplCompSunMoonVec p_JplCompSunMoonVec{};
    fn_JplCompSunMoonPos p_JplCompSunMoonPos{};
    fn_RemoveJpl p_RemoveJpl{};
    fn_TemeEpochToDate p_TemeEpochToDate{};
    fn_ECRToECITime p_ECRToECITime{};
    fn_ECIToECRTime p_ECIToECRTime{};
    fn_ECRToJ2KTime p_ECRToJ2KTime{};
    fn_J2KToECRTime p_J2KToECRTime{};

    SharedLibrary lib;

    static const char* resolveLibraryName() {
#ifdef _WIN32
        return "AstroFunc.dll";
#elif __APPLE__
        return "libastrofunc.dylib";
#else
        return "libastrofunc.so";
#endif
    }

    void loadSymbols() {
        p_AstroFuncInit = lib.load<fn_AstroFuncInit>("AstroFuncInit");
        p_AstroFuncGetInfo = lib.load<fn_AstroFuncGetInfo>("AstroFuncGetInfo");
        p_KepToEqnx = lib.load<fn_KepToEqnx>("KepToEqnx");
        p_KepToPosVel = lib.load<fn_KepToPosVel>("KepToPosVel");
        p_KepToUVW = lib.load<fn_KepToUVW>("KepToUVW");
        p_ClassToEqnx = lib.load<fn_ClassToEqnx>("ClassToEqnx");
        p_EqnxToClass = lib.load<fn_EqnxToClass>("EqnxToClass");
        p_EqnxToKep = lib.load<fn_EqnxToKep>("EqnxToKep");
        p_EqnxToPosVel = lib.load<fn_EqnxToPosVel>("EqnxToPosVel");
        p_PosVelToEqnx = lib.load<fn_PosVelToEqnx>("PosVelToEqnx");
        p_PosVelMuToEqnx = lib.load<fn_PosVelMuToEqnx>("PosVelMuToEqnx");
        p_PosVelToKep = lib.load<fn_PosVelToKep>("PosVelToKep");
        p_PosVelMuToKep = lib.load<fn_PosVelMuToKep>("PosVelMuToKep");
        p_PosVelToUUVW = lib.load<fn_PosVelToUUVW>("PosVelToUUVW");
        p_PosVelToPTW = lib.load<fn_PosVelToPTW>("PosVelToPTW");
        p_SolveKepEqtn = lib.load<fn_SolveKepEqtn>("SolveKepEqtn");
        p_CompTrueAnomaly = lib.load<fn_CompTrueAnomaly>("CompTrueAnomaly");
        p_NToA = lib.load<fn_NToA>("NToA");
        p_AToN = lib.load<fn_AToN>("AToN");
        p_KozaiToBrouwer = lib.load<fn_KozaiToBrouwer>("KozaiToBrouwer");
        p_BrouwerToKozai = lib.load<fn_BrouwerToKozai>("BrouwerToKozai");
        p_KepOscToMean = lib.load<fn_KepOscToMean>("KepOscToMean");
        p_XYZToLLH = lib.load<fn_XYZToLLH>("XYZToLLH");
        p_XYZToLLHTime = lib.load<fn_XYZToLLHTime>("XYZToLLHTime");
        p_LLHToXYZ = lib.load<fn_LLHToXYZ>("LLHToXYZ");
        p_LLHToXYZTime = lib.load<fn_LLHToXYZTime>("LLHToXYZTime");
        p_EFGToECI = lib.load<fn_EFGToECI>("EFGToECI");
        p_EFGToECITime = lib.load<fn_EFGToECITime>("EFGToECITime");
        p_ECIToEFG = lib.load<fn_ECIToEFG>("ECIToEFG");
        p_ECIToEFGTime = lib.load<fn_ECIToEFGTime>("ECIToEFGTime");
        p_ECRToEFG = lib.load<fn_ECRToEFG>("ECRToEFG");
        p_ECRToEFGTime = lib.load<fn_ECRToEFGTime>("ECRToEFGTime");
        p_EFGToECR = lib.load<fn_EFGToECR>("EFGToECR");
        p_EFGToECRTime = lib.load<fn_EFGToECRTime>("EFGToECRTime");
        p_EFGPosToLLH = lib.load<fn_EFGPosToLLH>("EFGPosToLLH");
        p_LLHToEFGPos = lib.load<fn_LLHToEFGPos>("LLHToEFGPos");
        p_RotJ2KToDate = lib.load<fn_RotJ2KToDate>("RotJ2KToDate");
        p_RotDateToJ2K = lib.load<fn_RotDateToJ2K>("RotDateToJ2K");
        p_CompSunMoonPos = lib.load<fn_CompSunMoonPos>("CompSunMoonPos");
        p_CompSunPos = lib.load<fn_CompSunPos>("CompSunPos");
        p_CompMoonPos = lib.load<fn_CompMoonPos>("CompMoonPos");
        p_AstroConvFrTo = lib.load<fn_AstroConvFrTo>("AstroConvFrTo");
        p_RADecToLAD = lib.load<fn_RADecToLAD>("RADecToLAD");
        p_AzElToLAD = lib.load<fn_AzElToLAD>("AzElToLAD");
        p_ECIToTopoComps = lib.load<fn_ECIToTopoComps>("ECIToTopoComps");
        p_RaDecToAzEl = lib.load<fn_RaDecToAzEl>("RaDecToAzEl");
        p_RaDecToAzElTime = lib.load<fn_RaDecToAzElTime>("RaDecToAzElTime");
        p_AzElToRaDec = lib.load<fn_AzElToRaDec>("AzElToRaDec");
        p_AzElToRaDecTime = lib.load<fn_AzElToRaDecTime>("AzElToRaDecTime");
        p_RAEToECI = lib.load<fn_RAEToECI>("RAEToECI");
        p_GetInitialDrag = lib.load<fn_GetInitialDrag>("GetInitialDrag");
        p_CovMtxPTWToUVW = lib.load<fn_CovMtxPTWToUVW>("CovMtxPTWToUVW");
        p_CovMtxUVWToPTW = lib.load<fn_CovMtxUVWToPTW>("CovMtxUVWToPTW");
        p_EarthObstructionAngles = lib.load<fn_EarthObstructionAngles>("EarthObstructionAngles");
        p_IsPointSunlit = lib.load<fn_IsPointSunlit>("IsPointSunlit");
        p_RotRADecl = lib.load<fn_RotRADecl>("RotRADecl");
        p_RotRADec_DateToEqnx = lib.load<fn_RotRADec_DateToEqnx>("RotRADec_DateToEqnx");
        p_RotRADec_EqnxToDate = lib.load<fn_RotRADec_EqnxToDate>("RotRADec_EqnxToDate");
        p_CovMtxEqnxToUVW = lib.load<fn_CovMtxEqnxToUVW>("CovMtxEqnxToUVW");
        p_CovMtxUVWToEqnx = lib.load<fn_CovMtxUVWToEqnx>("CovMtxUVWToEqnx");
        p_CovMtxECIToUVW = lib.load<fn_CovMtxECIToUVW>("CovMtxECIToUVW");
        p_CovMtxUVWToECI = lib.load<fn_CovMtxUVWToECI>("CovMtxUVWToECI");
        p_CovMtxECIToEFG = lib.load<fn_CovMtxECIToEFG>("CovMtxECIToEFG");
        p_CovMtxEFGToECI = lib.load<fn_CovMtxEFGToECI>("CovMtxEFGToECI");
        p_CovMtxECIToJ2k = lib.load<fn_CovMtxECIToJ2k>("CovMtxECIToJ2k");
        p_CovMtxJ2kToECI = lib.load<fn_CovMtxJ2kToECI>("CovMtxJ2kToECI");
        p_Mtx6x6ToLTA21 = lib.load<fn_Mtx6x6ToLTA21>("Mtx6x6ToLTA21");
        p_LTA21ToMtx6x6 = lib.load<fn_LTA21ToMtx6x6>("LTA21ToMtx6x6");
        p_Mtx9x9ToLTA45 = lib.load<fn_Mtx9x9ToLTA45>("Mtx9x9ToLTA45");
        p_LTA45ToMtx9x9 = lib.load<fn_LTA45ToMtx9x9>("LTA45ToMtx9x9");
        p_PropCovFrState = lib.load<fn_PropCovFrState>("PropCovFrState");
        p_CovMtxECIToEqnx = lib.load<fn_CovMtxECIToEqnx>("CovMtxECIToEqnx");
        p_CovMtxEqnxToECI9x9 = lib.load<fn_CovMtxEqnxToECI9x9>("CovMtxEqnxToECI9x9");
        p_CovMtxEqnxToUVW9x9 = lib.load<fn_CovMtxEqnxToUVW9x9>("CovMtxEqnxToUVW9x9");
        p_CovMtxUpdate = lib.load<fn_CovMtxUpdate>("CovMtxUpdate");
        p_AberrationAnnual = lib.load<fn_AberrationAnnual>("AberrationAnnual");
        p_AberrationDiurnal = lib.load<fn_AberrationDiurnal>("AberrationDiurnal");
        p_JplSetParameters = lib.load<fn_JplSetParameters>("JplSetParameters");
        p_JplGetParameters = lib.load<fn_JplGetParameters>("JplGetParameters");
        p_JplReset = lib.load<fn_JplReset>("JplReset");
        p_JplCompSunMoonVec = lib.load<fn_JplCompSunMoonVec>("JplCompSunMoonVec");
        p_JplCompSunMoonPos = lib.load<fn_JplCompSunMoonPos>("JplCompSunMoonPos");
        p_RemoveJpl = lib.load<fn_RemoveJpl>("RemoveJpl");
        p_TemeEpochToDate = lib.load<fn_TemeEpochToDate>("TemeEpochToDate");
        p_ECRToECITime = lib.load<fn_ECRToECITime>("ECRToECITime");
        p_ECIToECRTime = lib.load<fn_ECIToECRTime>("ECIToECRTime");
        p_ECRToJ2KTime = lib.load<fn_ECRToJ2KTime>("ECRToJ2KTime");
        p_J2KToECRTime = lib.load<fn_J2KToECRTime>("J2KToECRTime");
    }
};

// ========================= End of auto generated code ==========================

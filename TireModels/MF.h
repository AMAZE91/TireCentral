#pragma once

#include "State.h"

enum class MFFeatureLevel
{
    MF62,
    MCTyre11,
    MF52,
    MF60
};

struct MFParams
{
    //=========================================================================
    // Scaling Factors (Lambda)
    //=========================================================================
    // Longitudinal Scaling
    prec LCX = 1.0;      // Scale factor of Fx shape factor
    prec LMUX = 1.0;      // Scale factor of Fx peak friction coefficient
    prec LEX = 1.0;      // Scale factor of Fx curvature factor
    prec LKX = 1.0;      // Scale factor of slip stiffness
    prec LHX = 1.0;      // Scale factor of Fx horizontal shift
    prec LVX = 1.0;      // Scale factor of Fx vertical shift
    prec LXAL = 1.0;      // Scale factor of alpha influence on Fx

    // Lateral Scaling
    prec LCY = 1.0;      // Scale factor of Fy shape factor
    prec LMUY = 1.0;      // Scale factor of Fy peak friction coefficient
    prec LEY = 1.0;      // Scale factor of Fy curvature factor
    prec LKY = 1.0;      // Scale factor of cornering stiffness
    prec LKYC = 1.0;      // Scale factor of camber stiffness
    prec LKZC = 1.0;      // Scale factor of camber moment stiffness
    prec LHY = 1.0;      // Scale factor of Fy horizontal shift
    prec LVY = 1.0;      // Scale factor of Fy vertical shift
    prec LYKA = 1.0;      // Scale factor of kappa influence on Fy
    prec LVYKA = 1.0;      // Scale factor of kappa induced plysteer Fy
    prec LKC = 1.0;      // Scale factor of camber stiffness
    prec LCC = 1.0;      // Scale factor of camber shape factor
    prec LEC = 1.0;      // Scale factor of camber curvature factor

    // Moment Scaling
    prec LMX = 1.0;      // Scale factor of overturning moment
    prec LVMX = 1.0;      // Scale factor of Mx vertical shift
    prec LMY = 1.0;      // Scale factor of rolling resistance torque
    prec LTR = 1.0;      // Scale factor of Peak of pneumatic trail
    prec LRES = 1.0;      // Scale factor for offset of residual torque
    prec LS = 1.0;      // Scale factor of Moment arm of Fx
    prec LMP = 1.0;      // Scale factor of parking moment
    prec LGYR = 1.0;      // Scale factor gyroscopic moment

    //=========================================================================
    // Longitudinal Coefficients (Fx)
    //=========================================================================
    prec PCX1 = 1.5;     // Shape factor Cfx for longitudinal force
    prec PDX1 = 1.2;     // Longitudinal friction Mux at Fznom
    prec PDX2 = -0.2;    // Variation of friction Mux with load
    prec PDX3 = 0.0;     // Variation of friction Mux with camber
    prec PEX1 = 0.5;     // Longitudinal curvature Efx at Fznom
    prec PEX2 = 0.25;    // Variation of curvature Efx with load
    prec PEX3 = 0.0;     // Variation of curvature Efx with load squared
    prec PEX4 = 0.0;     // Factor in curvature Efx while driving
    prec PKX1 = 25.0;    // Longitudinal slip stiffness Kfx/Fz at Fznom
    prec PKX2 = -5.0;    // Variation of slip stiffness Kfx/Fz with load
    prec PKX3 = 0.25;    // Exponent in slip stiffness Kfx/Fz with load
    prec PHX1 = 0.0;     // Horizontal shift Shx at Fznom
    prec PHX2 = 0.0;     // Variation of shift Shx with load
    prec PVX1 = 0.0;     // Vertical shift Svx/Fz at Fznom
    prec PVX2 = 0.0;     // Variation of shift Svx/Fz with load

    // Combined Slip Fx
    prec RBX1 = 8.0;     // Slope factor for combined slip Fx reduction
    prec RBX2 = -2.0;    // Variation of slope Fx reduction with kappa
    prec RBX3 = 0.0;     // Influence of camber on stiffness for Fx combined
    prec RCX1 = 1.0;     // Shape factor for combined slip Fx reduction
    prec REX1 = 0.15;    // Curvature factor of combined Fx
    prec REX2 = -0.05;   // Curvature factor of combined Fx with load
    prec RHX1 = 0.0;     // Shift factor for combined slip Fx reduction

    // Pressure & Spin Effects Fx
    prec PPX1 = -0.1;    // Linear pressure effect on slip stiffness
    prec PPX2 = 0.01;    // Quadratic pressure effect on slip stiffness
    prec PPX3 = 0.2;     // Linear pressure effect on longitudinal friction
    prec PPX4 = -0.02;   // Quadratic pressure effect on longitudinal friction
    prec PDXP1 = 0.5;    // Peak Fx reduction due to spin parameter
    prec PDXP2 = 0.0;    // Peak Fx reduction due to spin with varying load parameter
    prec PDXP3 = 0.0;    // Peak Fx reduction due to spin with kappa parameter

    //=========================================================================
    // Lateral Coefficients (Fy)
    //=========================================================================
    prec PCY1 = 1.3;     // Shape factor Cfy for lateral forces
    prec PCY2 = 1.0;     // Shape factor Cfc for camber forces
    prec PDY1 = 1.1;     // Lateral friction Muy
    prec PDY2 = -0.15;   // Variation of friction Muy with load
    prec PDY3 = 0.0;     // Variation of friction Muy with squared camber
    prec PEY1 = -0.5;    // Lateral curvature Efy at Fznom
    prec PEY2 = 0.02;    // Variation of curvature Efy with load
    prec PEY3 = 0.0;     // Zero order camber dependency of curvature Efy
    prec PEY4 = 0.0;     // Variation of curvature Efy with camber
    prec PEY5 = 0.0;     // Camber curvature Efc
    prec PKY1 = 22.0;    // Maximum value of stiffness Kfy/Fznom
    prec PKY2 = 2.0012;  // Load at which Kfy reaches maximum value
    prec PKY3 = 0.0;     // Variation of Kfy/Fznom with camber
    prec PKY4 = 2.0;     // Peak stiffness variation with camber squared
    prec PKY5 = 0.0;     // Lateral stiffness dependency with camber
    prec PKY6 = 0.0;     // Camber stiffness factor
    prec PKY7 = 0.0;     // Load dependency of camber stiffness factor
    prec PHY1 = 0.0;     // Horizontal shift Shy at Fznom
    prec PHY2 = 0.0;     // Variation of shift Shy with load
    prec PHY3 = 0.0;     // Variation of shift Shy with camber
    prec PVY1 = 0.0;     // Vertical shift in Svy/Fz at Fznom
    prec PVY2 = 0.0;     // Variation of shift Svy/Fz with load
    prec PVY3 = 0.0;     // Variation of shift Svy/Fz with camber
    prec PVY4 = 0.0;     // Variation of shift Svy/Fz with camber and load

    // Combined Slip Fy
    prec RBY1 = 10.0;     // Slope factor for combined Fy reduction
    prec RBY2 = 5.5;     // Variation of slope Fy reduction with alpha
    prec RBY3 = 0.0;     // Shift term for alpha in slope Fy reduction
    prec RBY4 = 0.0;     // Influence of camber on stiffness of Fy combined
    prec RCY1 = 1.0;     // Shape factor for combined Fy reduction
    prec REY1 = 0.0;     // Curvature factor of combined Fy
    prec REY2 = 0.0;     // Curvature factor of combined Fy with load
    prec RHY1 = 0.0;     // Shift factor for combined Fy reduction
    prec RHY2 = 0.0;     // Shift factor for combined Fy reduction with load
    prec RVY1 = 0.0;     // Kappa induced side force Svyk/Muy*Fz at Fznom
    prec RVY2 = 0.0;     // Variation of Svyk/Muy*Fz with load
    prec RVY3 = 0.0;     // Variation of Svyk/Muy*Fz with camber
    prec RVY4 = 0.0;     // Variation of Svyk/Muy*Fz with alpha
    prec RVY5 = 0.0;     // Variation of Svyk/Muy*Fz with kappa
    prec RVY6 = 0.0;     // Variation of Svyk/Muy*Fz with atan(kappa)

    // Pressure & Spin Effects Fy
    prec PPY1 = 0.0;     // Pressure effect on cornering stiffness magnitude
    prec PPY2 = 0.0;     // Pressure effect on location of cornering stiffness peak
    prec PPY3 = 0.0;     // Linear pressure effect on lateral friction
    prec PPY4 = 0.0;     // Quadratic pressure effect on lateral friction
    prec PPY5 = 0.0;     // Influence of inflation pressure on camber stiffness
    prec PKYP1 = 0.0;    // Cornering stiffness reduction due to spin
    prec PDYP1 = 0.0;    // Peak Fy reduction due to spin parameter
    prec PDYP2 = 0.0;    // Peak Fy reduction due to spin with varying load parameter
    prec PDYP3 = 0.0;    // Peak Fy reduction due to spin with alpha parameter
    prec PDYP4 = 0.0;    // Peak Fy reduction due to square root of spin parameter
    prec PHYP1 = 0.0;    // Fy-alpha curve lateral shift limitation
    prec PHYP2 = 0.0;    // Fy-alpha curve maximum lateral shift parameter
    prec PHYP3 = 0.0;    // Fy-alpha curve maximum lateral shift varying with load parameter
    prec PHYP4 = 0.0;    // Fy-alpha curve maximum lateral shift parameter

    //=========================================================================
    // Overturning Moment Coefficients (Mx)
    //=========================================================================
    prec QSX1 = 0.0;     // Overturning moment offset
    prec QSX2 = 0.001;   // Camber induced overturning couple
    prec QSX3 = 0.025;   // Fy induced overturning couple
    prec QSX4 = 0.0;     // Mixed load, lateral force and camber on Mx
    prec QSX5 = 0.0;     // Load effect on Mx with lateral force and camber
    prec QSX6 = 0.8;     // B-factor of load with Mx
    prec QSX7 = 0.0;     // Camber with load on Mx
    prec QSX8 = 0.0;     // Lateral force with load on Mx
    prec QSX9 = 0.0;     // B-factor of lateral force with load on Mx
    prec QSX10 = 0.0;    // Vertical force with camber on Mx
    prec QSX11 = 0.0;    // B-factor of vertical force with camber on Mx
    prec QSX12 = 0.0;    // Camber squared induced overturning moment
    prec QSX13 = 0.0;    // Lateral force induced overturning moment
    prec QSX14 = 0.0;    // Lateral force induced overturning moment with camber
    prec PPMX1 = 0.001;  // Influence of inflation pressure on overturning moment

    //=========================================================================
    // Rolling Resistance Coefficients (My)
    //=========================================================================
    prec QSY1 = 0.01;        // Rolling resistance torque coefficient
    prec QSY2 = 0.001;       // Rolling resistance torque depending on Fx
    prec QSY3 = 0.005;       // Rolling resistance torque depending on speed
    prec QSY4 = 0.00008514;  // Rolling resistance torque depending on speed^4
    prec QSY5 = 0.0005;      // Rolling resistance torque depending on camber squared
    prec QSY6 = 0.0;         // Rolling resistance torque depending on load and camber squared
    prec QSY7 = 0.9008;      // Rolling resistance torque coefficient load dependency
    prec QSY8 = -0.4089;     // Rolling resistance torque coefficient pressure dependency

    //=========================================================================
    // Aligning Torque Coefficients (Mz)
    //=========================================================================
    prec QBZ1 = 12.035;   // Trail slope factor for trail Bpt at Fznom
    prec QBZ2 = -1.33;    // Variation of slope Bpt with load
    prec QBZ3 = 0.0;      // Variation of slope Bpt with load squared
    prec QBZ4 = 0.176;    // Variation of slope Bpt with camber
    prec QBZ5 = -0.14853; // Variation of slope Bpt with absolute camber
    prec QBZ9 = 34.5;     // Slope factor Br of residual torque Mzr
    prec QBZ10 = 0.0;     // Slope factor Br of residual torque Mzr
    prec QCZ1 = 1.2923;   // Shape factor Cpt for pneumatic trail
    prec QDZ1 = 0.09068;  // Peak trail Dpt"= Dpt*(Fz/Fznom*R0)
    prec QDZ2 = -0.00565; // Variation of peak Dpt with load
    prec QDZ3 = 0.3778;   // Variation of peak Dpt with camber
    prec QDZ4 = 0.0;      // Variation of peak Dpt with camber squared
    prec QDZ6 = 0.0017015;// Peak residual torque Dmr= Dmr/(Fz*R0)
    prec QDZ7 = -0.002091;// Variation of peak factor Dmr with load
    prec QDZ8 = -0.1428;  // Variation of peak factor Dmr with camber
    prec QDZ9 = 0.00915;  // Variation of peak factor Dmr with camber and load
    prec QDZ10 = 0.0;     // Variation of peak factor Dmr with camber squared
    prec QDZ11 = 0.0;     // Variation of Dmr with camber squared and load
    prec QEZ1 = -1.7924;  // Trail curvature Ept at Fznom
    prec QEZ2 = 0.8975;   // Variation of curvature Ept with load
    prec QEZ3 = 0.0;      // Variation of curvature Ept with load squared
    prec QEZ4 = 0.2895;   // Variation of curvature Ept with sign of Alpha-t
    prec QEZ5 = -0.6786;  // Variation of Ept with camber and sign Alpha-t
    prec QHZ1 = 0.0;      // Trail horizontal shift Sht at Fznom
    prec QHZ2 = 0.0;      // Variation of shift Sht with load
    prec QHZ3 = 0.0;      // Variation of shift Sht with camber
    prec QHZ4 = 0.0;      // Variation of shift Sht with camber and load
    prec SSZ1 = 0.00918;  // Nominal value of s/R0: effect of Fx on Mz
    prec SSZ2 = 0.03869;  // Variation of distance s/R0 with Fy/Fznom
    prec SSZ3 = 0.0;      // Variation of distance s/R0 with camber
    prec SSZ4 = 0.0;      // Variation of distance s/R0 with load and camber
    prec PPZ1 = -0.4408;  // Linear pressure effect on pneumatic trail
    prec PPZ2 = 0.0;      // Influence of inflation pressure on residual aligning torque
    prec QDTP1 = 0.0;     // Pneumatic trail reduction factor due to turn slip parameter
    prec QCRP1 = 0.0;     // Turning moment at constant turning and zero forward speed parameter
    prec QCRP2 = 0.0;     // Turn slip moment (at alpha=90deg) parameter for increase with spin
    prec QBRP1 = 0.0;     // Residual (spin) torque reduction factor parameter due to side slip
    prec QDRP1 = 0.0;     // Turn slip moment peak magnitude parameter

    //=========================================================================
    // Turn Slip Coefficients (Shared)
    //=========================================================================
    prec PECP1 = 0.0;     // Camber w.r.t spin reduction factor parameter in camber stiffness
    prec PECP2 = 0.0;     // Camber w.r.t spin reduction factor varying with load parameter in camber stiffness

    //=========================================================================
    // Operational References & Limits
    //=========================================================================
    prec FZ0 = 5000.0;    // Nominal wheel load (N)
    prec P0 = 230000.0;   // Nominal pressure used in (MF) equations (Pa) [Converted from 2.3 bar]
    prec VREF = 16.67;    // Reference speed (m/s)
    prec R0 = 0.30;       // Unloaded radius (Free tyre radius) (m)

    prec PRESMIN = 100000.0; // Minimum allowed inflation pressure (Pa)
    prec PRESMAX = 400000.0; // Maximum allowed inflation pressure (Pa)
    prec FZMIN = 1000.0;     // Minimum allowed wheel load (N)
    prec FZMAX = 10000.0;    // Maximum allowed wheel load (N)
    prec KPUMIN = -2.0;      // Minimum valid wheel slip
    prec KPUMAX = 2.0;       // Maximum valid wheel slip
    prec ALPMIN = -0.5;      // Minimum valid slip angle (rad)
    prec ALPMAX = 0.5;       // Maximum valid slip angle (rad)
    prec CAMMIN = -0.5;      // Minimum valid camber angle (rad)
    prec CAMMAX = 0.5;       // Maximum valid camber angle (rad)
    prec LFZO = 1.0;         // Scale factor of nominal (rated) load
};

TMOutput MF(const MFParams& Params, const TMState& State, MFFeatureLevel FeatureLevel, bool EnableTurnSlip)
{
    const bool MFMotorcycle = FeatureLevel == MFFeatureLevel::MCTyre11;
    const bool MF62 = FeatureLevel == MFFeatureLevel::MF62;
    const prec alpha = State.SlipAngle;
    const prec alpham = alpha;
    const prec alphaf = alpha;
    const prec gamma = State.InclinationAngle;
    const prec kappa = State.SlipRatio;
    const prec Fz0 = Params.FZ0 * Params.LFZO;
    const prec Fz = State.Fz;
    const prec FzRatio = Fz / Fz0;
    const prec dfz = (State.Fz - Fz0) / Fz0;
    const prec dfz2 = HLP::Square(dfz);
    const prec PressureRatio = State.Pressure / Params.P0;
    const prec dpi = (State.Pressure - Params.P0) / Params.P0;
    const prec dpi2 = HLP::Square(dpi);
    const prec gamma2 = HLP::Square(gamma);
    const prec epsgamma = Params.PECP1 * (prec(1) + Params.PECP2 * dfz);
    const prec absgamma = std::abs(gamma);
    const prec psi = State.Psi;
    const prec R = State.R;
    const prec sinegamma = std::sin(gamma);
    const prec phit = HLP::SafeDivide(-psi, State.Vx);

    // k = (rw - vx)/vx = k*vx = rw - vx = k*vx + vx = rw = vx(k+1) / r
    // vx(k+1) / r = w
    const prec Omega = State.Vx * (kappa + prec(1)) / R;
    const prec phif = HLP::SafeDivide(-(psi + (prec(1) - epsgamma) * Omega * sinegamma), State.Vx); // Phif = Phim = Spin
    const prec absphif = std::abs(phif);

    // Longitudinal Force (Fx)
    const prec Bxphi = Params.PDXP1 * (prec(1) + Params.PDXP2 * dfz) * HLP::CosAtan(Params.PDXP3 * kappa);
    const prec Sigma1 = EnableTurnSlip ? HLP::CosAtan(Bxphi * Params.R0 * phif) : prec(1);
    const prec Svx = Fz * (Params.PVX1 + Params.PVX2 * dfz) * Params.LVX * Params.LMUX * Sigma1;
    const prec Kxk = Fz * (Params.PKX1 + Params.PKX2 * dfz) * std::exp(Params.PKX3 * dfz) * (prec(1) + Params.PPX1 * dpi + Params.PPX2 * dpi2) * Params.LKX;
    const prec Shx = MFMotorcycle ? -(Params.QSY1 * Fz * Params.LMY + Svx) / Kxk : (Params.PHX1 + Params.PHX2 * dfz) * Params.LHX;
    const prec kappax = kappa + Shx;
    const prec Ex = HLP::SymmetricSaturation((Params.PEX1 + Params.PEX2 * dfz + Params.PEX3 * dfz2) * (prec(1) - Params.PEX4 * HLP::Sign(kappax)) * Params.LEX, prec(1));
    const prec Mux = (Params.PDX1 + Params.PDX2 * dfz) * (prec(1) + Params.PPX3 * dpi + Params.PPX4 * dpi2) * (prec(1) - Params.PDX3 * gamma2) * Params.LMUX;
    const prec Cx = Params.LCX * Params.PCX1;
    const prec Dx = Mux * Fz * Sigma1;
    const prec Bx = Kxk / (Cx * Dx);
    const prec Fx0 = HLP::Formula(kappax, Bx, Cx, Dx, Ex, Svx);

    // Lateral Force (Fy)
    prec Fyp{}, alphay{}, Cy{}, Dy{}, Muy{}, Ey{}, Kya{}, Kya0{}, Kygamma0{}, By{}, Shy0{}, Shygamma{}, Shy{}, Svy0{}, Svygamma{}, Svy{}, Egamma{}, Cgamma{}, Bgamma{};
    const prec Sigma0 = EnableTurnSlip ? prec(0) : prec(1);
    const prec Byphi = Params.PDYP1 * (prec(1) + Params.PDYP2 * dfz) * HLP::CosAtan(Params.PDYP3 * std::tan(alphaf));
    const prec Sigma2 = HLP::CosAtan(Byphi * (Params.R0 * absphif + Params.PDYP4 * std::sqrt(Params.R0 * absphif)));
    const prec Sigma3 = HLP::CosAtan(Params.PKYP1 * HLP::Square(Params.R0 * phif));
    const prec Chyphi = Params.PHYP1;
    const prec Dhyphi = Params.PHYP2 + Params.PHYP3 * dfz;
    const prec Ehyphi = Params.PHYP4;
    prec Sigma4 = prec(1);
    prec Shyphi{};

    if (MFMotorcycle)
    {
        const prec mulLMUYSigma2 = Params.LMUY * Sigma2;
        Shy = (Params.PHY1 + Params.PHY2 * dfz) * Params.LHY;
        Svy = Fz * (Params.PVY1 + Params.PVY2 * dfz) * Params.LVY * mulLMUYSigma2;
        Svygamma = Fz * (Params.PVY3 + Params.PVY4 * dfz) * gamma * Params.LKYC * mulLMUYSigma2;
        alphay = alphaf + Shy;

        Cy = Params.PCY1 * Params.LCY;
        Cgamma = Params.PCY2 * Params.LCC;
        Muy = Params.PDY1 * std::exp(Params.PDY2 * dfz) * (prec(1) + Params.PPY3 * dpi + Params.PPY4 * dpi2) * Params.LMUY * (prec(1) - Params.PDY3 * gamma2);
        Dy = Muy * Fz;
        const prec precompStiff = Params.PKY1 * Fz0 * (prec(1) + Params.PPY1 * dpi) * Params.LKY;
        const prec precompStiffAtanX = Fz0 * (prec(1) + Params.PPY2 * dpi);
        Kya = precompStiff * std::sin(Params.PKY2 * std::atan2(Fz, (Params.PKY3 + Params.PKY4 * gamma2) * precompStiffAtanX)) * (prec(1) - Params.PKY5 * gamma2) * Sigma3;
        Kya0 = precompStiff * std::sin(Params.PKY2 * std::atan2(Fz, Params.PKY3 * precompStiffAtanX));
        Kygamma0 = (Params.PKY6 + Params.PKY7 * dfz) * Fz * Params.LKYC * (prec(1) + Params.PPY5 * dpi);
        By = HLP::SafeDivide(Kya, (Cy * Dy));
        Bgamma = HLP::SafeDivide(Kygamma0, (Cgamma * Dy));
        Ey = HLP::SymmetricSaturation((Params.PEY1 + Params.PEY2 * gamma2 + (Params.PEY3 + Params.PEY4 * gamma) * HLP::Sign(alphay)) * Params.LEY, prec(1));
        Egamma = Params.PEY5 * Params.LEC;

        const prec ArgFyp1 = By * alphay;
        const prec ArgFyp2 = Bgamma * gamma;
        const prec atanTerm1 = std::atan(ArgFyp1 - Ey * (ArgFyp1 - std::atan(ArgFyp1)));
        const prec atanTerm2 = std::atan(ArgFyp2 - Egamma * (ArgFyp2 - std::atan(ArgFyp2)));
        Fyp = Dy * HLP::SinXpY(Cy * atanTerm1, Cgamma * atanTerm2) + Svy;
    }
    else
    {
        const prec precompStiff = Params.PKY1 * Fz0 * (prec(1) + Params.PPY1 * dpi) * Params.LKY;
        const prec precompStiffAtanX = Fz0 * (prec(1) + Params.PPY2 * dpi);
        Kya = precompStiff * std::sin(Params.PKY4 * std::atan2(Fz, (Params.PKY2 + Params.PKY5 * gamma2) * precompStiffAtanX)) * (prec(1) - Params.PKY3 * absgamma) * Sigma3;
        Kya0 = precompStiff * std::sin(Params.PKY4 * std::atan2(Fz, Params.PKY2 * precompStiffAtanX));

        switch (FeatureLevel)
        {
        case MFFeatureLevel::MF52:
        {
            Kygamma0 = (Params.PHY3 * Kya0 + Fz * (Params.PVY3 + Params.PVY4)) * Params.LKYC;
            break;
        }
        default:
        {
            Kygamma0 = (Params.PKY6 + Params.PKY7 * dfz) * Fz * Params.LKYC * (FeatureLevel == MFFeatureLevel::MF62 ? prec(1) + Params.PPY5 * dpi : prec(1));
            break;
        }
        }

        Svy0 = Fz * (Params.PVY1 + Params.PVY2 * dfz) * Params.LVY * Params.LMUY;
        Svygamma = Fz * (Params.PVY3 + Params.PVY4 * dfz) * gamma * Params.LKYC * Params.LMUY * Sigma2;
        Svy = (Svy0 * Sigma2) + Svygamma;
        const prec Kyrphi0 = HLP::SafeDivide(Kygamma0, (prec(1) - epsgamma));
        const prec Bhyphi = HLP::SafeDivide(-Kyrphi0, (Chyphi * Dhyphi * Kya0));
        Shyphi = HLP::Formula(Params.R0 * phif, Bhyphi, Chyphi, Dhyphi, Ehyphi, prec(0));
        Sigma4 = EnableTurnSlip ? prec(1) + Shyphi - HLP::SafeDivide(Svygamma, Kya) : prec(1);

        Shy0 = (Params.PHY1 + Params.PHY2 * dfz) * Params.LHY;
        Shygamma = FeatureLevel == MFFeatureLevel::MF52 ? (Params.PHY3 * gamma * Params.LCC) : HLP::SafeDivide((Kygamma0 * gamma - Svygamma), Kya) * ((Sigma0 + Sigma4) - prec(1));
        Shy = Shy0 + Shygamma;
        alphay = alphaf + Shy;

        Cy = Params.PCY1 * Params.LCY;
        Muy = (Params.PDY1 + Params.PDY2 * dfz) * (prec(1) + Params.PPY3 * dpi + Params.PPY4 * dpi2) * Params.LMUY * (prec(1) - Params.PDY3 * gamma2);
        Dy = Muy * Fz * Sigma2;
        Ey = HLP::SymmetricSaturation((Params.PEY1 + Params.PEY2 * dfz) * (prec(1) + Params.PEY5 * gamma2 - (Params.PEY3 + Params.PEY4 * gamma) * HLP::Sign(alphay)) * Params.LEY, prec(1));

        By = HLP::SafeDivide(Kya, (Cy * Dy));
        Fyp = HLP::Formula(alphay, By, Cy, Dy, Ey, Svy);
    }

    // Combined Slip
    const prec Shxa = Params.RHX1;
    const prec Exa = Params.REX1 + Params.REX2 * dfz;
    const prec Cxa = Params.RCX1;
    const prec Bxa = (Params.RBX1 + Params.RBX3 * gamma2) * HLP::CosAtan(Params.RBX2 * kappa) * Params.LXAL;
    const prec alphas = alphaf + Shxa;
    const prec Gxa = HLP::SafeDivide(HLP::FormulaCombinedSlip(alphas, Bxa, Cxa, Exa), HLP::FormulaCombinedSlip(Shxa, Bxa, Cxa, Exa));
    const prec Fx = Gxa * Fx0;

    const prec Dvyk = Muy * Fz * (Params.RVY1 + Params.RVY2 * dfz + Params.RVY3 * gamma) * HLP::CosAtan(Params.RVY4 * alphaf) * Sigma2;
    const prec Svyk = Dvyk * std::sin(Params.RVY5 * std::atan(Params.RVY6 * kappa)) * Params.LVYKA;

    const prec Shyk = (Params.RHY1 + Params.RHY2 * dfz);
    const prec Eyk = Params.REY1 + Params.REY2 * dfz;
    const prec Cyk = Params.RCY1;
    const prec Byk = (Params.RBY1 + Params.RBY4 * gamma2) * HLP::CosAtan(Params.RBY2 * (alphaf - Params.RBY3)) * Params.LYKA;
    const prec kappas = kappa + Shyk;
    const prec Gyk = HLP::SafeDivide(HLP::FormulaCombinedSlip(kappas, Byk, Cyk, Eyk), HLP::FormulaCombinedSlip(Shyk, Byk, Cyk, Eyk));
    const prec Fy = (Gyk * Fyp) + Svyk;

    const prec mulR0Fz0 = Params.R0 * Fz0;
    const prec FyOverFz0 = Fy / Fz0;

    // Overturning Moment (Mx)
    const prec Mx = mulR0Fz0 * Params.LMX * (Params.QSX1 - Params.QSX2 * gamma * (1.0 + Params.PPMX1 * dpi) + (Params.QSX3 * FyOverFz0) + (Params.QSX4 * (std::cos(Params.QSX5 * std::atan(HLP::Square(Params.QSX6 * (FzRatio))))) * std::sin(Params.QSX7 * gamma + Params.QSX8 * std::atan(Params.QSX9 * FyOverFz0))) + Params.QSX10 * std::atan(Params.QSX11 * (FzRatio)) * gamma) + (Params.R0 * (Fy * (Params.QSX13 + Params.QSX14 * absgamma) - Fz * Params.QSX12 * gamma * absgamma));

    // Roll Resistance Moment (My)
    const prec VxRatio = State.Vx / Params.VREF;
    const prec MyInternal = (Params.QSY1 + Params.QSY2 * (FzRatio) + Params.QSY3 * std::abs(VxRatio) + Params.QSY4 * HLP::Square(HLP::Square(VxRatio))) + (MF62 ? Params.QSY5 * gamma2 + Params.QSY6 * FzRatio * gamma2 : prec(0));
    const prec My = -mulR0Fz0 * Params.LMY * MyInternal * (MF62 ? std::pow(FzRatio, Params.QSY7) + std::pow(PressureRatio, Params.QSY8) : prec(1));

    // Self Aligning Moment (Mz)
    const prec Sht = Params.QHZ1 + Params.QHZ2 * dfz + ((Params.QHZ3 + Params.QHZ4 * dfz) * gamma);
    const prec alphat = MFMotorcycle ? alpham : alpham + Sht;
    const prec alphar = MFMotorcycle ? alpham + Sht : alphaf + Shy + HLP::SafeDivide(Svy, Kya);
    const prec eqterm = HLP::SafeDivide(Kxk, Kya) * kappa;
    const prec ateq = std::atan(std::hypot(std::tan(alphat), eqterm)) * HLP::SignF(alphat);
    const prec areq = std::atan(std::hypot(std::tan(alphar), eqterm)) * HLP::SignF(alphar);
    const prec S = (Params.SSZ1 + Params.SSZ2 * FyOverFz0 + (Params.SSZ3 + Params.SSZ4 * dfz) * gamma) * Params.LS * Params.R0;

    const prec phim = phif;
    const prec mulR0PhiM = Params.R0 * phif;
    const prec mulFzR0 = Params.R0 * Fz;
    const prec Sigma5 = EnableTurnSlip ? HLP::CosAtan(Params.QDTP1 * phim) : prec(1);
    const prec Sigma6 = EnableTurnSlip ? HLP::CosAtan(Params.QBRP1 * phim) : prec(1);
    const prec Mzphiinf = Params.QCRP1 * std::abs(Muy) * mulFzR0 * std::sqrt(FzRatio) * Params.LMP;
    const prec Mzphi90 = Mzphiinf * HLP::RcpHalfPi * std::atan(Params.QCRP2 * Params.R0 * std::abs(phit)) * Gyk;
    const prec Kzgammar0 = mulFzR0 * (Params.QDZ8 + Params.QDZ9 * dfz + (Params.QDZ10 + Params.QDZ11 * dfz) * absgamma) * Params.LKZC;
    const prec CDrphi = Params.QDRP1;
    const prec DDrphi = HLP::SafeDivide(Mzphiinf, std::sin(prec(0.5) * std::numbers::pi_v<prec> *CDrphi));
    const prec BDrphi = HLP::SafeDivide(Kzgammar0, (CDrphi * DDrphi * (prec(1) - epsgamma)));
    const prec Drphi = DDrphi * std::sin(CDrphi * std::atan(BDrphi * mulR0PhiM));
    const prec Sigma7 = EnableTurnSlip ? HLP::RcpHalfPi * std::acos(HLP::SymmetricSaturation(HLP::SafeDivide(Mzphi90, std::abs(Drphi)), prec(1))) : prec(1);
    const prec Sigma8 = EnableTurnSlip ? (1) + Drphi : prec(1);

    const prec StiffnessMuScaleY = Params.LKY / Params.LMUY;
    const prec Br = (Params.QBZ9 * StiffnessMuScaleY + Params.QBZ10 * By * Cy) * Sigma6;
    const prec Dr = mulFzR0 * Params.LMUY * cos(alpham) * ((Params.QDZ6 + Params.QDZ7 * dfz) * Params.LRES * Sigma2 + (Params.QDZ8 + Params.QDZ9 * dfz) * gamma * Params.LKC * (prec(1) + Params.PPZ2 * dpi) * Sigma0 + (Params.QDZ10 + Params.QDZ11 * dfz) * gamma * absgamma * Params.LKC * Sigma0) - Sigma8 + prec(1);
    const prec Mzr = Sigma7 == prec(1) ? Dr * HLP::CosAtan(Br * areq) : Dr * std::cos(Sigma7 * std::atan(Br * areq));

    const prec Ct = Params.QCZ1;
    const prec Bt = (Params.QBZ1 + Params.QBZ2 * dfz + Params.QBZ3 * dfz2) * (prec(1) + Params.QBZ4 * gamma + Params.QBZ5 * absgamma) * StiffnessMuScaleY;
    const prec Et = (Params.QEZ1 + Params.QEZ2 * dfz + Params.QEZ3 * dfz2) * (prec(1) + (Params.QEZ4 + Params.QEZ5 * gamma) * HLP::RcpHalfPi * std::atan(Bt * Ct * alphat));
    const prec Dtgammaterm = MFMotorcycle ? absgamma : gamma;
    prec Dt = Fz * (Params.QDZ1 + Params.QDZ2 * dfz) * (prec(1) + Params.QDZ3 * Dtgammaterm + Params.QDZ4 * gamma2) * Params.LTR * Sigma5 * Params.R0 / Fz0;
    Dt *= MFMotorcycle ? prec(1) : prec(1) - Params.PPZ1 * dpi;
    const prec t = HLP::Formula(ateq, Bt, Ct, Dt, Et, prec(0)) * std::cos(alpham);
    const bool Legacy = MFMotorcycle || FeatureLevel == MFFeatureLevel::MF52;
    const prec Mz = Legacy ? -t * Fyp * Gyk + Mzr + S * Fx : -t * Fy - Svyk + Mzr + S * Fx;

    TMOutput Output;
    Output.Fx0 = Fx0;
    Output.Fy0 = Fyp;
    Output.Fx = Fx;
    Output.Fy = Fy;
    Output.Mx = Mx;
    Output.My = My;
    Output.Mz = Mz;
    Output.Gxa = Gxa;
    Output.Gyk = Gyk;
    Output.Mux = Mux;
    Output.Muy = Muy;
    return Output;
}
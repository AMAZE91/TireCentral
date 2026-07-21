#pragma once

#include "State.h"
#include <cmath>

struct FialaParams
{
    //=========================================================================
    // Slip Stiffnesses
    //=========================================================================
    prec CSLIP = 50000.0;  // Cs: Longitudinal slip stiffness, dFx/dSs at Ss=0 (N). The .tir example's
    // 1000N is unrealistically soft for a loaded tire (Scritical = U*Fz/(2*Cs)
    // would exceed the physical +/-1 slip range and never saturate) - bumped to
    // a representative value; tune to your own tire's property file.
    prec CALPHA = 800.0;   // Ca: Cornering stiffness, dFy/dAlpha at Alpha=0 (N/deg, per .tir convention -
    //     converted to N/rad internally; matches the Fiala property-file example)

    //=========================================================================
    // Friction
    //=========================================================================
    prec UMin = 0.9;   // Sliding (full slip) friction coefficient
    prec UMax = 1.0;   // Static (zero slip) friction coefficient

    //=========================================================================
    // Geometry / Rolling Resistance
    //=========================================================================
    prec Width = 0.20;             // Tire section width (m), used in the aligning torque
    prec RollingResistance = 0.0;  // Rolling resistance coefficient (dimensionless)
};

TMOutput ClassicFiala(const FialaParams& Params, const TMState& State)
{
    const prec Fz = std::abs(State.Fz);   // N
    const prec Ss = State.SlipRatio;
    const prec Alpha = State.SlipAngle;   // rad
    const prec AbsAlpha = std::abs(Alpha);
    const prec TanAbsAlpha = std::tan(AbsAlpha);

    // Comprehensive Slip and Current Friction Coefficient
    const prec Ssalpha = std::sqrt(HLP::Square(Ss) + HLP::Square(std::tan(Alpha)));
    const prec SsalphaClamped = std::min(prec(1), Ssalpha); // keep U from going non physical after full slip
    const prec U = Params.UMax - (Params.UMax - Params.UMin) * SsalphaClamped;

    // Longitudinal Force (Fx)
    const prec Scritical = HLP::SafeDivide(U * Fz, prec(2) * Params.CSLIP);
    prec Fx{};
    if (std::abs(Ss) < Scritical)
    {
        Fx = Params.CSLIP * Ss;
    }
    else
    {
        const prec Fx1 = U * Fz;
        const prec Fx2 = HLP::SafeDivide(HLP::Square(Fx1), prec(4) * std::abs(Ss) * Params.CSLIP);
        Fx = HLP::Sign(Ss) * (Fx1 - Fx2);
    }

    // Lateral Force (Fy) and Self Aligning Moment (Mz)
    const prec CalphaRad = Params.CALPHA * HLP::R2D;
    const prec AlphaCritical = std::atan(HLP::SafeDivide(prec(3) * U * Fz, CalphaRad));
    const prec H = prec(1) - HLP::SafeDivide(CalphaRad * TanAbsAlpha, prec(3) * U * Fz);
    const prec H3 = HLP::Square(H) * H;

    prec Fy{};
    prec Mz{};
    if (AbsAlpha <= AlphaCritical)
    {
        Fy = U * Fz * (prec(1) - H3) * HLP::Sign(Alpha);
        Mz = U * Fz * Params.Width * (prec(1) - H) * H3 * HLP::Sign(Alpha);
    }
    else
    {
        Fy = U * Fz * HLP::Sign(Alpha);
        Mz = prec(0);
    }

    // Rolling Resistance Moment (My)
    const prec Omega = HLP::SafeDivide(State.Vx * (Ss + prec(1)), State.R);
    const prec My = -HLP::Sign(Omega) * Params.RollingResistance * Fz;

    TMOutput Output{};
    Output.Fx0 = Fx;
    Output.Fy0 = Fy;
    Output.Fx = Fx;
    Output.Fy = Fy;
    Output.Mx = prec(0);
    Output.My = My;
    Output.Mz = Mz;
    Output.Mux = U;
    Output.Muy = U;
    return Output;
}
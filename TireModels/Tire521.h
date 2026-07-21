#pragma once

#include "State.h"
#include <cmath>
#include <algorithm>

struct Tire521Params
{
    //=========================================================================
    // Vertical
    //=========================================================================
    prec VerticalStiffness = 206000.0;         // Kz: N/m
    prec VerticalDamping = 2060.0;             // Cz: N*s/m
    prec VerticalStiffnessExponent = 1.1;      // theta

    //=========================================================================
    // Lateral / Aligning
    //=========================================================================
    prec LateralStiffness = 50000.0;              // Ky: N/m, used for Mx and the combined-slip Mz increment
    prec CorneringStiffnessCoefficient = 50.0;     // Ka: 1/rad, exponential Fy shape factor

    //=========================================================================
    // Friction
    //=========================================================================
    prec MuStatic = 0.95;          // mu at/below MuStaticVelocity - also used directly in the Fy equation
    prec MuDynamic = 0.75;         // mu at/above MuDynamicVelocity
    prec MuStaticVelocity = 3.0;   // m/s
    prec MuDynamicVelocity = 6.0;  // m/s

    //=========================================================================
    // Rolling Resistance / Geometry
    //=========================================================================
    prec RollingResistanceCoefficient = 0.01;  // coefrr
    prec UnloadedRadius = 0.310;               // R0 (m), used for the pneumatic-trail half contact length
};

TMOutput Tire521(const Tire521Params& Params, const TMState& State)
{
    const prec Fz = std::abs(State.Fz);
    const prec Kappa = HLP::Clamp(State.SlipRatio, prec(-1), prec(1));
    constexpr prec MaxAlpha = prec(89) * HLP::D2R;
    const prec Alpha = HLP::Clamp(State.SlipAngle, -MaxAlpha, MaxAlpha);
    const prec AbsAlpha = std::abs(Alpha);
    const prec Ka = Params.CorneringStiffnessCoefficient;

    // Longitudinal Force (Fx)
    const prec AbsVsx = std::abs(Kappa) * std::abs(State.Vx);
    const prec MuVel = HLP::AdamsStep(AbsVsx, Params.MuStaticVelocity, Params.MuStatic, Params.MuDynamicVelocity, Params.MuDynamic);
    const prec FxPure = MuVel * Fz * HLP::Sign(Kappa);

    // Lateral Force (Fy)
    const prec FyPure = Params.MuStatic * Fz * (prec(1) - std::exp(-Ka * AbsAlpha)) * HLP::Sign(Alpha);

    const prec Rl = State.R; // loaded/effective radius
    const prec HalfContactLength = std::sqrt(std::max(prec(0), HLP::Square(Params.UnloadedRadius) - HLP::Square(Rl)));
    const prec PneumaticTrail = (HalfContactLength / prec(3)) * std::exp(-Ka * AbsAlpha);
    const prec MzPure = -PneumaticTrail * FyPure;

    // Combined Slip
    constexpr prec SlipEpsilon = prec(1e-6); // add to HLP?
    const prec SinAlpha = std::sin(Alpha);
    const prec KappaMag = std::abs(Kappa);
    const prec SinAlphaMag = std::abs(SinAlpha);
    const prec SlipMagnitude = std::hypot(KappaMag, SinAlphaMag);

    prec Fx{};
    prec Fy{};
    prec Mux{};
    prec Muy{};

    if (SlipMagnitude < SlipEpsilon)
    {
        Fx = FxPure;
        Fy = FyPure;
        Mux = Params.MuStatic;
        Muy = Params.MuStatic;
    }
    else
    {
        const prec Beta = std::acos(HLP::SymmetricSaturation(HLP::SafeDivide(KappaMag, SlipMagnitude), prec(1)));
        const prec TanBeta = std::tan(Beta);

        const prec MuxActMag = HLP::SafeDivide(std::abs(FxPure), Fz);
        const prec MuyActMag = HLP::SafeDivide(std::abs(FyPure), Fz);

        const prec MuxMag = HLP::SafeDivide(prec(1), std::hypot(HLP::SafeDivide(prec(1), MuxActMag), HLP::SafeDivide(TanBeta, Params.MuStatic)));
        const prec MuyMag = HLP::SafeDivide(TanBeta, std::hypot(HLP::SafeDivide(prec(1), Params.MuStatic), HLP::SafeDivide(TanBeta, MuyActMag)));

        Mux = MuxMag;
        Muy = MuyMag;

        Fx = HLP::Sign(FxPure) * HLP::SafeDivide(MuxMag, MuxActMag) * std::abs(FxPure);
        Fy = HLP::Sign(FyPure) * HLP::SafeDivide(MuyMag, MuyActMag) * std::abs(FyPure);
    }

    // Overturning Moment (Mx) and Aligning Moment (Mz)
    const prec Mx = HLP::SafeDivide(Fy, Params.LateralStiffness) * Fz;
    const prec Mz = MzPure + Fx * HLP::SafeDivide(Fy, Params.LateralStiffness);

    // Rolling Resistance Moment (My)
    const prec Omega = HLP::SafeDivide(State.Vx * (Kappa + prec(1)), State.R);
    const prec My = -HLP::Sign(Omega) * Params.RollingResistanceCoefficient * Fz;

    TMOutput Output{};
    Output.Fx0 = FxPure;
    Output.Fy0 = FyPure;
    Output.Fx = Fx;
    Output.Fy = Fy;
    Output.Mx = Mx;
    Output.My = My;
    Output.Mz = Mz;
    Output.Mux = Mux;
    Output.Muy = Muy;
    return Output;
}

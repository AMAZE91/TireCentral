#pragma once

#include "State.h"

enum class PacejkaFeatureLevel
{
    Pacejka89,
    Pacejka94
};

struct PacejkaParams
{
    //=========================================================================
    // Scaling Factors (Lambda)
    //=========================================================================
    prec DLON = 1.0;   // Longitudinal friction scaling factor (Mu)
    prec DLAT = 1.0;   // Lateral friction scaling factor (Mu)
    prec BCDLON = 1.0; // Longitudinal slip stiffness scaling factor
    prec BCDLAT = 1.0; // Lateral cornering stiffness scaling factor

    //=========================================================================
    // Longitudinal Coefficients (Fx)
    //=========================================================================
    prec B0 = 1.65;    // Cx: Shape factor
    prec B1 = 0.00;    // Dx: Load dependency on peak factor (quadratic)
    prec B2 = 1100.0;  // Dx: Peak friction coefficient Mux at nominal load (*1000)
    prec B3 = 0.00;    // BCDx: Load dependency on longitudinal stiffness (ascent rate)
    prec B4 = 300.0;   // BCDx: Longitudinal stiffness factor (at nominal load)
    prec B5 = 0.00;    // BCDx: Curvature change of stiffness with load
    prec B6 = 0.00;    // Ex: Quadratic load effect on curvature factor
    prec B7 = 0.00;    // Ex: Linear load effect on curvature factor
    prec B8 = -2.00;   // Ex: Curvature factor at nominal load
    prec B9 = 0.00;    // Shx: Load effect on horizontal shift
    prec B10 = 0.00;   // Shx: Constant horizontal shift
    prec B11 = 0.00;   // Svx: Load effect on vertical shift (Pacejka 94)
    prec B12 = 0.00;   // Svx: Constant vertical shift (Pacejka 94)
    prec B13 = 0.00;   // Ex: Driving/braking asymmetry factor (Pacejka 94)

    //=========================================================================
    // Lateral Coefficients (Fy)
    //=========================================================================
    prec A0 = 1.30;    // Cy: Shape factor
    prec A1 = 0.00;    // Dy: Load dependency on peak factor (quadratic in 89, linear in 94)
    prec A2 = 1100.0;  // Dy: Peak lateral friction Muy (*1000)
    prec A3 = 1100.0;  // BCDy: Cornering stiffness factor (at nominal load)
    prec A4 = 10.0;    // BCDy: Load at peak cornering stiffness
    prec A5 = 0.00;    // BCDy: Camber influence on cornering stiffness
    prec A6 = 0.00;    // Ey: Curvature factor change with load (linear in 89)
    prec A7 = -2.00;   // Ey: Curvature factor at nominal load
    prec A8 = 0.00;    // Shy: Camber influence on horizontal shift (89) / Load dependency (94)
    prec A9 = 0.00;    // Shy: Load dependency on horizontal shift (89) / Constant (94)
    prec A10 = 0.00;   // Shy: Constant horizontal shift (89) / Camber influence (94)
    prec A11 = 0.00;   // Svy: Camber influence with load (89) / Load effect (94)
    prec A12 = 0.00;   // Svy: Load effect on vertical shift (89) / Constant (94)
    prec A13 = 0.00;   // Svy: Constant vertical shift (89) / Camber influence with Fz^2 (94)
    prec A14 = 0.00;   // Svy: Camber influence constant (89) / Camber influence with Fz (94)
    prec A15 = 0.00;   // Dy: Camber squared effect on lateral peak friction (94)
    prec A16 = 0.00;   // Ey: Curvature change with camber angle (94)
    prec A17 = 0.00;   // Ey: Curvature shift factor (94)

    //=========================================================================
    // Aligning Torque Coefficients (Mz)
    //=========================================================================
    prec C0 = 2.20;    // Cz: Shape factor
    prec C1 = 0.00;    // Dz: Load dependency on peak factor (quadratic)
    prec C2 = -2.00;   // Dz: Peak factor at nominal load
    prec C3 = 0.00;    // BCDz: Torque stiffness slope
    prec C4 = -10.0;   // BCDz: Torque stiffness curve shape factor
    prec C5 = 0.00;    // BCDz: Exponential load influence on stiffness
    prec C6 = 0.00;    // BCDz: Camber influence on torque stiffness
    prec C7 = 0.00;    // Ez: Quadratic load effect on curvature factor
    prec C8 = 0.00;    // Ez: Linear load effect on curvature factor
    prec C9 = -1.00;   // Ez: Curvature factor at nominal load
    prec C10 = 0.00;   // Ez: Camber influence on curvature factor
    prec C11 = 0.00;   // Shz: Camber effect on horizontal shift (89) / Load effect (94)
    prec C12 = 0.00;   // Shz: Load effect on horizontal shift (89) / Constant (94)
    prec C13 = 0.00;   // Shz: Constant horizontal shift (89) / Camber effect (94)
    prec C14 = 0.00;   // Svz: Camber influence quadratic (89) / Load effect (94)
    prec C15 = 0.00;   // Svz: Camber influence linear (89) / Constant (94)
    prec C16 = 0.00;   // Svz: Load effect on vertical shift (89) / Camber influence with Fz^2 (94)
    prec C17 = 0.00;   // Svz: Constant vertical shift (89) / Camber influence with Fz (94)
    prec C18 = 0.00;   // Dz: Camber squared effect on peak factor (94)
    prec C19 = 0.00;   // Ez: Curvature change with camber angle (94)
    prec C20 = 0.00;   // Ez: Curvature shift factor (94)

    //=========================================================================
    // Operational References
    //=========================================================================
    prec Fz0 = 5000.0; // Nominal vertical load (N)
    prec R0 = 0.30;    // Unloaded tire radius (m)
    prec Vref = 16.67; // Reference speed (m/s)
    prec LateralStiffness = 190.0;    // Lateral stiffness for Mx contact patch deflection (N/rad or N)
    prec VerticalStiffness = 310.0;   // Vertical stiffness to estimate loaded radius (N/m)
    prec RollingResistance = 0.01;    // Rolling resistance coefficient (dimensionless, e.g., 0.01 = 1%)
};

TMOutput ClassicPacejka(const PacejkaParams& Params, const TMState& State, PacejkaFeatureLevel FeatureLevel)
{
    const bool Pacejka94 = FeatureLevel == PacejkaFeatureLevel::Pacejka94;

    const prec Fz = State.Fz;
    const prec FZ = Fz / prec(1000);       // Pacejka 94 and 89 use kN
    const prec FZ2 = HLP::Square(FZ);
    const prec Alpha = State.SlipAngle * HLP::R2D; // Degree
    const prec Kappa = State.SlipRatio;
    const prec Gamma = State.InclinationAngle; // rad
    const prec AbsGamma = std::abs(Gamma);
    const prec Gamma2 = HLP::Square(Gamma);

    // Longitudinal Force (Fx)
    const prec KappaPct = Kappa * prec(100); // Pacejka 94 and 89 use % (mul by 100)

    const prec Cx = Params.B0;
    const prec Dx = Pacejka94 ? (Params.B1 * FZ2 + Params.B2 * FZ) * Params.DLON : (Params.B1 * FZ2 + Params.B2 * FZ);
    const prec BCDx = Pacejka94 ? ((Params.B3 * FZ2 + Params.B4 * FZ) * std::exp(-Params.B5 * FZ)) * Params.BCDLON : (Params.B3 * FZ2 + Params.B4 * FZ) * std::exp(-Params.B5 * FZ);
    const prec Bx = HLP::SafeDivide(BCDx, Cx * Dx);
    const prec Shx = Params.B9 * FZ + Params.B10;
    const prec Svx = Pacejka94 ? (Params.B11 * FZ + Params.B12) : prec(0);
    const prec X1x = KappaPct + Shx;
    const prec Ex = Pacejka94 ? ((Params.B6 * FZ + Params.B7) * FZ + Params.B8) * (prec(1) - Params.B13 * HLP::SignF(X1x)) : (Params.B6 * FZ2 + Params.B7 * FZ + Params.B8);
    const prec Fx0 = HLP::Formula(X1x, Bx, Cx, Dx, Ex, Svx);

    // Lateral Force (Fy)
    const prec Cy = Params.A0;
    prec Dy{};
    prec BCDy{};
    prec Shy{};
    prec Svy{};
    if (Pacejka94)
    {
        Dy = ((Params.A1 * FZ + Params.A2) * (prec(1) - Params.A15 * Gamma2) * FZ) * Params.DLAT;
        BCDy = (Params.A3 * HLP::SinAtan(HLP::SafeDivide(FZ, Params.A4)) * prec(2)) * (prec(1) - Params.A5 * AbsGamma) * Params.BCDLAT;
        Shy = Params.A8 * FZ + Params.A9 + Params.A10 * Gamma;
        Svy = Params.A11 * FZ + Params.A12 + (Params.A13 * FZ2 + Params.A14 * FZ) * Gamma;
    }
    else
    {
        Dy = Params.A1 * FZ2 + Params.A2 * FZ;
        BCDy = Params.A3 * HLP::SinAtan(HLP::SafeDivide(FZ, Params.A4)) * prec(2) * (prec(1) - Params.A5 * AbsGamma);
        Shy = Params.A9 * FZ + Params.A10 + Params.A8 * Gamma;
        Svy = Params.A11 * FZ * Gamma + Params.A12 * FZ + Params.A13;
    }
    const prec By = HLP::SafeDivide(BCDy, Cy * Dy);
    const prec X1y = Alpha + Shy;
    const prec Ey = Pacejka94 ? (Params.A6 * FZ + Params.A7) * (prec(1) - ((Params.A16 * Gamma + Params.A17) * HLP::SignF(X1y))) : (Params.A6 * FZ + Params.A7);
    const prec Fy0 = HLP::Formula(X1y, By, Cy, Dy, Ey, Svy);

    // Self-Aligning Torque (Mz)
    const prec Cz = Params.C0;
    prec Dz{};
    prec Shz{};
    prec Svz{};
    if (Pacejka94)
    {
        Dz = (Params.C1 * FZ2 + Params.C2 * FZ) * (prec(1) - Params.C18 * Gamma2);
        Shz = Params.C11 * FZ + Params.C12 + Params.C13 * Gamma;
        Svz = Params.C14 * FZ + Params.C15 + (Params.C16 * FZ2 + Params.C17 * FZ) * Gamma;
    }
    else
    {
        Dz = Params.C1 * FZ2 + Params.C2 * FZ;
        Shz = Params.C11 * Gamma + Params.C12 * FZ + Params.C13;
        Svz = (Params.C14 * FZ2 + Params.C15 * FZ) * Gamma + Params.C16 * FZ + Params.C17;
    }
    const prec BCDz = (Params.C3 * FZ2 + Params.C4 * FZ) * (prec(1) - Params.C6 * AbsGamma) * std::exp(-Params.C5 * FZ);
    const prec Bz = HLP::SafeDivide(BCDz, Cz * Dz);
    const prec X1z = Alpha + Shz;
    const prec Ez = Pacejka94 ? HLP::SafeDivide((Params.C7 * FZ2 + Params.C8 * FZ + Params.C9) * (prec(1) - ((Params.C19 * Gamma + Params.C20) * HLP::SignF(X1z))), (prec(1) - Params.C10 * AbsGamma)) : (Params.C7 * FZ2 + Params.C8 * FZ + Params.C9) * (prec(1) - Params.C10 * AbsGamma);
    const prec Mz0 = HLP::Formula(X1z, Bz, Cz, Dz, Ez, Svz);

    // Combined Slip
    constexpr prec SlipEpsilon = prec(1e-6); // add to HLP?
    const prec KappaStar = KappaPct + Shx;
    const prec AlphaStar = Alpha + Shy;
    const prec SAG = std::sin(AlphaStar);
    const prec KappaStarMag = std::abs(KappaStar);
    const prec SAGMag = std::abs(SAG);
    const prec SlipMagnitude = std::hypot(KappaStarMag, SAGMag);

    prec Fx{}, Fy{}, Mux{}, Muy{};
    if (SlipMagnitude < SlipEpsilon)
    {
        Fx = Fx0;
        Fy = Fy0;
        Mux = HLP::SafeDivide(Dx, Fz);
        Muy = HLP::SafeDivide(Dy, Fz);
    }
    else
    {
        const prec Beta = std::acos(HLP::SymmetricSaturation(HLP::SafeDivide(KappaStarMag, SlipMagnitude), prec(1)));
        const prec TanBeta = std::tan(Beta);

        const prec MuxActMag = HLP::SafeDivide(std::abs(Fx0 - Svx), Fz);
        const prec MuxMaxMag = HLP::SafeDivide(std::abs(Dx), Fz);
        const prec MuyActMag = HLP::SafeDivide(std::abs(Fy0 - Svy), Fz);
        const prec MuyMaxMag = HLP::SafeDivide(std::abs(Dy), Fz);

        const prec MuxMag = HLP::SafeDivide(prec(1), std::hypot(HLP::SafeDivide(prec(1), MuxActMag), HLP::SafeDivide(TanBeta, MuyMaxMag)));
        const prec MuyMag = HLP::SafeDivide(TanBeta, std::hypot(HLP::SafeDivide(prec(1), MuxMaxMag), HLP::SafeDivide(TanBeta, MuyActMag)));

        Mux = MuxMag;
        Muy = MuyMag;

        Fx = HLP::Sign(Fx0) * HLP::SafeDivide(MuxMag, MuxActMag) * std::abs(Fx0 + Svx);
        Fy = HLP::Sign(Fy0) * HLP::SafeDivide(MuyMag, MuyActMag) * std::abs(Fy0 + Svy);
    }

    // Overturning Moment (Mx)
    const prec Deflection = HLP::SafeDivide(Fy, Params.LateralStiffness);
    const prec Mx = FZ * Deflection;
    const prec Mz = Mz0 + Fx * Deflection;

    // Rolling Resistance Moment (My)
    const prec Omega = HLP::SafeDivide(State.Vx * (Kappa + prec(1)), State.R);
    const prec My = -HLP::Sign(Omega) * Fz * State.R * Params.RollingResistance;

    TMOutput Output{};
    Output.Fx0 = Fx0;
    Output.Fy0 = Fy0;
    Output.Fx = Fx;
    Output.Fy = Fy;
    Output.Mx = Mx;
    Output.My = My;
    Output.Mz = Mz;
    Output.Mux = Mux;
    Output.Muy = Muy;
    return Output;
}
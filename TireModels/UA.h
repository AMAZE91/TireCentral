#pragma once

#include "State.h"

struct UAParams
{
    //=========================================================================
    // Slip / Camber Stiffnesses
    //=========================================================================
    prec CSLIP = 80000.0;   // Cs: Longitudinal slip stiffness, dFx/dKappa at Kappa=0 (N)
    prec CALPHA = 60000.0;  // Ca: Cornering stiffness, dFy/dAlpha at Alpha=0 (N/rad)
    prec CGAMMA = 3000.0;   // Cg: Camber stiffness, dFy/dGamma at Gamma=0 (N/rad)

    //=========================================================================
    // Friction
    //=========================================================================
    prec UMin = 0.8;   // Minimum friction coefficient (Ssag = 1, full slip)
    prec UMax = 1.1;   // Maximum friction coefficient (Ssag = 0, zero slip)

    //=========================================================================
    // Rolling Resistance / Contact Patch
    //=========================================================================
    prec RollingResistance = 0.003;    // Cr: Rolling resistance lever-arm length (m)
    prec VerticalStiffness = 190000.0; // kz: only used to approximate deflection Rho = Fz/kz
    // for the contact-patch length (no penetration state available)
};

TMOutput UATire(const UAParams& Params, const TMState& State)
{
    const prec Fz = std::abs(State.Fz);                 // N
    const prec Kappa = HLP::Clamp(State.SlipRatio, prec(-1), prec(1));
    const prec Alpha = State.SlipAngle;                  // rad
    const prec Gamma = State.InclinationAngle;           // rad

    const prec Cs = Params.CSLIP;
    const prec Ca = Params.CALPHA;
    const prec Cg = Params.CGAMMA;

    // Slip Ratios
    const prec Ss = std::abs(Kappa);
    const bool Braking = Kappa < prec(0);

    const prec TanAlpha = std::tan(Alpha);
    const prec AbsTanAlpha = std::abs(TanAlpha);
    const prec SaStar = Braking ? AbsTanAlpha : (prec(1) - Ss) * AbsTanAlpha;
    const prec Sa = std::min(prec(1), SaStar);

    const prec Sg = std::sin(Gamma);
    const prec AbsSg = std::abs(Sg);

    const prec Rho = HLP::SafeDivide(Fz, Params.VerticalStiffness);
    const prec Rl = State.R;
    const prec ContactLength = std::sqrt(std::max(prec(0), prec(8) * Rl * Rho));

    const prec SagStar = Braking ? AbsTanAlpha - HLP::SafeDivide(Sg * ContactLength, prec(2) * Rl) : (prec(1) - Ss) * AbsTanAlpha - HLP::SafeDivide(Sg * ContactLength, prec(2) * Rl);
    const prec Sag = HLP::Clamp(SagStar, prec(-1), prec(1));

    // Friction Coefficient and Friction-Circle Decomposition
    const prec SsagStarMag = std::sqrt(HLP::Square(Ss) + HLP::Square(Sag));
    const prec Ssag = std::min(prec(1), SsagStarMag);
    const prec Mu = HLP::Clamp(Params.UMax - (Params.UMax - Params.UMin) * Ssag, prec(0), Params.UMax);

    const prec CosBeta = HLP::SafeDivide(Ss, SsagStarMag);
    const prec SinBeta = HLP::SafeDivide(Sag, SsagStarMag);
    const prec Mux = Mu * CosBeta;
    const prec Muy = Mu * SinBeta;

    // 1) alpha*gamma < 0   2) >=0 and CaSa >= Cg|Sg|   3) >=0 and CaSa < Cg|Sg|
    const bool Case1 = (Alpha * Gamma) < prec(0);
    const bool Case2 = !Case1 && (Ca * Sa >= Cg * AbsSg);

    prec Sn{};
    prec MuYm{};
    bool Elastic{};

    if (Case1)
    {
        const prec Sgc = HLP::SafeDivide(Mu * Fz, Cg);
        const prec Ssc = HLP::SafeDivide(prec(3) * Mu * Fz, Cs);
        const prec Sac = HLP::SafeDivide(Cs, Ca) * std::sqrt(std::max(prec(0), HLP::Square(Ssc) - HLP::Square(Ss))) - prec(3) * HLP::SafeDivide(Cg, Ca) * Sg;

        const prec B1 = HLP::Square(prec(3) * Mu * Fz) - HLP::Square(prec(3) * Cg * Sg);
        const prec B2 = prec(2) * Ca * Sa * Cg * Sg;
        const prec B3 = -(HLP::Square(Cs * Ss) + HLP::Square(Ca * Sa));
        Sn = HLP::SafeDivide(B2 + std::sqrt(std::max(prec(0), HLP::Square(B2) - B1 * B3)), B1);

        MuYm = Muy - HLP::SafeDivide(Cg * Sg, Fz);
        Elastic = (AbsSg < Sgc) && (Ss < Ssc) && (Sa < Sac);
    }
    else if (Case2)
    {
        const prec Ssc = HLP::SafeDivide(prec(3) * Mu * Fz, Cs);
        const prec Sac = HLP::SafeDivide(Cs, Ca) * std::sqrt(std::max(prec(0), HLP::Square(Ssc) - HLP::Square(Ss))) + prec(3) * HLP::SafeDivide(Cg, Ca) * Sg;

        const prec B1 = HLP::Square(prec(3) * Mu * Fz) - HLP::Square(prec(3) * Cg * Sg);
        const prec B2 = -prec(3) * Ca * Sa * Cg * Sg;
        const prec B3 = -(HLP::Square(Cs * Ss) + HLP::Square(Ca * Sa));
        Sn = HLP::SafeDivide(B2 + std::sqrt(std::max(prec(0), HLP::Square(B2) - B1 * B3)), B1);

        MuYm = Muy + HLP::SafeDivide(Cg * Sg, Fz);
        Elastic = (Ss < Ssc) && (Sa < Sac);
    }
    else // Case 3
    {
        const prec Sgc = HLP::SafeDivide(prec(3) * Mu * Fz + Ca * Sa, prec(3) * Cg);
        const prec Ssc = HLP::SafeDivide(std::sqrt(std::max(prec(0), HLP::Square(prec(3) * Mu * Fz) - HLP::Square(Ca * Sa - prec(3) * Cg * Sg))), Cs);

        const prec B1 = HLP::Square(prec(3) * Mu * Fz) - HLP::Square(prec(3) * Cg * Sg);
        const prec B2 = -prec(3) * Ca * Sa * Cg * Sg;
        const prec B3 = -(HLP::Square(Cs * Ss) + HLP::Square(Ca * Sa));
        Sn = HLP::SafeDivide(B2 + std::sqrt(std::max(prec(0), HLP::Square(B2) - B1 * B3)), B1);

        Elastic = (AbsSg < Sgc) && (Ss < Ssc);
    }

    Sn = HLP::Clamp(Sn, prec(0), prec(1));
    const prec Ln = prec(1) - Sn;
    const prec Ln2 = HLP::Square(Ln);
    const prec Ln3 = Ln2 * Ln;
    const prec Ln4 = Ln3 * Ln;
    const prec Ln5 = Ln4 * Ln;
    const prec P1 = prec(1) - prec(3) * Ln2 + prec(2) * Ln3;

    // Force / Moment (Fxi, Feta, Mza, Mzsa, Mzsg - always >= 0 here;
    prec Fxi{};
    prec Feta{};
    prec Mza{};
    prec Mzsa{};

    if (Elastic)
    {
        Fxi = Cs * Ss * Ln2 + Mux * Fz * P1;

        if (Case1 || Case2)
        {
            Feta = Ca * Sa * Ln2 + MuYm * Fz * P1 + Cg * Sg;
            Mza = ContactLength * Ln2 * (Ca * Sa * (prec(2) / prec(3) * Ln - prec(0.5)) + prec(1.5) * MuYm * Fz * HLP::Square(Sn));
        }
        else
        {
            Feta = Cg * Sg * (prec(3) * Ln2 + prec(2) * Ln3) - Ca * Sa * Ln2 + Muy * Fz * P1;
            Mza = Ca * Sa * ContactLength * Ln2 / prec(6);
        }

        Mzsa = (prec(2) / prec(3)) * Cs * Ss * Sa * Ln3
            + HLP::SafeDivide(prec(3) * Mux * Muy * HLP::Square(Fz), prec(5) * Ca) * (prec(1) - prec(10) * Ln3 + prec(15) * Ln4 - prec(6) * Ln5);
    }
    else
    {
        Fxi = Mux * Fz;
        Feta = Muy * Fz;
        Mza = prec(0);
        Mzsa = HLP::SafeDivide(prec(3) * Mux * Muy * HLP::Square(Fz) * ContactLength, prec(5) * Ca);
    }

    const prec Eta = AbsSg * std::sqrt(std::max(prec(0), HLP::Square(Rl) - HLP::Square(ContactLength) / prec(4)));
    const prec Mzsg = Eta * Fxi;

    // to SAE Coordinate System
    const prec SignKappa = HLP::Sign(Kappa);
    const prec SignAlpha = HLP::Sign(Alpha);
    const prec SignGamma = HLP::Sign(Gamma);

    const prec Fx = SignKappa * Fxi;
    const prec Fy = (Case1 || Case2) ? SignAlpha * Feta : SignGamma * Feta;
    const prec Mz = SignAlpha * Mza + SignKappa * SignAlpha * Mzsa + SignGamma * Mzsg;

    // Rolling Resistance Moment (My)
    const prec Omega = HLP::SafeDivide(State.Vx * (Kappa + prec(1)), State.R);
    const prec My = -HLP::Sign(Omega) * Params.RollingResistance * Fz;

    TMOutput Output{};
    Output.Fx0 = Fx;
    Output.Fy0 = Fy;
    Output.Fx = Fx;
    Output.Fy = Fy;
    Output.Mx = prec(0);
    Output.My = My;
    Output.Mz = Mz;
    Output.Mux = Mux;
    Output.Muy = Muy;
    return Output;
}
#pragma once

#include "State.h"

struct DGParams
{
    //=========================================================================
    // Slip / Camber Stiffnesses
    //=========================================================================
    prec CX = 60.0;   // Cx: Longitudinal slip stiffness, dFx/dKappa at Kappa=0 (N)
    prec CY = 40000.0;  // Cy: Cornering stiffness, dFy/dAlpha at Alpha=0 (N/rad)

    //=========================================================================
    // Friction
    //=========================================================================
    prec MuF = 1.3;   // Friction coefficient
};

TMOutput DugoffTire(const DGParams& Params, const TMState& State)
{
    const prec abssf = std::abs(State.SlipRatio);
    const prec tanalphaf = std::tan(State.SlipAngle);
    const prec denominator = prec(1) - abssf;

    // Pure Slip Forces
    const prec Fyfd = HLP::SafeDivide((Params.CY * tanalphaf), denominator);
    const prec Fxfd = HLP::SafeDivide((Params.CX * State.SlipRatio), denominator);

    prec Fx{}, Fy{};
    const prec lambdaf = HLP::SafeDivide(Params.MuF, (prec(2) * std::hypot(Fyfd / State.Fz, Fxfd / State.Fz)));
    if (lambdaf >= prec(1))
    {
        Fx = Fxfd;
        Fy = Fyfd;
    }
    else
    {
        const prec twicelambdaf = lambdaf + lambdaf;
        const prec multiplier = (prec(1) - (lambdaf * prec(0.5))) * twicelambdaf;
        Fy = Fyfd * multiplier;
        Fx = Fxfd * multiplier;
    }

    TMOutput Output;
    Output.Fx0 = Fxfd;
    Output.Fy0 = Fyfd;
    Output.Fx = Fx;
    Output.Fy = Fy;
    Output.Mx = prec(0);
    Output.My = prec(0);
    Output.Mz = prec(0);
    Output.Gxa = prec(0);
    Output.Gyk = prec(0);
    Output.Mux = Params.MuF;
    Output.Muy = Params.MuF;
    return Output;
}
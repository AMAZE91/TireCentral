#pragma once

#include "State.h"

struct SimplePacejkaParams
{
	//=========================================================================
    // Longitudinal Coefficients (Fx)
	//=========================================================================

	prec Bx = prec(10.0); // Longitudinal stiffness factor.
	prec Cx = prec(1.9); // Longitudinal shape factor.
	prec Dx = prec(1.0); // Longitudinal peak friction coefficient.
	prec Ex = prec(0.97); // Longitudinal curvature factor.

	//=========================================================================
	// Lateral Coefficients (Fy)
	//=========================================================================

	prec By = prec(5.2); // Lateral cornering stiffness factor.
	prec Cy = prec(1.3); // Lateral shape factor.
	prec Dy = prec(1.0); // Lateral peak friction coefficient.
	prec Ey = prec(-1.6); // Lateral curvature factor.
};

TMOutput SimplePacejka(const SimplePacejkaParams& Params, const TMState& State)
{
	const prec MaxFx = State.Fz * Params.Dx;
	const prec Fx0 = HLP::Formula(State.SlipRatio, Params.Bx, Params.Cx, MaxFx, Params.Ex, prec(0));

	const prec MaxFy = State.Fz * Params.Dy;
	const prec Fy0 = HLP::Formula(State.SlipAngle, Params.By, Params.Cy, MaxFy, Params.Ey, prec(0));

	const prec RatioX = Fx0 / MaxFx;
	const prec RatioY = Fy0 / MaxFy;
	const prec Sc = std::hypot(RatioX, RatioY);

	prec Fx = Fx0, Fy = Fy0;
	if (Sc > prec(1))
	{
		Fx /= Sc;
		Fy /= Sc;
	}

	TMOutput Output;
	Output.Fx0 = Fx0;
	Output.Fy0 = Fy0;
	Output.Fx = Fx;
	Output.Fy = Fy;
	Output.Sc = Sc;
	Output.Mux = Params.Dx;
	Output.Muy = Params.Dy;
	return Output;
}
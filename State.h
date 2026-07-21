#pragma once

#include <numbers>
#include <cmath>
#include <type_traits>
#include <limits>
using prec = double;
static_assert(std::is_floating_point<prec>::value, "Compilation Failed: prec is not floating point");

#if defined(_MSC_VER)
#define EnsureInline __forceinline

#elif defined(__GNUC__) || defined(__clang__)
#define EnsureInline __attribute__((always_inline)) inline

#else 
#define EnsureInline inline
#endif

namespace HLP
{
	constexpr prec R2D = prec(180) / std::numbers::pi_v<prec>;
	constexpr prec D2R = std::numbers::pi_v<prec> / prec(180);
	constexpr prec RcpHalfPi = prec(2) / std::numbers::pi_v<prec>;

	EnsureInline prec Sign(prec X)
	{
		return prec(X > prec(0)) - prec(X < prec(0));
	}

	EnsureInline prec SignF(prec X)
	{
		return X >= prec(0) ? prec(1) : prec(-1);
	}

	EnsureInline prec CosAtan(prec X)
	{
		return prec(1) / std::hypot(prec(1), X);
	}

	EnsureInline prec Square(prec X)
	{
		return X * X;
	}

	EnsureInline prec Formula(prec X, prec B, prec C, prec D, prec E, prec Sv)
	{
		const prec Arg = B * X;
		return D * std::sin(C * std::atan(Arg - E * (Arg - std::atan(Arg)))) + Sv;
	}

	EnsureInline prec FormulaCombinedSlip(prec X, prec B, prec C, prec E)
	{
		const prec Arg = B * X;
		return std::cos(C * std::atan(Arg - E * (Arg - std::atan(Arg))));
	}

	EnsureInline prec Sin2Atan2(prec Y, prec X)
	{
		return prec(2) * X * Y / (Square(X) + Square(Y));
	}

	EnsureInline prec SinAtan2(prec Y, prec X)
	{
		return Y / std::hypot(X, Y);
	}

	EnsureInline prec SinAtan(prec X)
	{
		return X / std::hypot(X, prec(1));
	}

	EnsureInline prec SinXpY(prec X, prec Y)
	{
		return std::sin(X) * std::cos(Y) + std::sin(Y) * std::cos(X);
	}

	EnsureInline prec SymmetricSaturation(prec X, prec Y)
	{
	    return std::max(-Y, std::min(X, Y));
	}

	EnsureInline prec Clamp(prec X, prec Min, prec Max)
	{
		return std::max(Min, std::min(X, Max));
	}

	EnsureInline prec SafeDivide(prec X, prec Y)
	{
		const prec R = X / Y;
		return std::isinf(R) || std::isnan(R) ? prec(0) : R;
	}

	EnsureInline prec AdamsStep(prec X, prec X0, prec H0, prec X1, prec H1)
	{
		if (X <= X0)
		{
			return H0;
		}
		if (X >= X1)
		{
			return H1;
		}

		const prec Xi = HLP::SafeDivide(X - X0, X1 - X0);
		return H0 + (H1 - H0) * Xi * Xi * (prec(3) - prec(2) * Xi);
	}
}

struct TMState
{
	prec Fz; // N
	prec SlipAngle; // rad
	prec SlipRatio;
	prec InclinationAngle; // rad (Unused in Simple)
	prec Pressure; // pa (Unused in Simple and Classic)
	prec Psi; // turn slip
	prec Vx; // (Unused in Simple) m/s
	prec R;
};

struct TMOutput
{
	prec Fx0; // Long Force
	prec Fy0; // Lat Force
	prec Fx; // Combined Grip
	prec Fy; // Combined Grip


	// Exclusive to Friction Circle Combined Grip
	prec Sc;

	// Exclusive to MF
	prec Mx;
	prec My;
	prec Mz;
	prec Gxa;
	prec Gyk;
	prec Mux;
	prec Muy;
};
// HSL.h

#pragma once

#include "..\Color.h"

namespace PS
{
	namespace internal
	{
		/////////////////////////////////////////////////////////////////////
		// HLS
		/////////////////////////////////////////////////////////////////////

		struct HSL
		{
			HSL();
			HSL(int H, int S, int L);
			HSL(const Color& C);

			Color TurnToRGB();

			friend bool operator!=(HSL& first, HSL& second);

			static double EPSILON;

			double Hue;
			double Saturation;
			double Luminance;

		private:

			double HueToRGB(double arg1, double arg2, double H);

		};
	};
};
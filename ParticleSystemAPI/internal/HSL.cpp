// HSL.cpp

#include "HSL.h"

#include <algorithm>

namespace PS
{
	namespace internal
	{
		/////////////////////////////////////////////////////////////////////
		// HSL
		/////////////////////////////////////////////////////////////////////

		double HSL::EPSILON = 0.00000000000001;

		HSL::HSL()
			: Hue(0)
			, Saturation(0)
			, Luminance(0)
		{}

		HSL::HSL(const Color& C)
		{
			double R, G, B;
			R = (float)C.R / 255.0;
			G = (float)C.G / 255.0;
			B = (float)C.B / 255.0;

			double max, min, l, s = 0.f;

			max = std::max(std::max(R, G), B);

			min = std::min(std::min(R, G), B);

			l = ((max + min) / 2.0);

			if (max - min <= HSL::EPSILON)
			{
				Hue = 0;
				Saturation = 0;
			}
			else
			{
				double diff = max - min;

				if (Luminance < 0.5)
				{
					s = diff / (max + min);
				}
				else
				{
					s = diff / (2 - max - min);
				}

				double diffR = (((max - R) * 60) + (diff / 2.0)) / diff;
				double diffG = (((max - G) * 60) + (diff / 2.0)) / diff;
				double diffB = (((max - B) * 60) + (diff / 2.0)) / diff;


				if (max - R <= HSL::EPSILON) { Hue = diffB - diffG; }
				else if (max - G <= HSL::EPSILON) { Hue = (1 * 360) / 3.0 + (diffR - diffB); }
				else if (max - B <= HSL::EPSILON) { Hue = (2 * 360) / 3.0 + (diffG - diffR); }

				if (Hue <= 0 || Hue >= 360) { fmod(Hue, 360); }

				s *= 100;
			}

			l *= 100;
			Saturation = s;
			Luminance = l;

			if (Hue < 0) { Hue = 360 - abs(Hue); }
		}

		HSL::HSL(int H, int S, int L)
		{
			///Range control for Hue.
			if (H <= 360 && H >= 0) { Hue = H; }
			else
			{
				if (H > 360) { Hue = H % 360; }
				else if (H < 0 && H > -360) { Hue = -H; }
				else if (H < 0 && H < -360) { Hue = -(H % 360); }
			}

			///Range control for Saturation.
			if (S <= 100 && S >= 0) { Saturation = S; }
			else
			{
				if (S > 100) { Saturation = S % 100; }
				else if (S < 0 && S > -100) { Saturation = -S; }
				else if (S < 0 && S < -100) { Saturation = -(S % 100); }
			}

			///Range control for Luminance
			if (L <= 100 && L >= 0) { Luminance = L; }
			else
			{
				if (L > 100) { Luminance = L % 100; }
				if (L < 0 && L > -100) { Luminance = -L; }
				if (L < 0 && L < -100) { Luminance = -(L % 100); }
			}

		}

		double HSL::HueToRGB(double arg1, double arg2, double H)
		{
			if (H < 0) H += 1;
			if (H > 1) H -= 1;
			if ((6 * H) < 1) { return (arg1 + (arg2 - arg1) * 6 * H); }
			if ((2 * H) < 1) { return arg2; }
			if ((3 * H) < 2) { return (arg1 + (arg2 - arg1) * ((2.0 / 3.0) - H) * 6); }
			return arg1;
		}

		Color HSL::TurnToRGB()
		{
			///Reconvert to range [0,1]
			double H = Hue / 360.0;
			double S = Saturation / 100.0;
			double L = Luminance / 100.0;

			double arg1, arg2;

			if (S <= EPSILON)
			{
				Color C((unsigned char)L * 255, (unsigned char)L * 255, (unsigned char)L * 255);
				return C;
			}
			else
			{
				if (L < 0.5) { arg2 = L * (1 + S); }
				else { arg2 = (L + S) - (S * L); }
				arg1 = 2 * L - arg2;

				double r = (255 * HueToRGB(arg1, arg2, (H + 1.0 / 3.0)));
				double g = (255 * HueToRGB(arg1, arg2, H));
				double b = (255 * HueToRGB(arg1, arg2, (H - 1.0 / 3.0)));
				Color C((unsigned char)r, (unsigned char)g, (unsigned char)b);
				return C;
			}

		}

		bool operator!=(HSL& first, HSL& second)
		{
			return (first.Hue != second.Hue ||
				first.Saturation != second.Saturation ||
				first.Luminance != second.Luminance);
		}
	};
};
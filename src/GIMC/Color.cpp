// Color.cpp

#include "Color.h"

namespace GIMC
{
	/////////////////////////////////////////////////////////////////////
	// Color
	/////////////////////////////////////////////////////////////////////

	const Color Color::Black(0, 0, 0);
	const Color Color::White(255, 255, 255);
	const Color Color::Red(255, 0, 0);
	const Color Color::Green(0, 255, 0);
	const Color Color::Blue(0, 0, 255);
	const Color Color::Yellow(255, 255, 0);
	const Color Color::Magenta(255, 0, 255);
	const Color Color::Cyan(0, 255, 255);
	const Color Color::Transparent(0, 0, 0, 0);

	Color::Color()
		: R(255)
		, G(255)
		, B(255)
		, A(255)
	{}

	Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		: R(r)
		, G(g)
		, B(b)
		, A(a)
	{
		clampColor();
	}

	bool operator!=(Color& first, Color& second)
	{
		return (first.R != second.R ||
			first.G != second.G ||
			first.B != second.B ||
			first.A != second.A);
	}

	// Private

	void Color::clampColor()
	{
		R = clampValue(R);
		G = clampValue(G);
		B = clampValue(B);
		A = clampValue(A);
	}

	unsigned char Color::clampValue(unsigned char value)
	{
		if (value > 255)
			return 255;

		if (value < 0)
			return 0;

		return value;
	}
};
// Color.h

#pragma once

namespace GIMC
{
	/////////////////////////////////////////////////////////////////////
	// Color
	/////////////////////////////////////////////////////////////////////

	struct Color
	{
		Color();
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

		friend bool operator!=(Color& first, Color& second);

	public:

		unsigned char R, G, B, A;

		static const Color Black;       ///< Black predefined color
		static const Color White;       ///< White predefined color
		static const Color Red;         ///< Red predefined color
		static const Color Green;       ///< Green predefined color
		static const Color Blue;        ///< Blue predefined color
		static const Color Yellow;      ///< Yellow predefined color
		static const Color Magenta;     ///< Magenta predefined color
		static const Color Cyan;        ///< Cyan predefined color
		static const Color Transparent; ///< Transparent (black) predefined color

	private:

		void clampColor();
		unsigned char clampValue(unsigned char value);
	};
};
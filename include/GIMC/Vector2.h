// Vector2.h

#pragma once

namespace GIMC
{
	/////////////////////////////////////////////////////////////////////
	// Vector2
	/////////////////////////////////////////////////////////////////////

	struct Vector2
	{
		Vector2();
		Vector2(float x, float y);
		
		static Vector2 CreateUnit(float direction);

		Vector2& operator=(const Vector2& rhs);
		Vector2& operator+=(const Vector2& rhs);
		Vector2& operator-=(const Vector2& rhs);

		Vector2& operator*=(const float value);
		Vector2& operator/=(const float value);

		Vector2 operator+(const Vector2& rhs);
		Vector2 operator-(const Vector2& rhs);
		Vector2 operator*(const Vector2& rhs);
		Vector2 operator/(const Vector2& rhs);

		Vector2 operator*(float value);

		friend bool operator!=(Vector2& first, Vector2& second);

		float Length();
		void Normalize();
		float Distance(Vector2& target);

	public:

		float X, Y;
	};
};
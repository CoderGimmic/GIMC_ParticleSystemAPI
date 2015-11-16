// Vector2.cpp

#include "Vector2.h"

#include "internal/Math.h"

#include <cmath>

namespace PS
{
	/////////////////////////////////////////////////////////////////////
	// Vector2
	/////////////////////////////////////////////////////////////////////

	Vector2::Vector2()
		: X(0.0f)
		, Y(0.0f)
	{}

	Vector2 Vector2::CreateUnit(float direction)
	{
		float angle = direction * Math::degToRad;
		return (Vector2(cos(angle), sin(angle)));
	}

	Vector2::Vector2(float x, float y)
		: X(x)
		, Y(y)
	{}

	Vector2& Vector2::operator=(const Vector2& rhs)
	{
		X = rhs.X;
		Y = rhs.Y;
		return *this;
	}

	Vector2& Vector2::operator+=(const Vector2& rhs)
	{
		X += rhs.X;
		Y += rhs.Y;
		return *this;
	}

	Vector2& Vector2::operator-=(const Vector2& rhs)
	{
		X -= rhs.X;
		Y -= rhs.Y;
		return *this;
	}

	Vector2& Vector2::operator*=(const float value)
	{
		X *= value;
		Y *= value;
		return *this;
	}

	Vector2& Vector2::operator/=(const float value)
	{
		X /= value;
		Y /= value;
		return *this;
	}

	Vector2 Vector2::operator+(const Vector2& rhs)
	{
		return Vector2(X + rhs.X, Y + rhs.Y);
	}

	Vector2 Vector2::operator-(const Vector2& rhs)
	{
		return Vector2(X - rhs.X, Y - rhs.Y);
	}

	Vector2 Vector2::operator*(const Vector2& rhs)
	{
		return Vector2(X * rhs.X, Y * rhs.Y);
	}

	Vector2 Vector2::operator/(const Vector2& rhs)
	{
		return Vector2(X / rhs.X, Y / rhs.Y);
	}

	Vector2 Vector2::operator*(float value)
	{
		return Vector2(X * value, Y * value);
	}

	bool operator!=(Vector2& first, Vector2& second)
	{
		return (first.X != second.X ||
			first.Y != second.Y);
	}

	float Vector2::Length()
	{
		return std::sqrt(X * X + Y * Y);
	}

	void Vector2::Normalize()
	{
		float len = Length();
		if (len > 0.0f)
		{
			float inverted = 1.0f / len;
			X *= inverted;
			Y *= inverted;
		}
	}

	float Vector2::Distance(Vector2& target)
	{
		Vector2 difference = target - *this;
		return (difference.Length());
	}
};
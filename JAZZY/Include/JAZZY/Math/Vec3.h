#pragma once
#include <JAZZY/Core/Core.h>

namespace jazzy
{
	class Vec3
	{
	public:
		Vec3() = default;
		Vec3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}

		Vec3& operator+=(const Vec3& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}

		Vec3& operator*=(float scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}
	public:
		f32 x{}, y{}, z{};
	};

	inline Vec3 operator+(Vec3 lhs, const Vec3& rhs)
	{
		lhs += rhs;
		return lhs;
	}

	inline Vec3 operator*(Vec3 v, float scalar)
	{
		v *= scalar;
		return v;
	}
}
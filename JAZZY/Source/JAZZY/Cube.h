#pragma once
#include <JAZZY/Math/Vec3.h>

namespace jazzy
{
	class Cube
	{
	public:
		Cube(Vec3 _position, Vec3 _scale, Vec3 _rotation);
		Vec3 position{};
		Vec3 scale{};
		Vec3 rotation{};
	};
}


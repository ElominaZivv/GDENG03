#pragma once
#include <JAZZY/Math/Vec3.h>
#include <JAZZY/Math/Vec2.h>

namespace jazzy
{
	class Ball
	{
	public:
		Ball(Vec3 position, Vec3 velocity, f32 radius);
		f32 radius{1.0f};
		Vec3 position{};
		Vec3 velocity{};
	};
}


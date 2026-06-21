#pragma once
#include <JAZZY/Core/Core.h>

namespace jazzy
{
	class Vec2
	{
	public:
		Vec2() = default;
		Vec2(f32 x, f32 y) : x(x), y(y){}
	public:
		f32 x{}, y{};
	};
}
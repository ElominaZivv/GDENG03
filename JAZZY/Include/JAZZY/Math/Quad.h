#pragma once
#include <JAZZY/Math/Vertex.h>
#include <JAZZY/Math/Triangle.h>

namespace jazzy
{
	class Quad
	{
	public:
		Quad() = default;
		Quad(Vertex topLeft, Vertex topRight, Vertex bottomRight, Vertex bottomLeft);
		Triangle leftTriangle{}, rightTriangle{};
	};
}
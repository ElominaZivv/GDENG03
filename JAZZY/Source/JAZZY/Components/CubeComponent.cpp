#include <JAZZY/Components/CubeComponent.h>
#include <JAZZY/Math/Vertex.h>

#include "JAZZY/Graphics/GraphicsDevice.h"

jazzy::CubeComponent::CubeComponent(const ComponentDesc& data): Component(data)
{
	f32 cubeSize = 0.5f;
	Vec4 orange(0.99f, 0.16f, 0.01f, 1.0f);
	Vec4 mikublue(0.03f, 0.74f, 0.68f, 1.0f);
	Vec4 ourple(0.34f, 0.0f, 0.94f, 1.0f);
	Vec4 black(0.0f, 0.0f, 0.0f, 1.0f);

	// Vertex Buffer
	static const Vertex vertexList[] =
	{
		// Front Face
		Vertex{ {-cubeSize,-cubeSize,-cubeSize}, orange},
		Vertex{ {-cubeSize,cubeSize,-cubeSize}, mikublue},
		Vertex{ {cubeSize,cubeSize,-cubeSize}, ourple},
		Vertex{ {cubeSize,-cubeSize,-cubeSize}, black},
		// Back Face
		Vertex{ {cubeSize,-cubeSize,cubeSize}, orange},
		Vertex{ {cubeSize,cubeSize,cubeSize}, mikublue},
		Vertex{ {-cubeSize,cubeSize,cubeSize}, ourple},
		Vertex{ {-cubeSize,-cubeSize,cubeSize}, black}
	};

	// Index Buffer
	static const ui32 indexList[] =
	{
		// Front Side
		0,1,2,
		2,3,0,
		// Back Side
		4,5,6,
		6,7,4,
		// Top Side
		1,6,5,
		5,2,1,
		// Bottom Side
		7,0,3,
		3,4,7,
		// Left Side
		3,2,5,
		5,4,3,
		// Right Side
		7,6,1,
		1,0,7
	};

	static const auto vb = m_context.device.createVertexBuffer({ vertexList, std::size(vertexList), sizeof(Vertex) });
	static const auto ib = m_context.device.createIndexBuffer({ indexList, std::size(indexList)});

	m_vb = vb;
	m_ib = ib;
}

jazzy::VertexBuffer& jazzy::CubeComponent::getVertexBuffer()
{
	return *m_vb;
}

jazzy::IndexBuffer& jazzy::CubeComponent::getIndexBuffer()
{
	return *m_ib;
}

#include <JAZZY/Components/CubeComponent.h>
#include "JAZZY/Graphics/GraphicsDevice.h"

jazzy::CubeComponent::CubeComponent(const ComponentDesc& data): Component(data)
{
	// Vertices
	static const MeshVertex vertexList[] =
	{
		// Front (+Z)
		{{-0.5f, -0.5f,  0.5f}, {0, 1},{0,0,1}},
		{{-0.5f,  0.5f,  0.5f}, {0, 0},{0,0,1}},
		{{ 0.5f,  0.5f,  0.5f}, {1, 0},{0,0,1}},
		{{ 0.5f, -0.5f,  0.5f}, {1, 1},{0,0,1}},

		// Back (-Z)
		{{ 0.5f, -0.5f, -0.5f}, {0, 1},{0,0,-1}},
		{{ 0.5f,  0.5f, -0.5f}, {0, 0},{0,0,-1}},
		{{-0.5f,  0.5f, -0.5f}, {1, 0},{0,0,-1}},
		{{-0.5f, -0.5f, -0.5f}, {1, 1},{0,0,-1}},

		// Left (-X)
		{{-0.5f, -0.5f, -0.5f}, {0, 1},{-1,0,0}},
		{{-0.5f,  0.5f, -0.5f}, {0, 0},{-1,0,0}},
		{{-0.5f,  0.5f,  0.5f}, {1, 0},{-1,0,0}},
		{{-0.5f, -0.5f,  0.5f}, {1, 1},{-1,0,0}},

		// Right (+X)
		{{ 0.5f, -0.5f,  0.5f}, {0, 1},{1,0,0}},
		{{ 0.5f,  0.5f,  0.5f}, {0, 0},{1,0,0}},
		{{ 0.5f,  0.5f, -0.5f}, {1, 0},{1,0,0}},
		{{ 0.5f, -0.5f, -0.5f}, {1, 1},{1,0,0}},

		// Top (+Y)
		{{-0.5f,  0.5f,  0.5f}, {0, 1},{0,1,0}},
		{{-0.5f,  0.5f, -0.5f}, {0, 0},{0,1,0}},
		{{ 0.5f,  0.5f, -0.5f}, {1, 0},{0,1,0}},
		{{ 0.5f,  0.5f,  0.5f}, {1, 1},{0,1,0}},

		// Bottom (-Y)
		{{-0.5f, -0.5f, -0.5f}, {0, 1},{0,-1,0}},
		{{-0.5f, -0.5f,  0.5f}, {0, 0},{0,-1,0}},
		{{ 0.5f, -0.5f,  0.5f}, {1, 0},{0,-1,0}},
		{{ 0.5f, -0.5f, -0.5f}, {1, 1},{0,-1,0}},
	};

	// Index Buffer
	static const ui32 indexList[] =
	{
		 0,  2,  1,   0,  3,  2,   // Front
		 4,  6,  5,   4,  7,  6,   // Back
		 8, 10,  9,   8, 11, 10,   // Left
		12, 14, 13,  12, 15, 14,   // Right
		16, 18, 17,  16, 19, 18,   // Top
		20, 22, 21,  20, 23, 22    // Bottom
	};

	static const auto vb = m_context.device.createVertexBuffer({ vertexList, std::size(vertexList), sizeof(MeshVertex) });
	static const auto ib = m_context.device.createIndexBuffer({ indexList, std::size(indexList)});

	m_vb = vb;
	m_ib = ib;
}

void jazzy::CubeComponent::setMaterial(const RefPtr<MaterialResource>& material)
{
	m_material = material;
}


jazzy::MaterialResource* jazzy::CubeComponent::getMaterial()
{
	return m_material.get();
}

jazzy::VertexBuffer& jazzy::CubeComponent::getVertexBuffer()
{
	return *m_vb;
}

jazzy::IndexBuffer& jazzy::CubeComponent::getIndexBuffer()
{
	return *m_ib;
}

#include <JAZZY/Components/PlaneComponent.h>
#include "JAZZY/Graphics/GraphicsDevice.h"

jazzy::PlaneComponent::PlaneComponent(const ComponentDesc& data) : Component(data)
{
	static const MeshVertex vertexList[] =
	{
		{{-0.5f, 0.0f,  0.5f}, {0, 1}, {0, 1, 0}},
		{{-0.5f, 0.0f, -0.5f}, {0, 0}, {0, 1, 0}},
		{{ 0.5f, 0.0f, -0.5f}, {1, 0}, {0, 1, 0}},
		{{ 0.5f, 0.0f,  0.5f}, {1, 1}, {0, 1, 0}},
	};

	static const ui32 indexList[] =
	{
		0, 2, 1,
		0, 3, 2
	};

	static const auto vb = m_context.device.createVertexBuffer({ vertexList, std::size(vertexList), sizeof(MeshVertex) });
	static const auto ib = m_context.device.createIndexBuffer({ indexList, std::size(indexList) });

	m_vb = vb;
	m_ib = ib;
}

void jazzy::PlaneComponent::setMaterial(const RefPtr<MaterialResource>& material)
{
	m_material = material;
}

jazzy::MaterialResource* jazzy::PlaneComponent::getMaterial()
{
	return m_material.get();
}

jazzy::VertexBuffer& jazzy::PlaneComponent::getVertexBuffer()
{
	return *m_vb;
}

jazzy::IndexBuffer& jazzy::PlaneComponent::getIndexBuffer()
{
	return *m_ib;
}

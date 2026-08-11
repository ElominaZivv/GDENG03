#include <JAZZY/Components/SphereComponent.h>
#include <JAZZY/Math/MathUtils.h>
#include "JAZZY/Graphics/GraphicsDevice.h"

#include <cmath>
#include <vector>

namespace
{
	constexpr jazzy::ui32 SphereSegmentsX = 32;
	constexpr jazzy::ui32 SphereSegmentsY = 32;
	constexpr jazzy::f32 SphereRadius = 0.5f;
}

jazzy::SphereComponent::SphereComponent(const ComponentDesc& data) : Component(data)
{
	static RefPtr<VertexBuffer> vb{};
	static RefPtr<IndexBuffer> ib{};

	if (!vb || !ib)
	{
		std::vector<MeshVertex> vertexList{};
		std::vector<ui32> indexList{};

		vertexList.reserve((SphereSegmentsX + 1) * (SphereSegmentsY + 1));
		indexList.reserve(SphereSegmentsX * SphereSegmentsY * 6);

		for (auto y = 0u; y <= SphereSegmentsY; ++y)
		{
			const auto v = static_cast<f32>(y) / static_cast<f32>(SphereSegmentsY);
			const auto phi = v * jazzy::MathUtils::PI;

			for (auto x = 0u; x <= SphereSegmentsX; ++x)
			{
				const auto u = static_cast<f32>(x) / static_cast<f32>(SphereSegmentsX);
				const auto theta = u * 2.0f * jazzy::MathUtils::PI;

				const Vec3 normal
				{
					std::sin(phi) * std::cos(theta),
					std::cos(phi),
					std::sin(phi) * std::sin(theta)
				};

				vertexList.push_back(MeshVertex
				{
					normal * SphereRadius,
					{u, v},
					normal
				});
			}
		}

		const auto rowSize = SphereSegmentsX + 1;
		for (auto y = 0u; y < SphereSegmentsY; ++y)
		{
			for (auto x = 0u; x < SphereSegmentsX; ++x)
			{
				const auto topLeft = y * rowSize + x;
				const auto topRight = topLeft + 1;
				const auto bottomLeft = (y + 1) * rowSize + x;
				const auto bottomRight = bottomLeft + 1;

				indexList.push_back(topLeft);
				indexList.push_back(topRight);
				indexList.push_back(bottomLeft);
				indexList.push_back(bottomLeft);
				indexList.push_back(topRight);
				indexList.push_back(bottomRight);
			}
		}

		vb = m_context.device.createVertexBuffer({ vertexList.data(), static_cast<ui32>(vertexList.size()), sizeof(MeshVertex) });
		ib = m_context.device.createIndexBuffer({ indexList.data(), static_cast<ui32>(indexList.size()) });
	}

	m_vb = vb;
	m_ib = ib;
}

void jazzy::SphereComponent::setMaterial(const RefPtr<MaterialResource>& material)
{
	m_material = material;
}

jazzy::MaterialResource* jazzy::SphereComponent::getMaterial()
{
	return m_material.get();
}

jazzy::VertexBuffer& jazzy::SphereComponent::getVertexBuffer()
{
	return *m_vb;
}

jazzy::IndexBuffer& jazzy::SphereComponent::getIndexBuffer()
{
	return *m_ib;
}

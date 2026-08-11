#include <JAZZY/Components/CylinderComponent.h>
#include <JAZZY/Math/MathUtils.h>
#include "JAZZY/Graphics/GraphicsDevice.h"

#include <cmath>
#include <vector>

namespace
{
	constexpr jazzy::ui32 CylinderSegments = 32;
	constexpr jazzy::f32 CylinderRadius = 0.5f;
	constexpr jazzy::f32 HalfCylinderHeight = 0.5f;
}

jazzy::CylinderComponent::CylinderComponent(const ComponentDesc& data) : Component(data)
{
	static RefPtr<VertexBuffer> vb{};
	static RefPtr<IndexBuffer> ib{};

	if (!vb || !ib)
	{
		std::vector<MeshVertex> vertexList{};
		std::vector<ui32> indexList{};

		vertexList.reserve(2 + (CylinderSegments + 1) * 4);
		indexList.reserve(CylinderSegments * 12);

		const auto topCenterIndex = static_cast<ui32>(vertexList.size());
		vertexList.push_back({ {0.0f, HalfCylinderHeight, 0.0f}, {0.5f, 0.5f}, {0.0f, 1.0f, 0.0f} });

		const auto bottomCenterIndex = static_cast<ui32>(vertexList.size());
		vertexList.push_back({ {0.0f, -HalfCylinderHeight, 0.0f}, {0.5f, 0.5f}, {0.0f, -1.0f, 0.0f} });

		const auto topCapStart = static_cast<ui32>(vertexList.size());
		for (auto i = 0u; i <= CylinderSegments; ++i)
		{
			const auto u = static_cast<f32>(i) / static_cast<f32>(CylinderSegments);
			const auto angle = u * 2.0f * jazzy::MathUtils::PI;
			const auto x = CylinderRadius * std::cos(angle);
			const auto z = CylinderRadius * std::sin(angle);

			vertexList.push_back({
				{x, HalfCylinderHeight, z},
				{0.5f + (x / (CylinderRadius * 2.0f)), 0.5f - (z / (CylinderRadius * 2.0f))},
				{0.0f, 1.0f, 0.0f}
			});
		}

		const auto bottomCapStart = static_cast<ui32>(vertexList.size());
		for (auto i = 0u; i <= CylinderSegments; ++i)
		{
			const auto u = static_cast<f32>(i) / static_cast<f32>(CylinderSegments);
			const auto angle = u * 2.0f * jazzy::MathUtils::PI;
			const auto x = CylinderRadius * std::cos(angle);
			const auto z = CylinderRadius * std::sin(angle);

			vertexList.push_back({
				{x, -HalfCylinderHeight, z},
				{0.5f + (x / (CylinderRadius * 2.0f)), 0.5f + (z / (CylinderRadius * 2.0f))},
				{0.0f, -1.0f, 0.0f}
			});
		}

		const auto sideStart = static_cast<ui32>(vertexList.size());
		for (auto i = 0u; i <= CylinderSegments; ++i)
		{
			const auto u = static_cast<f32>(i) / static_cast<f32>(CylinderSegments);
			const auto angle = u * 2.0f * jazzy::MathUtils::PI;
			const auto x = CylinderRadius * std::cos(angle);
			const auto z = CylinderRadius * std::sin(angle);
			const Vec3 normal = Vec3::normalize({ x, 0.0f, z });

			vertexList.push_back({
				{x, HalfCylinderHeight, z},
				{u, 0.0f},
				normal
			});
			vertexList.push_back({
				{x, -HalfCylinderHeight, z},
				{u, 1.0f},
				normal
			});
		}

		for (auto i = 0u; i < CylinderSegments; ++i)
		{
			const auto topCurrent = topCapStart + i;
			const auto topNext = topCurrent + 1;
			indexList.push_back(topCenterIndex);
			indexList.push_back(topNext);
			indexList.push_back(topCurrent);

			const auto bottomCurrent = bottomCapStart + i;
			const auto bottomNext = bottomCurrent + 1;
			indexList.push_back(bottomCenterIndex);
			indexList.push_back(bottomCurrent);
			indexList.push_back(bottomNext);

			const auto sideTopCurrent = sideStart + i * 2;
			const auto sideBottomCurrent = sideTopCurrent + 1;
			const auto sideTopNext = sideTopCurrent + 2;
			const auto sideBottomNext = sideBottomCurrent + 2;

			indexList.push_back(sideTopCurrent);
			indexList.push_back(sideTopNext);
			indexList.push_back(sideBottomCurrent);
			indexList.push_back(sideTopNext);
			indexList.push_back(sideBottomNext);
			indexList.push_back(sideBottomCurrent);
		}

		vb = m_context.device.createVertexBuffer({ vertexList.data(), static_cast<ui32>(vertexList.size()), sizeof(MeshVertex) });
		ib = m_context.device.createIndexBuffer({ indexList.data(), static_cast<ui32>(indexList.size()) });
	}

	m_vb = vb;
	m_ib = ib;
}

void jazzy::CylinderComponent::setMaterial(const RefPtr<MaterialResource>& material)
{
	m_material = material;
}

jazzy::MaterialResource* jazzy::CylinderComponent::getMaterial()
{
	return m_material.get();
}

jazzy::VertexBuffer& jazzy::CylinderComponent::getVertexBuffer()
{
	return *m_vb;
}

jazzy::IndexBuffer& jazzy::CylinderComponent::getIndexBuffer()
{
	return *m_ib;
}

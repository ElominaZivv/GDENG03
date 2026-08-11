#include <JAZZY/Components/CapsuleComponent.h>
#include <JAZZY/Math/MathUtils.h>
#include "JAZZY/Graphics/GraphicsDevice.h"

#include <cmath>
#include <vector>

namespace
{
	constexpr jazzy::ui32 CapsuleSlices = 32;
	constexpr jazzy::ui32 CapsuleHemisphereStacks = 16;
	constexpr jazzy::f32 CapsuleRadius = 0.5f;
	constexpr jazzy::f32 CapsuleHalfCylinderHeight = 0.5f;
	constexpr jazzy::f32 CapsuleHalfTotalHeight = CapsuleHalfCylinderHeight + CapsuleRadius;
}

jazzy::CapsuleComponent::CapsuleComponent(const ComponentDesc& data) : Component(data)
{
	static RefPtr<VertexBuffer> vb{};
	static RefPtr<IndexBuffer> ib{};

	if (!vb || !ib)
	{
		std::vector<MeshVertex> vertexList{};
		std::vector<ui32> indexList{};
		std::vector<ui32> ringStarts{};

		vertexList.reserve((CapsuleHemisphereStacks * 2 + 2) * (CapsuleSlices + 1));
		indexList.reserve((CapsuleHemisphereStacks * 2 + 1) * CapsuleSlices * 6);
		ringStarts.reserve(CapsuleHemisphereStacks * 2 + 2);

		for (auto stack = 0u; stack <= CapsuleHemisphereStacks; ++stack)
		{
			const auto t = static_cast<f32>(stack) / static_cast<f32>(CapsuleHemisphereStacks);
			const auto angle = t * (jazzy::MathUtils::PI * 0.5f);
			const auto sinAngle = std::sin(angle);
			const auto cosAngle = std::cos(angle);
			const auto ringRadius = CapsuleRadius * sinAngle;
			const auto y = CapsuleHalfCylinderHeight + CapsuleRadius * cosAngle;

			ringStarts.push_back(static_cast<ui32>(vertexList.size()));
			for (auto slice = 0u; slice <= CapsuleSlices; ++slice)
			{
				const auto u = static_cast<f32>(slice) / static_cast<f32>(CapsuleSlices);
				const auto theta = u * 2.0f * jazzy::MathUtils::PI;
				const auto cosTheta = std::cos(theta);
				const auto sinTheta = std::sin(theta);
				const Vec3 normal{ sinAngle * cosTheta, cosAngle, sinAngle * sinTheta };
				const Vec3 position{ ringRadius * cosTheta, y, ringRadius * sinTheta };
				const auto v = (CapsuleHalfTotalHeight - position.y) / (CapsuleHalfTotalHeight * 2.0f);

				vertexList.push_back({ position, {u, v}, normal });
			}
		}

		ringStarts.push_back(static_cast<ui32>(vertexList.size()));
		for (auto slice = 0u; slice <= CapsuleSlices; ++slice)
		{
			const auto u = static_cast<f32>(slice) / static_cast<f32>(CapsuleSlices);
			const auto theta = u * 2.0f * jazzy::MathUtils::PI;
			const auto cosTheta = std::cos(theta);
			const auto sinTheta = std::sin(theta);
			const Vec3 normal{ cosTheta, 0.0f, sinTheta };
			const Vec3 position{ CapsuleRadius * cosTheta, -CapsuleHalfCylinderHeight, CapsuleRadius * sinTheta };
			const auto v = (CapsuleHalfTotalHeight - position.y) / (CapsuleHalfTotalHeight * 2.0f);

			vertexList.push_back({ position, {u, v}, normal });
		}

		for (auto stack = 1u; stack <= CapsuleHemisphereStacks; ++stack)
		{
			const auto t = static_cast<f32>(stack) / static_cast<f32>(CapsuleHemisphereStacks);
			const auto angle = t * (jazzy::MathUtils::PI * 0.5f);
			const auto sinAngle = std::sin(angle);
			const auto cosAngle = std::cos(angle);
			const auto ringRadius = CapsuleRadius * cosAngle;
			const auto y = -CapsuleHalfCylinderHeight - CapsuleRadius * sinAngle;

			ringStarts.push_back(static_cast<ui32>(vertexList.size()));
			for (auto slice = 0u; slice <= CapsuleSlices; ++slice)
			{
				const auto u = static_cast<f32>(slice) / static_cast<f32>(CapsuleSlices);
				const auto theta = u * 2.0f * jazzy::MathUtils::PI;
				const auto cosTheta = std::cos(theta);
				const auto sinTheta = std::sin(theta);
				const Vec3 normal{ cosAngle * cosTheta, -sinAngle, cosAngle * sinTheta };
				const Vec3 position{ ringRadius * cosTheta, y, ringRadius * sinTheta };
				const auto v = (CapsuleHalfTotalHeight - position.y) / (CapsuleHalfTotalHeight * 2.0f);

				vertexList.push_back({ position, {u, v}, normal });
			}
		}

		for (auto ring = 0u; ring + 1 < ringStarts.size(); ++ring)
		{
			const auto topRing = ringStarts[ring];
			const auto bottomRing = ringStarts[ring + 1];

			for (auto slice = 0u; slice < CapsuleSlices; ++slice)
			{
				const auto topLeft = topRing + slice;
				const auto topRight = topLeft + 1;
				const auto bottomLeft = bottomRing + slice;
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

void jazzy::CapsuleComponent::setMaterial(const RefPtr<MaterialResource>& material)
{
	m_material = material;
}

jazzy::MaterialResource* jazzy::CapsuleComponent::getMaterial()
{
	return m_material.get();
}

jazzy::VertexBuffer& jazzy::CapsuleComponent::getVertexBuffer()
{
	return *m_vb;
}

jazzy::IndexBuffer& jazzy::CapsuleComponent::getIndexBuffer()
{
	return *m_ib;
}

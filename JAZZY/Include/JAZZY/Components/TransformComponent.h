#pragma once
#include <JAZZY/Core/Core.h>
#include <JAZZY/Game/Component.h>

#include "JAZZY/Math/Mat4x4.h"
#include "JAZZY/Math/Vec3.h"

namespace jazzy
{
	class TransformComponent final : public Component
	{
		dx3d_typeid(TransformComponent)

	public:
		explicit TransformComponent(const ComponentDesc& data);

		void setPosition(const Vec3& position);
		Vec3 getPosition() const noexcept;

		void setRotation(const Vec3& rotation);
		Vec3 getRotation() const noexcept;
		
		void setScale(const Vec3& scale);
		Vec3 getScale() const noexcept;

		Mat4x4 getAffineWorldMatrix() noexcept;
		Mat4x4 getRigidWorldMatrix() noexcept;

		void updateWorldMatrix() noexcept;
	private:
		void markAsDirty();

	private:
		Vec3 m_position{ 0.0f, 0.0f, 0.0f };
		Vec3 m_rotation{ 0.0f, 0.0f, 0.0f };
		Vec3 m_scale{ 1.0f, 1.0f, 1.0f };

		Mat4x4 m_rigidWorldMatrix{};	// rotation + translation only
		Mat4x4 m_affineWorldMatrix{};	// rotation + translation + scale

		bool m_dirty{};
	};
}

#pragma once
#include <JAZZY/Core/Core.h>
#include <JAZZY/Game/Component.h>

// ReactPhysics3D
#include <reactphysics3d/reactphysics3d.h>
using namespace reactphysics3d;

namespace jazzy
{
	class TransformComponent;

	class RigidBodyComponent final : public Component
	{
		dx3d_typeid(RigidBodyComponent)

	public:
		explicit RigidBodyComponent(const ComponentDesc& data);
		void syncTransformToPhysics() noexcept;
		void syncPhysicsToTransform() noexcept;

	private:
		reactphysics3d::Transform buildPhysicsTransform(const TransformComponent& transform) const noexcept;

	private:
		reactphysics3d::RigidBody* m_rigidBody{};
	};
}

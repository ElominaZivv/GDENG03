#pragma once
#include <JAZZY/Core/Core.h>
#include <JAZZY/Game/Component.h>

// ReactPhysics3D
#include <reactphysics3d/reactphysics3d.h>
using namespace reactphysics3d;

namespace jazzy
{
	class RigidBodyComponent final : public Component
	{
		dx3d_typeid(RigidBodyComponent)

	public:
		explicit RigidBodyComponent(const ComponentDesc& data);

	private:
		reactphysics3d::RigidBody* m_rigidBody;
	};
}

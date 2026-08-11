#pragma once
#include <JAZZY/Core/Common.h>
#include <JAZZY/Core/Base.h>
#include <JAZZY/Core/Identifiable.h>
#include <reactphysics3d/decimal.h>

// ReactPhysics3D
#include <reactphysics3d/reactphysics3d.h>
using namespace reactphysics3d;

namespace jazzy
{
	class Component : public Identifiable
	{
		dx3d_typeid(Component)

	public:
		explicit Component(const ComponentDesc& desc);
		GameObject& getGameObject() noexcept;

	protected:
		GameObject& m_object;
		World& m_world;
		GameContext& m_context;
		reactphysics3d::PhysicsWorld* m_worldPhysics;
	};
}

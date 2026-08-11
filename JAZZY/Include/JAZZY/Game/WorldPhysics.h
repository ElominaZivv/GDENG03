#pragma once
#include <JAZZY/Core/Core.h>
#include <JAZZY/Core/Base.h>
#include <JAZZY/Game/World.h>

// ReactPhysics3D
#include <reactphysics3d/reactphysics3d.h>
using namespace reactphysics3d;

namespace jazzy
{
	class WorldPhysics final : public Base
	{
	public:
		explicit WorldPhysics(const WorldPhysicsDesc& desc);
		void update(f32 deltaTime);

		reactphysics3d::PhysicsWorld* getWorld() const noexcept;

	private:
		RefPtr<reactphysics3d::PhysicsCommon> m_common{};
		reactphysics3d::PhysicsWorld::WorldSettings m_settings;
		reactphysics3d::PhysicsWorld* m_world{};

	};
}


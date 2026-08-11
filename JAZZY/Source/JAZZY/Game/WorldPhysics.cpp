#include <JAZZY/Game/WorldPhysics.h>

jazzy::WorldPhysics::WorldPhysics(const WorldPhysicsDesc& desc) : Base(desc.base)
{
	m_common = std::make_shared<reactphysics3d::PhysicsCommon>();
	reactphysics3d::PhysicsWorld::WorldSettings physicsWorldSettings;
	physicsWorldSettings.defaultVelocitySolverNbIterations = 50;
	physicsWorldSettings.gravity = Vector3(0.0f, -9.81f, 0.0f);
	m_world = m_common->createPhysicsWorld(physicsWorldSettings);
}

void jazzy::WorldPhysics::update(f32 deltaTime)
{
	m_world->update(decimal(deltaTime));
	//DX3DLogInfo("World Gravity {}.", m_world->getGravity().to_string().c_str())
}

reactphysics3d::PhysicsWorld* jazzy::WorldPhysics::getWorld() const noexcept
{
	return m_world;
}

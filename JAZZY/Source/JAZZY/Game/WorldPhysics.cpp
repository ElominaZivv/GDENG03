#include <JAZZY/Game/WorldPhysics.h>
#include <algorithm>

jazzy::WorldPhysics::WorldPhysics(const WorldPhysicsDesc& desc) : Base(desc.base)
{
	m_common = std::make_shared<reactphysics3d::PhysicsCommon>();
	reactphysics3d::PhysicsWorld::WorldSettings physicsWorldSettings;
	physicsWorldSettings.defaultVelocitySolverNbIterations = 18;
	physicsWorldSettings.defaultPositionSolverNbIterations = 8;
	physicsWorldSettings.defaultBounciness = decimal(0.0);
	physicsWorldSettings.defaultFrictionCoefficient = decimal(1.0);
	physicsWorldSettings.gravity = Vector3(0.0f, -9.81f, 0.0f);
	m_world = m_common->createPhysicsWorld(physicsWorldSettings);
}

void jazzy::WorldPhysics::update(f32 deltaTime)
{
	m_accumulator = std::min(m_accumulator + deltaTime, FixedTimeStep * static_cast<f32>(MaxSubsteps));

	i32 numSteps = 0;
	while (m_accumulator >= FixedTimeStep && numSteps < MaxSubsteps)
	{
		m_world->update(decimal(FixedTimeStep));
		m_accumulator -= FixedTimeStep;
		++numSteps;
	}
}

reactphysics3d::PhysicsCommon* jazzy::WorldPhysics::getCommon() const noexcept
{
	return m_common.get();
}

reactphysics3d::PhysicsWorld* jazzy::WorldPhysics::getWorld() const noexcept
{
	return m_world;
}

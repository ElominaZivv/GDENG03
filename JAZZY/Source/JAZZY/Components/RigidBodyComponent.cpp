#include <JAZZY/Components/RigidBodyComponent.h>
#include <JAZZY/Components/TransformComponent.h>
#include <JAZZY/Game/GameObject.h>
#include <JAZZY/Math/MathUtils.h>

jazzy::RigidBodyComponent::RigidBodyComponent(const ComponentDesc& data) : Component(data)
{
	m_rigidBody = data.worldPhysics->createRigidBody(buildPhysicsTransform(getGameObject().getTransform()));
	m_rigidBody->setType(BodyType::DYNAMIC);
	m_rigidBody->enableGravity(true);
	m_rigidBody->setLinearDamping(decimal(0.0));
	m_rigidBody->setAngularDamping(decimal(0.0));
	m_rigidBody->setIsAllowedToSleep(false);
	m_rigidBody->setMass(decimal(1.0));
	syncTransformToPhysics();
}

void jazzy::RigidBodyComponent::syncTransformToPhysics() noexcept
{
	if (!m_rigidBody) return;
	m_rigidBody->setTransform(buildPhysicsTransform(getGameObject().getTransform()));
}

void jazzy::RigidBodyComponent::syncPhysicsToTransform() noexcept
{
	if (!m_rigidBody) return;
	getGameObject().getTransform().syncFromPhysics(m_rigidBody->getTransform());
}

reactphysics3d::Transform jazzy::RigidBodyComponent::buildPhysicsTransform(const TransformComponent& transform) const noexcept
{
	const auto position = transform.getPosition();
	const auto rotation = transform.getRotation() * (MathUtils::PI / 180.0f);

	return reactphysics3d::Transform(
		reactphysics3d::Vector3(position.x, position.y, position.z),
		reactphysics3d::Quaternion::fromEulerAngles(rotation.x, rotation.y, rotation.z)
	);
}

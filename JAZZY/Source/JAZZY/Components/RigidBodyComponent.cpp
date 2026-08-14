#include <JAZZY/Components/RigidBodyComponent.h>
#include <JAZZY/Components/TransformComponent.h>
#include <JAZZY/Game/GameObject.h>
#include <JAZZY/Math/MathUtils.h>

#include <reactphysics3d/reactphysics3d.h>

jazzy::RigidBodyComponent::RigidBodyComponent(const ComponentDesc& data) : Component(data)
{
	m_rigidBody = data.worldPhysics->createRigidBody(buildPhysicsTransform(getGameObject().getTransform()));
	setBodyType(BodyType::DYNAMIC);
	m_rigidBody->setLinearDamping(decimal(0.0));
	m_rigidBody->setAngularDamping(decimal(0.0));
	m_rigidBody->setIsAllowedToSleep(false);
	m_rigidBody->setMass(decimal(1.0));
	syncTransformToPhysics();
}

void jazzy::RigidBodyComponent::setBodyType(reactphysics3d::BodyType type) noexcept
{
	if (!m_rigidBody) return;
	m_rigidBody->setType(type);
	m_rigidBody->enableGravity(type == BodyType::DYNAMIC);
}

void jazzy::RigidBodyComponent::addBoxCollider(const Vec3& halfExtents, const Vec3& localOffset) noexcept
{
	if (!m_rigidBody || !m_physicsCommon) return;

	if (m_collider) {
		m_rigidBody->removeCollider(m_collider);
	}

	m_boxShape = m_physicsCommon->createBoxShape(
		reactphysics3d::Vector3(halfExtents.x, halfExtents.y, halfExtents.z)
	);
	m_collider = m_rigidBody->addCollider(
		m_boxShape,
		reactphysics3d::Transform(
			reactphysics3d::Vector3(localOffset.x, localOffset.y, localOffset.z),
			reactphysics3d::Quaternion::identity()
		)
	);

	if (m_rigidBody->getType() == BodyType::DYNAMIC)
	{
		m_rigidBody->updateMassPropertiesFromColliders();
	}
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

jazzy::Vec3 jazzy::RigidBodyComponent::getColliderDimensions()
{
	if (m_boxShape) {
		return Vec3(m_boxShape->getHalfExtents().x,
			m_boxShape->getHalfExtents().y,
			m_boxShape->getHalfExtents().z);
	}
	else {
		return Vec3(-1.0f, 0.0f, 0.0f);
	}
}

BodyType jazzy::RigidBodyComponent::GetRigidBodyType()
{
	return m_rigidBody->getType();
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

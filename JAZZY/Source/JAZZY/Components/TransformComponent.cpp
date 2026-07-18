#include <ranges>
#include <JAZZY/Components/TransformComponent.h>
#include <JAZZY/Game/World.h>

#include "JAZZY/Game/GameObject.h"

jazzy::TransformComponent::TransformComponent(const ComponentDesc& data): Component(data)
{
	markAsDirty();
}

void jazzy::TransformComponent::setPosition(const Vec3& position)
{
	Vec3 currentPostition = m_position;
	Vec3 newPosition = position;
	Vec3 displacement = newPosition - currentPostition;
	m_position = position;

	markAsDirty();

	GameObject& obj = getGameObject();
	for (auto i : std::views::iota(0u, obj.getChildCount()))
	{
		TransformComponent* childTransform = obj.getChildByIndex(i)->getComponent<jazzy::TransformComponent>();
		Vec3 currentChildPosition = childTransform->getPosition();
		childTransform->setPosition(currentChildPosition + displacement);
	}

}

jazzy::Vec3 jazzy::TransformComponent::getPosition() const noexcept
{
	return m_position;
}

void jazzy::TransformComponent::setRotation(const Vec3& rotation)
{
	Vec3 currentRotation = m_rotation;
	Vec3 newRotation = rotation;
	Vec3 difference = newRotation - currentRotation;

	m_rotation = rotation;
	markAsDirty();

	GameObject& obj = getGameObject();
	for (auto i : std::views::iota(0u, obj.getChildCount()))
	{
		TransformComponent* childTransform = obj.getChildByIndex(i)->getComponent<jazzy::TransformComponent>();
		childTransform->markAsDirty();

		/*
		// Set Position
		Vec3 currentChildPosition = childTransform->getPosition();
		Vec3 newChildPosition = currentChildPosition;
		childTransform->setPosition(newChildPosition);

		// Set Rotation
		Vec3 currentChildRotation = childTransform->getRotation();
		childTransform->setRotation(currentChildRotation + difference);
		*/
	}

}

jazzy::Vec3 jazzy::TransformComponent::getRotation() const noexcept
{
	return m_rotation;
}

void jazzy::TransformComponent::setScale(const Vec3& scale)
{
	Vec3 oldScale = m_scale;

	Vec3 scaleRatio
	{
		scale.x / oldScale.x,
		scale.y / oldScale.y,
		scale.z / oldScale.z
	};

	m_scale = scale;

	GameObject& obj = getGameObject();
	for (auto i : std::views::iota(0u, obj.getChildCount()))
	{
		auto* childTransform =
			obj.getChildByIndex(i)->getComponent<TransformComponent>();

		// Current gap from parent
		Vec3 offset = childTransform->getPosition() - m_position;

		// Scaled gap
		offset.x *= scaleRatio.x;
		offset.y *= scaleRatio.y;
		offset.z *= scaleRatio.z;

		// Child transform stuff
		childTransform->setPosition(m_position + offset);

		Vec3 childScale = childTransform->getScale();

		childScale.x *= scaleRatio.x;
		childScale.y *= scaleRatio.y;
		childScale.z *= scaleRatio.z;

		childTransform->setScale(childScale);
	}
	markAsDirty();
}

jazzy::Vec3 jazzy::TransformComponent::getScale() const noexcept
{
	return m_scale;
}

jazzy::Mat4x4 jazzy::TransformComponent::getAffineWorldMatrix() noexcept
{
	return m_affineWorldMatrix;
}

jazzy::Mat4x4 jazzy::TransformComponent::getRigidWorldMatrix() noexcept
{

	return m_rigidWorldMatrix;
}

void jazzy::TransformComponent::updateWorldMatrix() noexcept
{
	if (!m_dirty) return;

	m_dirty = false;

	m_rigidWorldMatrix =
		Mat4x4::rotateX(m_rotation.x) *
		Mat4x4::rotateY(m_rotation.y) *
		Mat4x4::rotateZ(m_rotation.z) *
		Mat4x4::translation(m_position);

	m_affineWorldMatrix =
		Mat4x4::scale(m_scale) *
		m_rigidWorldMatrix;

	auto& obj = getGameObject();
	auto* parent = obj.getParent();

	if (parent)
	{
		DX3DLogInfo("TESTING");
		TransformComponent* parent_transform = parent->getComponent<jazzy::TransformComponent>();
		Mat4x4 parentMat = parent_transform->getRigidWorldMatrix();
		m_affineWorldMatrix =
			parentMat *
			m_rigidWorldMatrix;
	}
}

void jazzy::TransformComponent::markAsDirty()
{
	if (m_dirty) return;
	m_dirty = true;
	m_world.addDirtyTransformInternal(*this);
}


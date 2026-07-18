#include <iostream>
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
	Vec3 currentPosition = m_position;
	Vec3 newPosition = position;
	Vec3 displacement = position - m_position;

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
	Vec3 differenceRadians = (difference * (MathUtils::PI / 180.0f));

	m_rotation = rotation;
	markAsDirty();

	GameObject& obj = getGameObject();
	for (auto i : std::views::iota(0u, obj.getChildCount()))
	{
		TransformComponent* childTransform = obj.getChildByIndex(i)->getComponent<jazzy::TransformComponent>();

		Vec3 currentChildPosition = childTransform->getPosition();
		Vec3 newChildPosition{};

		// Get displacement of child from parent
		Vec3 displacement = childTransform->getPosition() - m_position;
		// Rotate displacement
		Mat4x4 temporary = Mat4x4::identity();
		temporary = Mat4x4::translation(displacement);
		temporary =
			temporary *
			Mat4x4::rotateX(differenceRadians.x) *
			Mat4x4::rotateY(differenceRadians.y) *
			Mat4x4::rotateZ(differenceRadians.z);
		// Translate to parent
		temporary = temporary * Mat4x4::translation(m_position);
		Vec3 newPosition({ temporary.row(3).x, temporary.row(3).y, temporary.row(3).z});
		childTransform->setPosition(newPosition);

		// Set Rotation
		Vec3 currentChildRotation = childTransform->getRotation();
		childTransform->setRotation(currentChildRotation + difference);
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
	markAsDirty();

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

	Vec3 radians = (m_rotation * (MathUtils::PI / 180.0f));

	m_rigidWorldMatrix =
		Mat4x4::rotateX(radians.x) *
		Mat4x4::rotateY(radians.y) *
		Mat4x4::rotateZ(radians.z) *
		Mat4x4::translation(m_position);

	m_affineWorldMatrix =
		Mat4x4::scale(m_scale) *
		m_rigidWorldMatrix;
}

void jazzy::TransformComponent::markAsDirty()
{
	if (m_dirty) return;
	m_dirty = true;
	m_world.addDirtyTransformInternal(*this);
}


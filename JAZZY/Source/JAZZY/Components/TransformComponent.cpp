#include <JAZZY/Components/TransformComponent.h>

void jazzy::TransformComponent::setPosition(const Vec3& position)
{
	m_position = position;
	markAsDirty();
}

jazzy::Vec3 jazzy::TransformComponent::getPosition() const noexcept
{
	return m_position;
}

void jazzy::TransformComponent::setRotation(const Vec3& rotation)
{
	m_rotation = rotation;
	markAsDirty();
}

jazzy::Vec3 jazzy::TransformComponent::getRotation() const noexcept
{
	return m_rotation;
}

void jazzy::TransformComponent::setScale(const Vec3& scale)
{
	m_scale = scale;
	markAsDirty();
}

jazzy::Vec3 jazzy::TransformComponent::getScale() const noexcept
{
	return m_scale;
}

void jazzy::TransformComponent::markAsDirty()
{
	if (m_dirty) return;
	m_dirty = true;
	//m_world.addDirtyTransformInternal(*this);
}

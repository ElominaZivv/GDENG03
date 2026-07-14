#include <JAZZY/Components/TransformComponent.h>
#include <JAZZY/Game/World.h>

jazzy::TransformComponent::TransformComponent(const ComponentDesc& data): Component(data)
{
}

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
		Mat4x4::rotateX(m_rotation.x) *
		Mat4x4::rotateX(m_rotation.x) *
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

#include <JAZZY/Game/Component.h>

jazzy::Component::Component(const ComponentDesc& desc) : Identifiable(desc.base), m_object(desc.object), m_world(desc.world), m_context(desc.context), m_physicsCommon(desc.physicsCommon), m_worldPhysics(desc.worldPhysics)
{
}

jazzy::GameObject& jazzy::Component::getGameObject() noexcept
{
	return m_object;
}

bool jazzy::Component::getHidden()
{
	return hidden;
}

void jazzy::Component::setHidden(bool hide)
{
	hidden = hide;
}

bool jazzy::Component::getHiddenByParent()
{
	return hidden;
}

void jazzy::Component::setHiddenByParent(bool hide)
{
	hiddenByParent = hide;
}

bool  jazzy::Component::isVisible()
{
	return !hidden && !hiddenByParent;
}
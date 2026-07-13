#include <JAZZY/Game/Component.h>
//#include <JAZZY/Game/World.h>	// Why is this here?

jazzy::Component::Component(const ComponentDesc& desc) : Identifiable(desc.base), m_object(desc.object), m_world(desc.world), m_context(desc.context)
{
}

jazzy::GameObject& jazzy::Component::getGameObject() noexcept
{
	return m_object;
}

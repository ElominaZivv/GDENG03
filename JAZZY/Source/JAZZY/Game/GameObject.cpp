#include <JAZZY/Game/GameObject.h>

jazzy::GameObject::GameObject(const GameObjectDesc& desc): Identifiable(desc.base), m_gameContext(desc.gameContext)
{
}

jazzy::InputSystem& jazzy::GameObject::getInputSystem() noexcept
{
	return m_gameContext.input;
}

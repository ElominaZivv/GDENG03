#include <iostream>
#include <ranges>
#include <JAZZY/Game/GameObject.h>
#include <JAZZY/Game/World.h>

jazzy::GameObject::GameObject(const GameObjectDesc& desc): Identifiable(desc.base), m_world(desc.world), m_gameContext(desc.gameContext), m_name(desc.name)
{
	m_transform = createOrGetComponent<TransformComponent>();
}

jazzy::TransformComponent& jazzy::GameObject::getTransform() noexcept
{
	return *m_transform;
}

jazzy::InputSystem& jazzy::GameObject::getInputSystem() noexcept
{
	return m_gameContext.input;
}

void jazzy::GameObject::setParent(GameObject* obj)
{
	if (!obj) return;
	obj->addChild(this);
	m_parent = obj;
}

jazzy::GameObject* jazzy::GameObject::getParent()
{
	if (!m_parent) return{};
	return static_cast<GameObject*>(m_parent);
}

void jazzy::GameObject::addChild(GameObject* obj)
{
	if (!obj) return;
	m_children.push_back(static_cast<GameObject*>(obj));
}

void jazzy::GameObject::removeChildByIndex(ui32 index)
{
	if (m_children.empty()) return;
	if (index >= m_children.size()) return;
	std::swap(m_children[index], m_children.back());
	m_children.pop_back();
}

void jazzy::GameObject::removeChildByName(std::string name)
{
	if (m_children.empty()) return;

	i32 indexToRemove = -1;
	for (auto i : std::views::iota(0u, m_children.size()))
	{
		if (m_children[i]->m_name == name) indexToRemove = i;
	}

	if (indexToRemove < 0) return;
	removeChildByIndex(indexToRemove);
}

jazzy::GameObject* jazzy::GameObject::getChildByIndex(ui32 index)
{
	if (m_children.empty()) return{};
	if (index >= m_children.size()) return {};
	return m_children[index];
}

jazzy::ui32 jazzy::GameObject::getChildCount()
{
	return m_children.size();
}

jazzy::Component* jazzy::GameObject::createComponentInternal(UniquePtr<Component>& component)
{
	if (!component) return{};
	auto typeId = component->getTypeId();
	auto ptr = component.get();
	if (m_components.find(typeId) != m_components.end()) return {};
	m_components.emplace(typeId, std::move(component));
	m_world.addComponentInternal(*ptr);
	return ptr;
}

jazzy::Component* jazzy::GameObject::getComponentInternal(size_t id)
{
	auto it = m_components.find(id);
	if (it == m_components.end()) return{};
	return it->second.get();
}

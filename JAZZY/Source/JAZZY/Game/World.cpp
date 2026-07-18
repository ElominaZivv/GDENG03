#include <ranges>
#include <JAZZY/Game/World.h>
#include <JAZZY/Game/GameObject.h>

jazzy::World::World(const WorldDesc& desc) : Base(desc.base), m_gameContext(desc.gameContext)
{
}

void jazzy::World::update(f32 deltaTime)
{
	// GameObject Creation Event
	if (m_events.size())
	{
		std::swap(m_events, m_eventsSwapBuffer);
		std::swap(m_pendingObjects, m_pendingObjectSwapBuffer);

		for (auto& e : m_eventsSwapBuffer)
		{
			auto objTypeId = e.object->getTypeId();
			auto pendingObjIndex = e.pendingObjectIndex;

			if (e.eventType == EventType::Create)
			{
				auto& obj = m_pendingObjectSwapBuffer[pendingObjIndex];
				auto ptr = obj.get();
				// Gets from the unordered map the type of object (objTypeId) then 
				// pushes back that object into a vector of that typeId
				m_objects[objTypeId].push_back(std::move(obj));
				ptr->onCreate();
			}
		}

		m_pendingObjectSwapBuffer.clear();
		m_eventsSwapBuffer.clear();
	}

	// Update of Individual Transform Components
	for (auto& comp : m_dirtyTransforms)
	{
		comp->updateWorldMatrix();
	}
	m_dirtyTransforms.clear();
}

void jazzy::World::SetSelectedObject(ui32 index)
{
	selectedObjIndex = index;
}

jazzy::ui32 jazzy::World::GetSelectedIndex()
{
	return selectedObjIndex;
}

jazzy::GameObject* jazzy::World::createGameObjectInternal(UniquePtr<GameObject>& object)
{
	if (!object) return{};

	auto ptr = object.get();
	auto index = m_pendingObjects.size();
	m_pendingObjects.push_back(std::move(object));
	m_events.push_back({ ptr, index, EventType::Create });

	return ptr;
}

void jazzy::World::addComponentInternal(Component& component)
{
	auto typeId = component.getTypeId();
	m_components[typeId].push_back(&component);
}

void jazzy::World::addDirtyTransformInternal(TransformComponent& component)
{
	m_dirtyTransforms.push_back(&component);
}

jazzy::Component* const* jazzy::World::getComponentsInternal(size_t typeId, ui32* numComponents) const noexcept
{
	auto it = m_components.find(typeId);
	if (it != m_components.end())
	{
		// This modifies the numComponents =0u in WorldRenderer/GraphicsEngine into the number of instances of components in the world
		*numComponents = static_cast<ui32>(it->second.size());
		return it->second.data();
	}
	*numComponents = 0u;
	return {};
}

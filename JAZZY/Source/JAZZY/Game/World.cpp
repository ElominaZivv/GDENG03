#include <JAZZY/Game/World.h>
#include <JAZZY/Game/GameObject.h>

jazzy::World::World(const WorldDesc& desc) : Base(desc.base), m_gameContext(desc.gameContext)
{
}

void jazzy::World::update(f32 deltaTime)
{
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
				// Gets from the unordered map the type of object (objTypeId) then pushes back that object into a vector of that typeId
				m_objects[objTypeId].push_back(std::move(obj));
				ptr->onCreate();
			}
		}

		m_pendingObjectSwapBuffer.clear();
		m_eventsSwapBuffer.clear();
	}
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

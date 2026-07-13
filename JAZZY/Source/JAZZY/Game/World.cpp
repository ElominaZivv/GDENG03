#include <JAZZY/Game/World.h>

jazzy::World::World(const WorldDesc& desc) : Base(desc.base), m_gameContext(desc.gameContext)
{
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

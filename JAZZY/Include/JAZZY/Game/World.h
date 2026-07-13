#pragma once
#include <JAZZY/Core/Common.h>
#include <JAZZY/Core/Base.h>
#include <JAZZY/Core/Identifiable.h>
#include <unordered_map>
#include <vector>

namespace jazzy
{
	class World final : public Base
	{
	public:
		explicit World(const WorldDesc& desc);

		template<typename T>
		T* createGameObject() requires isRegistered<GameObject, T>
		{
			UniquePtr<GameObject> e = std::make_unique<T>(GameObjectDesc{
				{m_logger},
				m_gameContext,
				*this
				});
			return static_cast<T*>(createGameObjectInternal(e));
		}

		void update(f32 deltaTime);
	private:
		enum class EventType
		{
			Create = 0
		};
		struct GameObjectEvents
		{
			GameObject* object{};
			size_t pendingObjectIndex{};
			EventType eventType{};
		};

	private:
		GameObject* createGameObjectInternal(UniquePtr<GameObject>& object);

	private:
		GameContext m_gameContext;

		std::unordered_map<size_t, std::vector<UniquePtr<GameObject>>> m_objects{};

		std::vector<UniquePtr<GameObject>> m_pendingObjects;
		std::vector<UniquePtr<GameObject>> m_pendingObjectSwapBuffer;
		std::vector<GameObjectEvents> m_events{};
		std::vector<GameObjectEvents> m_eventsSwapBuffer{};
	
		friend class GameObject;
	};
}

#pragma once
#include <JAZZY/Core/Common.h>
#include <JAZZY/Core/Base.h>
#include <JAZZY/Core/Identifiable.h>
#include <JAZZY/Game/Component.h>
#include <unordered_map>
#include <vector>

#include "JAZZY/Components/TransformComponent.h"

namespace jazzy
{
	class World final : public Base
	{
	public:
		explicit World(const WorldDesc& desc);

		template<typename T>
		T* createGameObject(const char* name = "GameObject")
			requires isRegistered<GameObject, T>
		{
			UniquePtr<GameObject> e = std::make_unique<T>(
				GameObjectDesc{
					{m_logger},
					m_gameContext,
					*this,
					name
				});

			return static_cast<T*>(createGameObjectInternal(e));
		}

		template<typename T>
		T* const* getComponents(ui32& numComponents) const noexcept
		{
			return reinterpret_cast<T* const*>(getComponentsInternal(T::GetTypeId(), &numComponents));
		}

		void update(f32 deltaTime);

		void SetSelectedObject(ui32 index);
		ui32 GetSelectedIndex();

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
		void addComponentInternal(Component& component);
		void addDirtyTransformInternal(TransformComponent& component);

		Component* const* getComponentsInternal(size_t typeId, ui32* numComponents) const noexcept;

	private:
		GameContext m_gameContext;

		// size_t is the typeId of the GameObject which maps to a list of GameObjects of the same typeId?
		std::unordered_map<size_t, std::vector<UniquePtr<GameObject>>> m_objects{};
		std::unordered_map<size_t, std::vector<Component*>> m_components{};

		std::vector<TransformComponent*> m_dirtyTransforms{};

		std::vector<UniquePtr<GameObject>> m_pendingObjects;
		std::vector<UniquePtr<GameObject>> m_pendingObjectSwapBuffer;
		std::vector<GameObjectEvents> m_events{};
		std::vector<GameObjectEvents> m_eventsSwapBuffer{};
	
		friend class GameObject;
		friend class TransformComponent;

		ui32 selectedObjIndex = 0;
	};
}

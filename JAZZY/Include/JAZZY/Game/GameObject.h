#pragma once
#include <JAZZY/Core/Common.h>
#include <JAZZY/Core/Identifiable.h>
#include <JAZZY/Game/Component.h>

#include <unordered_map>

#include "JAZZY/Components/TransformComponent.h"

namespace jazzy
{
	class GameObject : public Identifiable
	{
		// This basically adds a bunch of methods to GameObject from the macro defined in Identifiable. Kewlness
		dx3d_typeid(GameObject)

	public:
		explicit GameObject(const GameObjectDesc& desc);

		template<typename T>
		T* createOrGetComponent() requires isRegistered<Component, T>
		{
			auto c = getComponent<T>();
			if (c) return c;
			UniquePtr<Component> cp = std::make_unique<T>(ComponentDesc{
				{m_logger},
				*this, 
				m_world,
				m_gameContext
				});
			return static_cast<T*>(createComponentInternal(cp));
		}

		template<typename T>
		T* getComponent() requires isRegistered<Component, T>
		{
			return static_cast<T*>(getComponentInternal(T::GetTypeId()));
		}

		TransformComponent& getTransform() noexcept;
		InputSystem& getInputSystem() noexcept;

		void setParent(GameObject* obj);
		GameObject* getParent();
		void addChild(GameObject* obj);
		GameObject* getChild(ui32 index);
	
	protected:
		virtual void onCreate() {}
		virtual void onUpdate(f32 deltaTime) {}

	private:
		Component* createComponentInternal(UniquePtr<Component>& component);
		Component* getComponentInternal(size_t id);

	private:
		std::unordered_map<size_t, UniquePtr<Component>> m_components{};
		TransformComponent* m_transform{};
		GameContext m_gameContext;
		World& m_world;

		GameObject* m_parent;
		std::vector<GameObject*> m_children;

		friend class World;
	};
}


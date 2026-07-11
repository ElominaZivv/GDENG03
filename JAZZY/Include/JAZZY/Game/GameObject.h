#pragma once
#include <JAZZY/Core/Common.h>
#include <JAZZY/Core/Identifiable.h>

#include <unordered_map>

namespace jazzy
{
	class GameObject : public Identifiable
	{
		// This basically adds a bunch of methods to GameObject from the macro defined in Identifiable. Kewlness
		dx3d_typeid(GameObject)

	public:
		explicit GameObject(const GameObjectDesc& desc);

		InputSystem& getInputSystem() noexcept;

	protected:
		virtual void onCreate() {}
		virtual void onUpdate(f32 deltaTime) {}

	private:
		GameContext m_gameContext;
	};
}


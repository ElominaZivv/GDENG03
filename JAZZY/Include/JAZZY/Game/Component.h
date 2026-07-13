#pragma once
#include <JAZZY/Core/Common.h>
#include <JAZZY/Core/Base.h>
#include <JAZZY/Core/Identifiable.h>

namespace jazzy
{
	class Component : public Identifiable
	{
		dx3d_typeid(Component)

	public:
		explicit Component(const ComponentDesc& desc);
		GameObject& getGameObject() noexcept;

	protected:
		GameObject& m_object;
		World& m_world;
		GameContext& m_context;
	};
}

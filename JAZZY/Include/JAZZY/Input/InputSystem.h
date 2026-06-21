#pragma once
#include <JAZZY/Core/Base.h>
#include <array>

namespace jazzy
{
	class InputSystem final : public Base
	{
	public:
		InputSystem(const InputSystemDesc& desc);
		~InputSystem();

		void update();

		bool isKeyDown(KeyCode key) const;
		bool isKeyPressed(KeyCode key) const;
		bool isKeyReleased(KeyCode key) const;
	private:
		short getInternalKeyCode(const KeyCode& key);

		// Creates an array of booleans with the same number of booleans as the KeyCode
		std::array<bool, static_cast<std::size_t>(KeyCode::Count)>m_currentKeys{};
		std::array<bool, static_cast<std::size_t>(KeyCode::Count)>m_previousKeys{};


	};
}


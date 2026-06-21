#pragma once
#include <JAZZY/Core/Base.h>
#include <JAZZY/Math/Vec2.h>
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

		Vec2 getMousePos() const noexcept;
		Vec2 getMouseDelta() const noexcept;

		void setCursorVisible(bool visible);
		void setCursorLockArea(const Rect& rect);
		void setCursorLocked(bool locked);
	private:
		void centerCursor();
	private:
		short getInternalKeyCode(const KeyCode& key);

		// Creates an array of booleans with the same number of booleans as the KeyCode
		std::array<bool, static_cast<std::size_t>(KeyCode::Count)>m_currentKeys{};
		std::array<bool, static_cast<std::size_t>(KeyCode::Count)>m_previousKeys{};

		Vec2 m_mousePos{};
		Vec2 m_previousMousePos{};
		Vec2 m_mouseDelta{};

		bool m_cursorVisible{ true };
		bool m_cursorLocked{ false };
		Rect m_lockArea{};
	};
}


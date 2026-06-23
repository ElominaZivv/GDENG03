#include <JAZZY/Input/InputSystem.h>
#include <windows.h>
#include <ranges>

jazzy::InputSystem::InputSystem(const InputSystemDesc& desc): Base(desc.base)
{
}

jazzy::InputSystem::~InputSystem()
{
}

void jazzy::InputSystem::update()
{
	// Keyboard
	m_previousKeys = m_currentKeys;

	for (auto i : std::views::iota(0u, static_cast<std::size_t>(KeyCode::Count)))
	{
		// From the iota, i, return the short virtual key code to be passed into getAsyncKeyState
		const auto vk = getInternalKeyCode(static_cast<KeyCode>(i));
		// & 0x8000 checks if the keystate is pressed or not
		// GetAsyncKeyState returns the 16bit integer of the key where the 15th bit determines if the key is 1 pressed or 0 unpressed
		m_currentKeys[i] = (GetAsyncKeyState(vk) & 0x8000) != 0;
	}

	// Mouse
	m_previousMousePos = m_mousePos;

	POINT point{};
	GetCursorPos(&point);

	m_mousePos.x = static_cast<f32>(point.x);
	m_mousePos.y = static_cast<f32>(point.y);
	m_mouseDelta.x = m_mousePos.x - m_previousMousePos.x;
	m_mouseDelta.y = m_mousePos.y - m_previousMousePos.y;
}

bool jazzy::InputSystem::isKeyDown(KeyCode key) const
{
	return m_currentKeys[static_cast<std::size_t>(key)];
}

bool jazzy::InputSystem::isKeyPressed(KeyCode key) const
{
	return m_currentKeys[static_cast<std::size_t>(key)] && !m_previousKeys[static_cast<std::size_t>(key)];
}

bool jazzy::InputSystem::isKeyReleased(KeyCode key) const
{
	return !m_currentKeys[static_cast<std::size_t>(key)] && m_previousKeys[static_cast<std::size_t>(key)];
}

jazzy::Vec2 jazzy::InputSystem::getMousePos() const noexcept
{
	return m_mousePos;
}

jazzy::Vec2 jazzy::InputSystem::getMouseDelta() const noexcept
{
	return m_mouseDelta;
}

void jazzy::InputSystem::setCursorVisible(bool visible)
{
	m_cursorVisible = visible;

	ShowCursor(visible);
}

void jazzy::InputSystem::setCursorLockArea(const Rect& rect)
{
	m_lockArea = rect;
}

void jazzy::InputSystem::setCursorLocked(bool locked)
{
	m_cursorLocked = locked;
	if (locked) centerCursor();
}

void jazzy::InputSystem::centerCursor()
{
	const auto centerX = m_lockArea.left + (m_lockArea.width / 2);
	const auto centerY = m_lockArea.top + (m_lockArea.height / 2);

	SetCursorPos(centerX, centerY);

	m_mousePos.x = static_cast<f32>(centerX);
	m_mousePos.y = static_cast<f32>(centerY);
}

short jazzy::InputSystem::getInternalKeyCode(const KeyCode& key)
{
	const auto value = static_cast<int>(key);
	// A -> Z
	if (key >= KeyCode::A && key <= KeyCode::Z) return 'A' + (value - static_cast<int>(KeyCode::A));
	// 0 -> 9
	if (key >= KeyCode::Num1 && key <= KeyCode::Num9) return '0' + (value - static_cast<int>(KeyCode::Num9));

	switch (key)
	{
	case KeyCode::Shift: return VK_SHIFT;
	case KeyCode::Escape: return VK_ESCAPE;
	case KeyCode::Space: return VK_SPACE;
	case KeyCode::Enter: return VK_RETURN;
	case KeyCode::Backspace: return VK_BACK;
	case KeyCode::Delete: return VK_DELETE;
	case KeyCode::MouseLeft: return VK_LBUTTON;
	case KeyCode::MouseMiddle: return VK_MBUTTON;
	case KeyCode::MouseRight: return VK_RBUTTON;
	case KeyCode::Up: return VK_UP;
	case KeyCode::Down: return VK_DOWN;
	case KeyCode::Left: return VK_LEFT;
	case KeyCode::Right: return VK_RIGHT;
	default: return 0;
	}
}

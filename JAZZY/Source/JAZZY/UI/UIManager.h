#pragma once

#include <iostream>
#include <vector>
#include <JAZZY/Core/Core.h>
#include <JAZZY/Core/Base.h>
#include <JAZZY/IMGUI/imgui.h>
#include <JAZZY/IMGUI/imgui_impl_dx11.h>
#include <JAZZY/IMGUI/imgui_impl_win32.h>
#include <JAZZY/Window/Window.h>
#include <Windows.h>

#include <JAZZY/UI/Screens/Screens.h>
#include <JAZZY/UI/Screens/MenuScreen.h>
#include <JAZZY/UI/Screens/UndoScreen.h>
#include <JAZZY/UI/Screens/DebugConsole.h>

namespace jazzy {

	class UIManager
	{
	public:
		explicit UIManager(const UIManagerDesc& desc);
		void draw();
		void destroy();

		DebugConsole* GetDebugConsoleScreen();

	private:

		std::vector<UniquePtr<Screens>> m_screens;

		enum UI_SCREENS {
			UI_MENU = 0,
			UI_INSPECTOR,
			UI_HIERARCHY,
			UI_UNDO,
			UI_DEBUG
		};

	};
}


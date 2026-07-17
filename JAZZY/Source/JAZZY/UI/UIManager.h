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

namespace jazzy {

	class UIManager
	{
	public:
		void initialize(HWND hwnd, GraphicsEngine& graphicsEngine, World& world);
		void draw();
		void destroy();
	private:

		std::vector<UniquePtr<Screens>> m_screens;
		GraphicsDevice* m_graphicsDevice;
	};
}


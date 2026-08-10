#include <JAZZY/UI/UIManager.h>
#include <JAZZY/Graphics/DeviceContext.h>
#include <JAZZY/Graphics/GraphicsDevice.h>
#include <JAZZY/UI/Screens/MenuScreen.h>
#include <JAZZY/UI/Screens/InspectorScreen.h>
#include <JAZZY/UI/Screens/HierarchyScreen.h>
#include <JAZZY/Graphics/SwapChain.h>
#include <JAZZY/Game/World.h>
#include <iostream>

#include "JAZZY/Game/Display.h"

jazzy::UIManager::UIManager(const UIManagerDesc& desc)
{
    auto& graphicsDevice = desc.graphicsDevice;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    auto hwnd = static_cast<HWND>(desc.display.getHWND());
	ImGui_ImplWin32_Init(hwnd);

    bool check = ImGui_ImplDX11_Init(
        graphicsDevice.getDevice(),
        graphicsDevice.getContext()
    );

    m_screens.push_back(UniquePtr<MenuScreen>(new MenuScreen(desc.world)));
    m_screens.push_back(UniquePtr<InspectorScreen>(new InspectorScreen(desc.world)));
    m_screens.push_back(UniquePtr<HierarchyScreen>(new HierarchyScreen(desc.world)));
    m_screens.push_back(UniquePtr<UndoScreen>(new UndoScreen(desc.world)));
}

void jazzy::UIManager::draw()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    
    for (auto& screen : m_screens)
    {
        if (screen)
            screen->draw();
    }

    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void jazzy::UIManager::destroy()
{
    m_screens.clear();

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

#include <JAZZY/UI/UIManager.h>
#include <JAZZY/Graphics/GraphicsEngine.h>
#include <JAZZY/Graphics/DeviceContext.h>
#include <JAZZY/Graphics/GraphicsDevice.h>
#include <JAZZY/UI/Screens/MenuScreen.h>
#include <JAZZY/UI/Screens/InspectorScreen.h>
#include <JAZZY/UI/Screens/HierarchyScreen.h>
#include <JAZZY/Graphics/SwapChain.h>
#include <JAZZY/Game/World.h>
#include <iostream>
void jazzy::UIManager::initialize(HWND hwnd, GraphicsEngine& graphicsEngine, World& world)
{
    m_graphicsDevice = &graphicsEngine.getGraphicsDevice();
    auto& graphicsDevice = graphicsEngine.getGraphicsDevice();
    auto& deviceContext = graphicsEngine.getDeviceContext();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);

    ImGui_ImplDX11_Init(
        graphicsDevice.getDevice(),
        graphicsDevice.getContext()
    );

    m_screens.push_back(UniquePtr<MenuScreen>(new MenuScreen(world)));
    m_screens.push_back(UniquePtr<InspectorScreen>(new InspectorScreen(world)));
    m_screens.push_back(UniquePtr<HierarchyScreen>(new HierarchyScreen(world)));
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

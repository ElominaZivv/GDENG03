#include <JAZZY/UI/Screens/HierarchyScreen.h>
#include <JAZZY/IMGUI/imgui.h>
#include <JAZZY/IMGUI/imgui_internal.h>
#include <JAZZY/Components/CubeComponent.h>
#include <JAZZY/Game/World.h>
#include <JAZZY/Game/GameObject.h>
#include <iostream>

jazzy::HierarchyScreen::HierarchyScreen(World & world)
    : Screens("Menu"), m_world(world)
{
}

void jazzy::HierarchyScreen::draw()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoNavFocus;

    // transform temp || this should be a reference to the specific transform being changed
    static float position[3] = { 0.0f, 1.0f, 0.0f };
    static float scale[3] = { 1.0f, 1.0f, 1.0f };
    static float rotation[3] = { 0.0f, 0.0f, 0.0f };

    if (ImGui::BeginViewportSideBar("Hierarchy", viewport, ImGuiDir_Left, 150.0f, windowFlags)) {

        ImGui::Text("Game Objects");
        // put hierarchy things here...

        ImGui::End();
    }
}
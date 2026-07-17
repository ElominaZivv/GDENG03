#include <JAZZY/UI/Screens/InspectorScreen.h>
#include <JAZZY/IMGUI/imgui.h>
#include <JAZZY/IMGUI/imgui_internal.h>
#include <JAZZY/Components/CubeComponent.h>
#include <JAZZY/Game/World.h>
#include <JAZZY/Game/GameObject.h>
#include <iostream>

jazzy::InspectorScreen::InspectorScreen(World& world)
    : Screens("Menu"), m_world(world)
{
}

void jazzy::InspectorScreen::draw()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoSavedSettings |
                                   ImGuiWindowFlags_NoNavFocus;

    // scale temp || this should be a reference to the transform being changed
    static float scale[3] = { 1.0f, 1.0f, 1.0f };

    if (ImGui::BeginViewportSideBar("Inspector", viewport, ImGuiDir_Right, 200.0f, windowFlags)) {

        if (ImGui::InputFloat3("Scale", scale, "%.2f")) {

        }

        ImGui::End();
    }
}
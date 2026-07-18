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

    // transform temp || this should be a reference to the specific transform being changed
    auto numComp = 0u;
    auto cubes = m_world.getComponents<CubeComponent>(numComp);


    auto plane = cubes[0];
    auto& transform = plane->getGameObject().getTransform();

    static float position[3] = {0.0f, 1.0f, 0.0f};
    static float scale[3] = { 1.0f, 1.0f, 1.0f };
    static float rotation[3] = { 0.0f, 0.0f, 0.0f };

    if (ImGui::BeginViewportSideBar("Inspector", viewport, ImGuiDir_Right, 200.0f, windowFlags)) {

        if (ImGui::InputFloat3("Position", position, "%.2f")) {
            transform.setPosition({ position[0], position[1], position[2] });
        }

        if (ImGui::InputFloat3("Scale", scale, "%.2f")) {

        }

        if (ImGui::InputFloat3("Rotation", rotation, "%.2f")) {

        }

        ImGui::End();
    }
}
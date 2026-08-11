#include "JAZZY/UI/Screens/UndoScreen.h"
#include <JAZZY/IMGUI/imgui.h>
#include <JAZZY/Game/World.h>

jazzy::UndoScreen::UndoScreen(World& world)
    : Screens("Undo"), m_world(world)
{
}

void jazzy::UndoScreen::draw()
{
    ImGui::SetNextWindowSize(ImVec2(160.0f, 60.f));

    if (ImGui::Begin("Action", nullptr, ImGuiWindowFlags_NoResize))
    {
        if (ImGui::Button("Undo", ImVec2(70.0f, 25.0f)))
        {
            m_world.getRecordHolder().undo();
        }

        ImGui::SameLine();

        if (ImGui::Button("Redo", ImVec2(70.0f, 25.0f)))
        {
            m_world.getRecordHolder().redo();
        }
    }

    ImGui::End();
}

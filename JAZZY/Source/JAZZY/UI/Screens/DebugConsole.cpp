#include <JAZZY/UI/Screens/DebugConsole.h>
#include <JAZZY/IMGUI/imgui.h>
#include <JAZZY/IMGUI/imgui_internal.h>

#include <vector>

#include <iostream>

jazzy::DebugConsole::DebugConsole() : Screens("Debug Console") {
    logMessages.clear();
}

void jazzy::DebugConsole::draw()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoFocusOnAppearing;

    if (ImGui::BeginViewportSideBar("Debug Console", viewport, ImGuiDir_Down, 150.0f, windowFlags)) {

        ImGui::BeginChild("AllMessages", ImVec2(0, 100), false, ImGuiWindowFlags_HorizontalScrollbar);

        ImGuiListClipper clipper;
        clipper.Begin(logMessages.size());

        while (clipper.Step()) {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
                ImGui::Text(logMessages[i].c_str());
            }
        }

        ImGui::EndChild();

        ImGui::End();
    }
}

void jazzy::DebugConsole::AddToDebugLog(std::string newMsg)
{
    logMessages.push_back(newMsg);
}

void jazzy::DebugConsole::ClearDebugLog()
{
    logMessages.clear();
}

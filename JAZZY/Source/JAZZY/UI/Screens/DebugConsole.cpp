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

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoFocusOnAppearing;

    float height = isOpen ? expandedHeight : collapsedHeight;

    if (ImGui::BeginViewportSideBar("Debug Console", viewport, ImGuiDir_Down, height, windowFlags)) {

        // header / toggle visibility
        if (ImGui::Button(isOpen ? "<" : ">", ImVec2(25.0f, 0))) {
            isOpen = !isOpen;
            autoScroll = true;
        } ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.9f, 0.5f, 0.2f, 1.0f), "Debug Console"); ImGui::SameLine();
        if (ImGui::Button("Clear Logs", ImVec2(100.0f, 0))) {
            ClearDebugLog();
        } 
        ImGui::Separator();

        // renders log messages if full
        if (isOpen) {
            ImGui::BeginChild("AllMessages", ImVec2(0, 130), false, ImGuiWindowFlags_HorizontalScrollbar);

            // this is for rendering optimization stuff
            ImGuiListClipper clipper;
            clipper.Begin(logMessages.size());

            while (clipper.Step()) {
                for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
                    ImGui::Text(logMessages[i].c_str());
                }
            }

            if (autoScroll) {
                ImGui::SetScrollHereY(1.0f);
                autoScroll = false;
            }

            ImGui::EndChild();
        }

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

jazzy::DebugConsole::~DebugConsole()
{
    ClearDebugLog();
}

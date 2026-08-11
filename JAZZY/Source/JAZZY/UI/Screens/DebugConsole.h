#pragma once

#include <JAZZY/Core/Core.h>
#include <JAZZY/Core/Base.h>
#include <JAZZY/IMGUI/imgui.h>
#include <JAZZY/UI/Screens/Screens.h>

#include <vector>

namespace jazzy
{
    class DebugConsole : public Screens
    {
    public:
        DebugConsole();

        void draw() override;

        // For manipulating debug log list messages
        void AddToDebugLog(std::string newMsg);
        void ClearDebugLog();

        ~DebugConsole();

    private:
        std::vector<std::string> logMessages{};

        bool isOpen = false;
        bool autoScroll = true;

        float expandedHeight = 180.0f;
        float collapsedHeight = 40.0f;
    };

}


#include <JAZZY/UI/Screens/MenuScreen.h>
#include <JAZZY/IMGUI/imgui.h>
#include <JAZZY/Components/CubeComponent.h>
#include <JAZZY/Game/World.h>
#include <JAZZY/Game/GameObject.h>
#include <iostream>

jazzy::MenuScreen::MenuScreen(World& world)
    : Screens("Menu"), m_world(world)
{
}

void jazzy::MenuScreen::draw()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save"))
            {
            }

            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("In Progress");
            }

            if (ImGui::MenuItem("Exit"))
            {
            }

            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("In Progress");
            }

            ImGui::EndMenu();
        }


        if (ImGui::BeginMenu("Game Object"))
        {
            if (ImGui::MenuItem("Create Cube"))
            {
                auto cube = m_world.createGameObject<GameObject>();

                cube->createOrGetComponent<CubeComponent>();

                auto transform = cube->createOrGetComponent<TransformComponent>();

                float newPosition = m_cubeCount * 1.0f;

                transform->setPosition({ 0.0f, 1.0f, newPosition + 0.5f });

                m_cubeCount++;
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("Creates a Cube");
            }
                
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
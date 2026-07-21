#include <JAZZY/UI/Screens/MenuScreen.h>
#include <JAZZY/IMGUI/imgui.h>
#include <JAZZY/Components/CubeComponent.h>
#include <JAZZY/Game/World.h>
#include <JAZZY/Game/GameObject.h>

#include <format>
#include <string>
#include <iostream>

jazzy::MenuScreen::MenuScreen(World& world)
    : Screens("Menu"), m_world(world)
{
}

void jazzy::MenuScreen::draw()
{
    static bool showCredits = false;

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("About"))
        {
            if (ImGui::MenuItem("Credits"))
            {
                showCredits = true;
            }

            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("View engine credits");
            }

            ImGui::EndMenu();
        }


        if (ImGui::BeginMenu("Game Object"))
        {
            if (ImGui::MenuItem("Create Cube"))
            {
                auto numComp = 0u;
                auto allCubes = m_world.getComponents<CubeComponent>(numComp);
                auto worldPlane = allCubes[0];
                auto& parent = worldPlane->getGameObject();

                std::string name = "Cube " + std::to_string(m_cubeCount);
                auto cube = m_world.createGameObject<GameObject>(name);

                cube->createOrGetComponent<CubeComponent>();

                auto transform = cube->createOrGetComponent<TransformComponent>();

                float newPosition = m_cubeCount * 1.5f; //1.5 is for spacing

                transform->setPosition({ 0.0f, 1.0f, newPosition});

                cube->setParent(&parent);

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
    if (showCredits)
    {
        ImGui::OpenPopup("Credits");
        showCredits = false;
    }

    if (ImGui::BeginPopupModal("Credits", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("JAZZY Engine");
        ImGui::Separator();

        ImGui::Text("Created by:");
        ImGui::BulletText("Vivian Angel Chen");
        ImGui::BulletText("Zivv Elomina");
        ImGui::BulletText("Johann Naranjo");
        ImGui::BulletText("Yzabelle Anne Montuerto");
        ImGui::Spacing();

        ImGui::Separator();
        ImGui::BulletText("Based on PardCode's CPP 3D Game Tutorial Series");
        ImGui::Spacing();

        ImGui::Separator();
        if (ImGui::Button("Close"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}
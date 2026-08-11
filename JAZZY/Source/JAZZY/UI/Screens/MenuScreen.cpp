#include <JAZZY/UI/Screens/MenuScreen.h>
#include <JAZZY/IMGUI/imgui.h>
#include <JAZZY/Components/CubeComponent.h>
#include <JAZZY/Components/PlaneComponent.h>
#include <JAZZY/Components/SphereComponent.h>
#include <JAZZY/Components/CylinderComponent.h>
#include <JAZZY/Components/CapsuleComponent.h>
#include <JAZZY/Game/World.h>
#include <JAZZY/Game/GameObject.h>
#include <JAZZY/Resource/ResourceManager.h>

#include <format>
#include <string>
#include <iostream>

jazzy::MenuScreen::MenuScreen(World& world, ResourceManager& resource)
    : Screens("Menu"), m_world(world), m_resource(resource)
{
}

void jazzy::MenuScreen::draw()
{
    static bool showCredits = false;

    if (ImGui::BeginMainMenuBar())
    {
        auto woodTexture = m_resource.createResourceFromFile<jazzy::TextureResource>(L"./Game/Assets/Textures/wood.jpg");
        auto woodMat = m_resource.createResourceFromFile<jazzy::MaterialResource>(L"./Game/Assets/Shaders/BasicShader.hlsl");
        woodMat->setTexture(0, woodTexture);

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
            if (ImGui::MenuItem("Create Plane"))
            {
                std::string name =
                    "Plane " + std::to_string(m_planeCount);
                float newPosition =
                    m_planeCount * 1.5f;

                auto id = m_world.AddGameSceneObject(name, { World::COMP_Plane }, { 0.0f, 1.0f, newPosition });
                auto plane = m_world.getGameObjectByID(id);
                auto planeComp = plane->getComponent<jazzy::PlaneComponent>();
                planeComp->setMaterial(woodMat);
                plane->getTransform().setScale({ 25.0f, 25.0f, 25.0f });

                m_planeCount++;
            }

            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("Creates a Plane");
            }

            if (ImGui::MenuItem("Create Cube"))
            {
                std::string name =
                    "Cube " + std::to_string(m_cubeCount);

                float newPosition =
                    m_cubeCount * 1.5f;

                auto id = m_world.AddGameSceneObject(name, { World::COMP_Cube }, { 0.0f, 1.0f, newPosition });
                auto obj = m_world.getGameObjectByID(id);
                auto objComp = obj->getComponent<jazzy::CubeComponent>();
                objComp->setMaterial(woodMat);

                m_cubeCount++;
            }

            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("Creates a Cube");
            }

            if (ImGui::MenuItem("Create Sphere"))
            {
                std::string name =
                    "Sphere " + std::to_string(m_sphereCount);

                float newPosition =
                    m_sphereCount * 1.5f;

                auto id = m_world.AddGameSceneObject(name, { World::COMP_Sphere }, { 0.0f, 1.0f, newPosition });
                auto obj = m_world.getGameObjectByID(id);
                auto objComp = obj->getComponent<jazzy::SphereComponent>();
                objComp->setMaterial(woodMat);

                m_sphereCount++;
            }

            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("Creates a Sphere");
            }

            if (ImGui::MenuItem("Create Capsule"))
            {
                std::string name =
                    "Capsule " + std::to_string(m_capsuleCount);

                float newPosition =
                    m_capsuleCount * 1.5f;

                auto id = m_world.AddGameSceneObject(name, { World::COMP_Capsule }, { 0.0f, 1.0f, newPosition });
                auto obj = m_world.getGameObjectByID(id);
                auto objComp = obj->getComponent<jazzy::CapsuleComponent>();
                objComp->setMaterial(woodMat);

                m_capsuleCount++;
            }

            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("Creates a Capsule");
            }

            if (ImGui::MenuItem("Create Cylinder"))
            {
                std::string name =
                    "Cylinder " + std::to_string(m_cylinderCount);

                float newPosition =
                    m_cylinderCount * 1.5f;

                auto id = m_world.AddGameSceneObject(name, { World::COMP_Cylinder }, { 0.0f, 1.0f, newPosition });
                auto obj = m_world.getGameObjectByID(id);
                auto objComp = obj->getComponent<jazzy::CylinderComponent>();
                objComp->setMaterial(woodMat);

                m_cylinderCount++;
            }

            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("Creates a Cylinder");
            }

            ImGui::EndMenu();
        }

        if (m_world.currentSceneState == m_world.EDIT_MODE) {
            if (ImGui::Button(" > "))
            {
                m_world.ChangeSceneState(m_world.PLAY_MODE);
            }
        } 
        else if (m_world.currentSceneState == m_world.PLAY_MODE) {
            if (ImGui::Button(" [] "))
            {
                m_world.ChangeSceneState(m_world.EDIT_MODE);
            }
            if (ImGui::Button(" || "))
            {
                m_world.ChangeSceneState(m_world.PAUSED_MODE);
            }
        }
        else if (m_world.currentSceneState == m_world.PAUSED_MODE) {
            if (ImGui::Button(" [] "))
            {
                m_world.ChangeSceneState(m_world.EDIT_MODE);
            }
            if (ImGui::Button(" -> "))
            {
                m_world.ProceedWhenPaused();
            }
        }

        ImGui::EndMainMenuBar();
    }

    if (showCredits)
    {
        ImGui::OpenPopup("Credits");
        showCredits = false;
    }

    if (ImGui::BeginPopupModal(
        "Credits",
        nullptr,
        ImGuiWindowFlags_AlwaysAutoResize))
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
        ImGui::BulletText(
            "Based on PardCode's CPP 3D Game Tutorial Series"
        );

        ImGui::Spacing();

        ImGui::Separator();

        if (ImGui::Button("Close"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}
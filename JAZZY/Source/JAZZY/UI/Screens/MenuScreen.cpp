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

                auto plane =
                    m_world.createGameObject<GameObject>(name);

                auto planeComp =
                    plane->createOrGetComponent<PlaneComponent>();

                planeComp->setMaterial(woodMat);

                auto transform =
                    plane->createOrGetComponent<TransformComponent>();

                float newPosition =
                    m_planeCount * 1.5f;

                transform->setPosition(
                    { 0.0f, 1.0f, newPosition }
                );

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

                auto cube =
                    m_world.createGameObject<GameObject>(name);

                auto cubeComp =
                    cube->createOrGetComponent<CubeComponent>();

                cubeComp->setMaterial(woodMat);

                auto transform =
                    cube->createOrGetComponent<TransformComponent>();

                float newPosition =
                    m_cubeCount * 1.5f;

                transform->setPosition(
                    { 0.0f, 1.0f, newPosition }
                );

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

                auto sphere =
                    m_world.createGameObject<GameObject>(name);

                auto sphereComp =
                    sphere->createOrGetComponent<SphereComponent>();

                sphereComp->setMaterial(woodMat);

                auto transform =
                    sphere->createOrGetComponent<TransformComponent>();

                float newPosition =
                    m_sphereCount * 1.5f;

                transform->setPosition(
                    { 0.0f, 1.0f, newPosition }
                );

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

                auto capsule =
                    m_world.createGameObject<GameObject>(name);

                auto capsuleComp =
                    capsule->createOrGetComponent<CapsuleComponent>();

                capsuleComp->setMaterial(woodMat);

                auto transform =
                    capsule->createOrGetComponent<TransformComponent>();

                float newPosition =
                    m_capsuleCount * 1.5f;

                transform->setPosition(
                    { 0.0f, 1.0f, newPosition }
                );

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

                auto cylinder =
                    m_world.createGameObject<GameObject>(name);

                auto cylinderComp =
                    cylinder->createOrGetComponent<CylinderComponent>();

                cylinderComp->setMaterial(woodMat);

                auto transform =
                    cylinder->createOrGetComponent<TransformComponent>();

                float newPosition =
                    m_cylinderCount * 1.5f;

                transform->setPosition(
                    { 0.0f, 1.0f, newPosition }
                );

                m_cylinderCount++;
            }

            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("Creates a Cylinder");
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
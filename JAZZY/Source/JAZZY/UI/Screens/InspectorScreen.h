#pragma once

#include <JAZZY/Core/Core.h>
#include <JAZZY/Core/Base.h>
#include <JAZZY/UI/Screens/Screens.h>
#include <vector>

namespace jazzy
{
    class InspectorScreen : public Screens
    {
    public:
        InspectorScreen(World& world, ResourceManager& resource);

        void draw() override;

    private:
        World& m_world;
        ResourceManager& m_resource;
        int m_cubeCount = 1;
        void setHiddenRecursive(GameObject* object, bool hidden);
        void setMaterial(GameObject* object, RefPtr<MaterialResource> material);
    };

}


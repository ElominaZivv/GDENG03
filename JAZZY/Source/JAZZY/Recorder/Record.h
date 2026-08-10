#pragma once

#include "JAZZY/Math/Vec3.h"
#include <string>

namespace jazzy
{
    class Record
    {
    public:
        struct TransformData
        {
            Vec3 position;
            Vec3 rotation;
            Vec3 scale;
        };

    public:
        Record(
            const std::string& objectName,
            const TransformData& oldData,
            const TransformData& newData
        );

        ~Record() = default;

        const std::string& getObjectName() const noexcept;

        Vec3 getOldPosition() const noexcept;
        Vec3 getOldRotation() const noexcept;
        Vec3 getOldScale() const noexcept;

        Vec3 getNewPosition() const noexcept;
        Vec3 getNewRotation() const noexcept;
        Vec3 getNewScale() const noexcept;

    private:
        std::string m_objectName;

        TransformData m_oldData;
        TransformData m_newData;
    };
}
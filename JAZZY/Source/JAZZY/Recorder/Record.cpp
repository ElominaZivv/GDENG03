#include "JAZZY/Recorder/Record.h"

namespace jazzy
{
    Record::Record(
        const std::string& objectName,
        const TransformData& oldData,
        const TransformData& newData)
        : m_objectName(objectName),
        m_oldData(oldData),
        m_newData(newData)
    {
    }

    const std::string& Record::getObjectName() const noexcept
    {
        return m_objectName;
    }

    Vec3 Record::getOldPosition() const noexcept
    {
        return m_oldData.position;
    }

    Vec3 Record::getOldRotation() const noexcept
    {
        return m_oldData.rotation;
    }

    Vec3 Record::getOldScale() const noexcept
    {
        return m_oldData.scale;
    }

    Vec3 Record::getNewPosition() const noexcept
    {
        return m_newData.position;
    }

    Vec3 Record::getNewRotation() const noexcept
    {
        return m_newData.rotation;
    }

    Vec3 Record::getNewScale() const noexcept
    {
        return m_newData.scale;
    }
}
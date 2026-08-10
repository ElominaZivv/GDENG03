#include "JAZZY/Recorder/RecordHolder.h"
#include "JAZZY/Game/World.h"
#include "JAZZY/Game/GameObject.h"
#include "JAZZY/Components/TransformComponent.h"

namespace jazzy
{
    RecordHolder::RecordHolder(World& world)
        : m_world(world)
    {
    }

    void RecordHolder::addRecord(const Record& record)
    {
        if (m_isUndoRedo)
            return;

        bool objectExists = false;

        for (Record& worldRecord : m_worldObjects)
        {
            if (worldRecord.getObjectName() == record.getObjectName())
            {
                worldRecord = record;
                objectExists = true;
                break;
            }
        }

        if (!objectExists)
        {
            m_worldObjects.push_back(record);
        }

        if (m_undoStack.size() >= UNDO_LIMIT)
        {
            m_undoStack.pop_front();
        }

        m_undoStack.push_back(record);
        m_redoStack.clear();
    }

    void RecordHolder::undo()
    {
        if (m_undoStack.empty())
            return;

        Record record = m_undoStack.back();
        m_undoStack.pop_back();

        m_isUndoRedo = true;

        GameObject* object =
            m_world.getGameObjectByName(record.getObjectName());

        if (object)
        {
            TransformComponent* transform =
                object->getComponent<TransformComponent>();

            if (transform)
            {
                transform->setPosition(record.getOldPosition());
                transform->setRotation(record.getOldRotation());
                transform->setScale(record.getOldScale());
            }
        }

        m_isUndoRedo = false;

        m_redoStack.push_back(record);
    }

    void RecordHolder::redo()
    {
        if (m_redoStack.empty())
            return;

        Record record = m_redoStack.back();
        m_redoStack.pop_back();

        m_isUndoRedo = true;

        GameObject* object =
            m_world.getGameObjectByName(record.getObjectName());

        if (object)
        {
            TransformComponent* transform =
                object->getComponent<TransformComponent>();

            if (transform)
            {
                transform->setPosition(record.getNewPosition());
                transform->setRotation(record.getNewRotation());
                transform->setScale(record.getNewScale());
            }
        }

        m_isUndoRedo = false;
        m_undoStack.push_back(record);
    }
}
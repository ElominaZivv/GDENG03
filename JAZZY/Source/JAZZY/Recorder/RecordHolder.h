#pragma once

#include "JAZZY/Recorder/Record.h"

#include <deque>
#include <vector>

namespace jazzy
{
    class World;

    class RecordHolder
    {
    public:
        explicit RecordHolder(World& world);

        void addRecord(const Record& record);
        void undo();
        void redo();

    private:
        static constexpr size_t UNDO_LIMIT = 5;

        World& m_world;

        std::vector<Record> m_worldObjects;

        std::deque<Record> m_undoStack;
        std::deque<Record> m_redoStack;

        bool m_isUndoRedo = false;
    };
}
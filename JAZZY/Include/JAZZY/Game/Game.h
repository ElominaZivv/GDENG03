#pragma once
#include <chrono>
#include <JAZZY/Core/Base.h>
#include <JAZZY/Core/Core.h>

namespace jazzy
{
	class Game: public Base
	{
	public:
		explicit Game(const GameDesc& desc);
		virtual ~Game() override;

		virtual void run() final;
		
	private:
		void onInternalUpdate();
	private:
		// The order is important. What gets initialized and deallocated is based on a stack
		// GraphicsEngine is initialized first and the window last. When deallocated, the window is first, graphics engine last
		UniquePtr<Logger> m_LoggerPtr{};
		RefPtr<EditorCamera> m_editorCamera{};
		UniquePtr<GraphicsEngine> m_graphicsEngine{};
		UniquePtr<Display> m_display{};
		RefPtr<InputSystem> m_inputSystem{};
		bool m_isRunning{ true };

		// Time
		std::chrono::steady_clock::time_point m_previousTime{};
	};
}


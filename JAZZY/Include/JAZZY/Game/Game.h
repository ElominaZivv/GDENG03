#pragma once
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
		std::unique_ptr<Logger> m_LoggerPtr{};
		std::unique_ptr<GraphicsEngine> m_graphicsEngine{};
		std::unique_ptr<Display> m_display{};
		bool m_isRunning{ true };
	};
}


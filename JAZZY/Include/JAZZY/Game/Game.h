#pragma once
#include <JAZZY/Core/Base.h>
#include <JAZZY/Core/Core.h>

namespace jazzy
{
	class Game: public Base
	{
	public:
		Game();
		virtual ~Game() override;

		virtual void run() final;

	private:
		std::unique_ptr<Window> m_display{};
		bool m_isRunning{ true };
	};
}


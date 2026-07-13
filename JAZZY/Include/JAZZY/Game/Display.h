#pragma once
#include <JAZZY/Window/Window.h>

namespace jazzy
{
	class Display final: public Window
	{
	public:
		explicit Display(const DisplayDesc& desc);

		SwapChain& getSwapChain() noexcept;
	private:
		RefPtr<SwapChain> m_swapChain{};

	};
}

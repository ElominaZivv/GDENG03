#include <JAZZY/Game/Display.h>
#include <JAZZY/Graphics/GraphicsDevice.h>

jazzy::Display::Display(const DisplayDesc& desc): Window(desc.window)
{
	m_swapChain = desc.graphicsDevice.createSwapChain({ m_handle, m_size });
}

jazzy::SwapChain& jazzy::Display::getSwapChain() noexcept
{
	return *m_swapChain;
}

#pragma once
#include <JAZZY/Graphics/GraphicsResource.h>

namespace jazzy
{
	class SwapChain final: public GraphicsResource
	{
	public:
		SwapChain(const SwapChainDesc& desc,const GraphicsResourceDesc& gDesc);
		Rect getSize() const noexcept;

		void present(bool vsync = false);
	private:
		void reloadBuffers();
	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain{};
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv{};
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv{};
		Rect m_size{};

		friend class DeviceContext;
	};
}


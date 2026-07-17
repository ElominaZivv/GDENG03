#pragma once
#include<JAZZY/Graphics/GraphicsResource.h>
#include<JAZZY/Core/Common.h>
#include<JAZZY/Core/Base.h>
#include<d3d11.h>
#include<wrl.h>

// I didnt quite understand Pardcode, he wanted to put the GraphicsDevice in this so that its functions are not exposed to the rest of the engine
// Which is why the header is in the source folder instead of the include folder

namespace jazzy
{
	class GraphicsDevice final: public Base, public std::enable_shared_from_this<GraphicsDevice>
	{
	public:
		explicit GraphicsDevice(const GraphicsDeviceDesc& desc);
		virtual ~GraphicsDevice() override;

		RefPtr<SwapChain> createSwapChain(const SwapChainDesc& desc) const;
		RefPtr<DeviceContext> createDeviceContext();
		RefPtr<ShaderBinary> compileShader(const ShaderCompileDesc& desc);
		RefPtr<GraphicsPipelineState> createGraphicsPipelineState(const GraphicsPipelineStateDesc& desc);
		RefPtr<VertexBuffer> createVertexBuffer(const VertexBufferDesc& desc);
		RefPtr<VertexShaderSignature> createVertexShaderSignature(const VertexShaderSignatureDesc& desc);
		RefPtr<ConstantBuffer> createConstantBuffer(const ConstantBufferDesc& desc);
		RefPtr<IndexBuffer> createIndexBuffer(const IndexBufferDesc& desc);
		ID3D11Device* getDevice() const noexcept;
		ID3D11DeviceContext* getContext() const noexcept;
		void setBackBuffer(const SwapChain& swapChain);

		void executeCommandList(DeviceContext& context);
	private:
		GraphicsResourceDesc getGraphicsResourceDesc() const noexcept;
	private:
		// Beautiful smart pointers keep tracks of the number of references to the pointer
		// If the number of references to the pointer == 0, it automatically destroys the things <3
		Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice{};
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dContext{};
		Microsoft::WRL::ComPtr<IDXGIDevice> m_dxgiDevice{};
		Microsoft::WRL::ComPtr<IDXGIAdapter> m_dxgiAdapter{};
		Microsoft::WRL::ComPtr<IDXGIFactory> m_dxgiFactory{};

	};
}

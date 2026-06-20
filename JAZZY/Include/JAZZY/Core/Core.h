#pragma once
#include <stdexcept>
#include <memory>

// Forward Declaration to declare a class without defining it
namespace jazzy
{
	class Base;
	class Window;
	class Game;
	class GraphicsEngine;
	class GraphicsDevice;
	class Logger;
	class SwapChain;
	class Display;
	class DeviceContext;
	class ShaderBinary;
	class GraphicsPipelineState;
	class VertexBuffer;
	class VertexShaderSignature;
	class ConstantBuffer;
	class IndexBuffer;
	class InputSystem;

	using i32 = int;
	using ui32 = unsigned int;
	using f32 = float;
	using d64 = double;

	using SwapChainPtr = std::shared_ptr<SwapChain>;
	using DeviceContextPtr = std::shared_ptr<DeviceContext>;
	using ShaderBinaryPtr = std::shared_ptr<ShaderBinary>;
	using GraphicsPipelineStatePtr = std::shared_ptr<GraphicsPipelineState>;
	using VertexBufferPtr = std::shared_ptr< VertexBuffer>;
	using VertexShaderSignaturePtr = std::shared_ptr< VertexShaderSignature>;
	using ConstantBufferPtr = std::shared_ptr<ConstantBuffer>;
	using IndexBufferPtr = std::shared_ptr<IndexBuffer>;
	using InputSystemPtr = std::shared_ptr<InputSystem>;
}
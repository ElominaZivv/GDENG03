#pragma once
#include <JAZZY/Core/Core.h>
#include <JAZZY/Core/Logger.h>
#include <JAZZY/Math/Rect.h>
#include <JAZZY/Math/Vec3.h>
#include <JAZZY/Math/Vec2.h>

namespace jazzy
{
	struct BaseDesc
	{
		Logger& logger;
	};

	struct WindowDesc
	{
		BaseDesc base;
		Rect size{};
	};

	struct DisplayDesc
	{
		WindowDesc window;
		GraphicsDevice& graphicsDevice;
	};

	struct GraphicsEngineDesc
	{
		BaseDesc base;
		RefPtr<EditorCamera>& editorCamera;
	};

	struct GraphicsDeviceDesc
	{
		BaseDesc base;
	};

	struct SwapChainDesc
	{
		void* winHandle{};
		Rect winSize{};
	};

	enum class ShaderType
	{
		
		VertexShader = 0,
		PixelShader
	};

	struct ShaderCompileDesc
	{

		const char* shaderSourceName{};
		const void* shaderSourceCode{};
		size_t shaderSourceCodeSize{};
		const char* shaderEntryPoint{};
		ShaderType shaderType{};
	};

	struct VertexShaderSignatureDesc
	{
		const RefPtr<ShaderBinary>& vsBinary;
	};

	struct BinaryData
	{
		const void* data{};
		size_t dataSize{};
	};

	struct GraphicsPipelineStateDesc
	{
		const GraphicsPipelineLayout& layout;
	};

	struct VertexBufferDesc
	{
		const void* vertexList{};
		ui32 vertexListSize{};
		ui32 vertexSize{};
	};

	struct IndexBufferDesc
	{
		const void* indexList{};
		ui32 indexListSize{};
	};

	struct ConstantBufferDesc
	{
		const void* buffer{};
		ui32 bufferSize{};
	};

	struct GameDesc
	{
		Rect windowSize{ 1280,720 };
		Logger::LogLevel logLevel = Logger::LogLevel::Error;
	};

	enum class KeyCode
	{
		Unkown = 0,

		// Letters
		A, B, C, D, E, F, G,
		H, I, J, K, L, M, N,
		O, P, Q, R, S, T, U,
		V, W, X, Y, Z,

		// Numbers
		Num0,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,


		Escape,
		Shift,
		Space,
		Enter,

		// Mouse buttons (optional inclusion)
		MouseLeft,
		MouseRight,
		MouseMiddle,

		// Arrows
		Up,
		Down,
		Left,
		Right,

		Count
	};

	struct InputSystemDesc
	{
		BaseDesc base;
	};


	struct EditorCameraDesc
	{
		BaseDesc base;
		InputSystem& input;
	};

	struct GameContext
	{
		InputSystem& input;
		ResourceManager& resourceManager;
		GraphicsDevice& device;
	};

	struct GameObjectDesc
	{
		BaseDesc base;
		GameContext gameContext;
		World& world;
		std::string name;
	};

	struct WorldDesc
	{
		BaseDesc base;
		GameContext gameContext;
	};

	struct ComponentDesc
	{
		BaseDesc base;
		GameObject& object;
		World& world;
		GameContext& context;
	};

	struct TextureDesc
	{
		Rect size{};
		const void* pixels{};
	};

	struct ResourceDesc
	{
		BaseDesc base;
		const wchar_t* path{};
		ResourceManager& manager;
	};

	struct SystemContext
	{
		GraphicsDevice& graphicsDevice;
	};

	struct MaterialResourceDesc
	{
		ResourceDesc base;
		GraphicsDevice& graphicsDevice;
	};

	struct ResourceManagerDesc
	{
		BaseDesc base;
		SystemContext context;
	};

	struct TextureResourceDesc
	{
		ResourceDesc base;
		GraphicsDevice& graphicsDevice;
	};

	struct MeshResourceDesc
	{
		ResourceDesc base;
		GraphicsDevice& graphicsDevice;
	};

	struct GraphicsPipelineLayoutDesc
	{
		const RefPtr<ShaderBinary>& vsBinary;
		const RefPtr<ShaderBinary>& psBinary;
	};

	struct WorldRendererDesc
	{
		BaseDesc base;
		GraphicsDevice& engine;
	};

	struct UIManagerDesc
	{
		Display& display;
		GraphicsDevice& graphicsDevice;
		World& world;
	};

	struct MeshVertex
	{
		Vec3 position{};
		Vec2 texcoord{};
		Vec3 normal{};
	};
	struct MaterialSlot
	{
		ui32 startIndex{};
		ui32 indexCount{};
		i32 materialIndex{};
	};
	struct WorldPhysicsDesc
	{
		BaseDesc base;
	};
}
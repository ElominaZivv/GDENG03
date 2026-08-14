#pragma once
#include <stdexcept>
#include <memory>

// JSON Parser
#include <json-forwards.h>

//	These /**/ is causing issues with the code. I have to use the // instead >:[
//	these "= delete" forbids the surface level copying of anything that inherits from the Base class
//	Basically, you cant assign this class to anything and manipulate the copy, this will now cause an error
//	You need to directly manipulate the instance
#define dx3d_disable_copy_and_move(Class)\
protected:\
	Class(const Class&) = delete;\
	Class& operator=(const Class&) = delete;\
	Class(Class&&) = delete;\
	Class& operator=(Class&&) = delete;

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
	class GraphicsPipelineLayout;
	class GraphicsPipelineState;
	class VertexBuffer;
	class VertexShaderSignature;
	class ConstantBuffer;
	class IndexBuffer;
	class InputSystem;
	class EditorCamera;
	class GameObject;
	class World;
	class UIManager;
	class Screens;
	class MenuScreen;
	class DebugConsole;
	class Texture;

	class WorldRenderer;
	class WorldPhysics;

	class ResourceManager;
	class Resource;
	class MaterialResource;
	class TextureResource;
	class MeshResource;

	class SceneSerializer;

	using i32 = int;
	using ui32 = unsigned int;
	using f32 = float;
	using d64 = double;

	template <typename T> using RefPtr = std::shared_ptr<T>;
	template <typename T> using UniquePtr = std::unique_ptr<T>;
}

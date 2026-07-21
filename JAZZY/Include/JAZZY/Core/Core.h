#pragma once
#include <stdexcept>
#include <memory>

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
	class Texture;

	class ResourceManager;
	class Resource;

	using i32 = int;
	using ui32 = unsigned int;
	using f32 = float;
	using d64 = double;

	template <typename T> using RefPtr = std::shared_ptr<T>;
	template <typename T> using UniquePtr = std::unique_ptr<T>;
}
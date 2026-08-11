#pragma once
#include <JAZZY/Core/Base.h>
#include <JAZZY/Core/Core.h>
#include <chrono>

namespace jazzy
{
	class Game
	{
		dx3d_disable_copy_and_move(Game)

	public:
		explicit Game(const GameDesc& desc);
		virtual ~Game();

		virtual World& getWorld() noexcept final;
		virtual Logger& getLogger() const noexcept final;
		virtual InputSystem& getInputSystem() noexcept final;
		virtual ResourceManager& getResourceManager() noexcept final;

		virtual void run() final;
		
	protected:
		virtual void onCreate() {}
		virtual void onUpdate(f32 deltaTime) {}
	private:
		void onInternalUpdate();
	private:
		// The order is important. What gets initialized and deallocated is based on a stack
		// GraphicsEngine is initialized first and the window last. When deallocated, the window is first, graphics engine last
		UniquePtr<Logger> m_logger{};
		UniquePtr<InputSystem> m_inputSystem{};
		RefPtr<GraphicsDevice> m_graphicsDevice{};
		UniquePtr<Display> m_display{};
		UniquePtr<UIManager> m_uiManager{};
		UniquePtr<ResourceManager> m_resourceManager{};
		UniquePtr<World> m_world{};

		UniquePtr<WorldPhysics>m_worldPhysics{};
		UniquePtr<WorldRenderer>m_worldRenderer{};

		// My Free-Cam
		RefPtr<EditorCamera> m_editorCamera{};

		bool m_isRunning{ true };

		GameObject* test_parent;
		GameObject* test_child;

		// Time
		std::chrono::steady_clock::time_point m_previousTime{};
	};
}


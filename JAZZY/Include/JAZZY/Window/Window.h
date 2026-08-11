#pragma once
#define NOMINMAX // to avoid conflict with Windows.h MIN and MAX declarations
#include <JAZZY/Core/Base.h>
#include <JAZZY/Core/Common.h>
#include <Windows.h>

namespace jazzy
{
	class Window: public Base
	{
	public:
		explicit Window(const WindowDesc& desc);
		virtual ~Window() override;

		HWND getHWND() const noexcept;

		Rect getClientAreaInScreenSpace();

	protected:
		void* m_handle{};
		Rect m_size{};
	};
}
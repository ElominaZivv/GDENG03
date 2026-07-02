#pragma once
#include <JAZZY/Core/Base.h>
#include <JAZZY/Core/Common.h>

namespace jazzy
{
	class Window: public Base
	{
	public:
		explicit Window(const WindowDesc& desc);
		virtual ~Window() override;

		Rect getClientAreaInScreenSpace();

	protected:
		void* m_handle{};
		Rect m_size{};
	};
}
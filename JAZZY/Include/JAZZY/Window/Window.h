#pragma once
#include <JAZZY/Core/Base.h>

namespace jazzy
{
	class Window: public Base
	{
	public:
		Window();
		virtual ~Window() override;

	private:
		void* m_handle{};
	};
}
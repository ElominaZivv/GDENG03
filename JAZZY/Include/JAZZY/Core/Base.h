#pragma once
#include <JAZZY/Core/Common.h>

namespace jazzy
{
	class Base
	{
		dx3d_disable_copy_and_move(Base)
	public:
		explicit Base(const BaseDesc& desc);
		virtual ~Base();
		virtual jazzy::Logger& getLogger() noexcept final;

	protected:
		jazzy::Logger& m_logger;
	};
}
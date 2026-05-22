#include "JAZZY/Core/Base.h"
using namespace jazzy;

jazzy::Base::Base(const BaseDesc& desc): m_logger(desc.logger)
{
}

jazzy::Base::~Base()
{
}

Logger& Base::getLogger() noexcept
{
	return m_logger;
}

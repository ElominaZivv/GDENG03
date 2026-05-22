#include "JAZZY/Core/Base.h"
using namespace jazzy;

jazzy::Base::Base(const BaseDesc& desc): m_logger(desc.logger)
{
}

jazzy::Base::~Base()
{
}

Logger& Base::getLogger() const noexcept
{
	return m_logger;
}

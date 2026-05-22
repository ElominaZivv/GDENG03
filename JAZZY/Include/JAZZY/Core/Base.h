#pragma once
#include <JAZZY/Core/Common.h>

namespace jazzy
{
	class Base
	{
	public:
		explicit Base(const BaseDesc& desc);
		virtual ~Base();
		virtual Logger& getLogger() noexcept final;

	protected:
		//	These /**/ is causing issues with the code. I have to use the // instead >:[
		//	these "= delete" forbids the surface level copying of anything that inherits from the Base class
		//	Basically, you cant assign this class to anything and manipulate the copy, this will now cause an error
		//	You need to directly manipulate the instance
		Base(const Base&) = delete;
		Base(Base&) = delete;
		Base& operator = (const Base&) = delete;
		Base& operator = (Base&&) = delete;

	protected:
		Logger& m_logger;
	};
}

#define DX3DLogInfo(message)\
	DX3DLog(getLogger(), Logger::LogLevel::Info, message)

#define DX3DLogWarning(message)\
	DX3DLog(getLogger(), Logger::LogLevel::Warning, message)

#define DX3DLogError(message)\
	DX3DLog(getLogger(), Logger::LogLevel::Error, message)

#define DX3DLogThrowError(message)\
	DX3DLogThrow(getLogger(), std::runtime_error, Logger::LogLevel::Error, message)

#define DX3DLogThrowInvalidArg(message)\
	DX3DLogThrow(getLogger(), std::invalid_argument, Logger::LogLevel::Error, message)
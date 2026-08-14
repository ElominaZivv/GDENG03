#pragma once
#include <JAZZY/Core/Core.h>
#include <JAZZY/Core/Base.h>
#include <string>

namespace jazzy
{
	class Resource : public Base
	{
	public:
		explicit Resource(const ResourceDesc& desc);
		const std::wstring& getPath() const noexcept;
	protected:
		std::wstring m_path{};
		ResourceManager& m_manager;
	};
}

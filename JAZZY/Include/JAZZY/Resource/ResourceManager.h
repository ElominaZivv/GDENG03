#pragma once

#include <unordered_map>
#include <JAZZY/Core/Core.h>
#include <JAZZY/Core/Base.h>

namespace jazzy
{
	class ResourceManager final : public Base
	{
	public:
		explicit ResourceManager(const ResourceManagerDesc& desc);
		template <typename T>
		RefPtr<T> createResourceFromFile(const wchar_t* file_path)
		{
			return std::dynamic_pointer_cast<T>(createResourceFileFromConcrete(file_path));
		}
	private:
		void createResourceFileFromConcrete(const wchar_t* file_path);
	private:
		std::unordered_map<std::wstring, RefPtr<Resource>> m_resources{};
		SystemContext m_context;
	};
}

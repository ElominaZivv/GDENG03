#include <filesystem>
#include <JAZZY/Resource/ResourceManager.h>


jazzy::ResourceManager::ResourceManager(const ResourceManagerDesc& desc) : Base(desc.base), m_context(desc.context)
{
}

void jazzy::ResourceManager::createResourceFileFromConcrete(const wchar_t* file_path)
{
	std::filesystem::path resourcePath{ file_path };
	auto ext = resourcePath.extension();

	auto it = m_resources.find(file_path);
	if (it != m_resources.end())
	{
		//auto mat = std::dynamic_pointer_cast<MaterialResource>( it -> second );
	}
}

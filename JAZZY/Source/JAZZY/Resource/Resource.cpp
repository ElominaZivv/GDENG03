#include <JAZZY/Resource/Resource.h>

jazzy::Resource::Resource(const ResourceDesc& desc): Base (desc.base), m_path(desc.path), m_manager(desc.manager)
{
}

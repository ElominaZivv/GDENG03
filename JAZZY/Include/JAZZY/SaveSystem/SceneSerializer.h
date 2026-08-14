#pragma once

#include <JAZZY/Core/Common.h>
#include <JAZZY/Core/Base.h>
#include <filesystem>
#include <string>
#include <json.h>

namespace jazzy
{
	class SceneSerializer final : public Base
	{
	public:
		explicit SceneSerializer(const SceneSerializerDesc& desc);

		void save(const World& world, const std::string& levelName);
		void load(World& world, const std::string& levelName);

		const std::filesystem::path& getLevelDirectory() const noexcept;

	private:
		std::filesystem::path getLevelPath(const std::string& levelName) const noexcept;

	private:
		std::filesystem::path m_levelDirectory;

		friend class World;
	};
}

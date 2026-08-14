#pragma once

#include <filesystem>
#include <string>

namespace jazzy
{
	class World;

	// Reads and writes editable world data as text-based .lvl files.
	class SceneSerializer final
	{
	public:
		explicit SceneSerializer(World& world,
			std::filesystem::path levelDirectory = std::filesystem::path("Game") / "Assets" / "Levels");

		bool save(const std::string& levelName, std::string* error = nullptr) const;
		bool load(const std::string& levelName, std::string* error = nullptr) const;

		const std::filesystem::path& getLevelDirectory() const noexcept;

	private:
		std::filesystem::path getLevelPath(const std::string& levelName) const;

	private:
		World& m_world;
		std::filesystem::path m_levelDirectory;
	};
}

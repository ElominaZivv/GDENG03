#pragma once

#include <JAZZY/Core/Common.h>
#include <JAZZY/Core/Base.h>
#include <JAZZY/Game/World.h>
#include <filesystem>
#include <string>
#include <vector>
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
		struct SavedObject
		{
			std::string name;
			std::string id;
			std::string parentId;
			std::vector<ComponentType> components;
			std::string meshPath;
			std::string materialPath;
			std::vector<std::string> texturePaths;
			Vec3 position;
			Vec3 rotation;
			Vec3 scale;
		};

		static constexpr const char* LevelFormat = "jazzy-level";
		static constexpr int LevelVersion = 1;

		static const char* componentTypeToString(ComponentType component);
		static bool componentTypeFromString(const std::string& name, ComponentType& component);
		static std::vector<ComponentType> getComponentTypes(GameObject& object);
		static std::string getResourcePath(const Resource* resource);
		static MaterialResource* getMaterial(GameObject& object);
		static void setMaterial(GameObject& object, const RefPtr<MaterialResource>& material);
		static Json::Value vec3ToJson(const Vec3& vector);
		static bool jsonToVec3(const Json::Value& value, Vec3& vector);

		std::filesystem::path getLevelPath(const std::string& levelName) const noexcept;

	private:
		std::filesystem::path m_levelDirectory;

		friend class World;
	};
}

#pragma once

#include <cstdint>
#include <nlohmann/json.hpp>

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>

#include "Render/Mesh.h"
#include "Render/Texture.h"

#include "nlohmann/json_fwd.hpp"

using json = nlohmann::json;

namespace Sauce
{
	constexpr uint32_t CUBE_UUID {0};
	constexpr uint32_t PLANE_UUID {1};
	constexpr uint32_t CYLINDER_UUID {2};
	
	constexpr uint32_t EXTENSION_LENGTH {4};

	enum class AssetType
	{
		ILLEGAL,

		MESH_JSON,
		MESH_GLB,

		IMAGE
	};

	struct Asset
	{
		AssetType type;
		std::string path;
	};

	struct MeshAsset
	{
		std::unique_ptr<Mesh> mesh;
		std::string name {};
	};

	struct TextureAsset
	{
		std::unique_ptr<Texture> texture {};
		std::string name {};
	};

	class AssetSystem
	{
	public:
		AssetSystem();
		~AssetSystem();

		std::unordered_map<uint32_t, MeshAsset> p_Meshes;
		std::unordered_map<uint32_t, TextureAsset> p_Textures;
		size_t p_MeshCount {}, p_TextureCount {};

		void PushAssets(json j);
		void LoadAssets();
	private:
		std::vector<Asset> m_AssetsToLoad;

		AssetType GetTypeByPath(const std::string &path);
		std::optional<std::unique_ptr<Mesh>> CreateMeshFile(const std::string &path);
	};
}

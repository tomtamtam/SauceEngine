#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <print>

#include <algorithm>
#include <filesystem>
#include <memory>

#include "AssetSystem.h"

#include "Engine/vendor/utils_hpp/escapes.hpp"

#include "Render/Mesh.h"

namespace Sauce
{
	AssetSystem::AssetSystem()
	{
		p_Meshes.insert({CUBE_UUID, {{Shapes::CreateCubeMesh()}, "Cube"}});
		p_MeshCount ++;
		p_Meshes.insert({PLANE_UUID, {Shapes::CreatePlaneMesh(), "Plane"}});
		p_MeshCount++;
		p_Meshes.insert({CYLINDER_UUID, {Shapes::CreateCylinderMesh(), "Cylinder"}});
		p_MeshCount++;
	}

	AssetSystem::~AssetSystem()
	{
	}

	void AssetSystem::PushAssets(json j)
	{
		for(size_t i{}; i < j.size(); i++)
		{
			std::string path = j[i];
			Asset asset {GetTypeByPath(path), path};
			m_AssetsToLoad.push_back(asset);
		}
	}

	AssetType AssetSystem::GetTypeByPath(const std::string &path)
	{
		if(path.ends_with(".mesh.json")) return AssetType::MESH_JSON;
		else if(path.ends_with(".glb")) return AssetType::MESH_GLB;
		else if (path.ends_with(".pnd") || path.ends_with(".jpg") || path.ends_with(".pbm")) return AssetType::IMAGE;
		else return AssetType::ILLEGAL;
	}

	void AssetSystem::LoadAssets()
	{
		for(auto a : m_AssetsToLoad)
		{
			switch (a.type)
			{
				case AssetType::MESH_JSON:
					break;
				case AssetType::MESH_GLB:
					break;
				case AssetType::IMAGE:
					p_Textures[p_TextureCount].texture = std::make_unique<Texture>(a.path, true, p_TextureCount + 1);
					p_Textures[p_TextureCount].name = a.path.substr(0, a.path.size() - EXTENSION_LENGTH);
					p_TextureCount ++;
					break;
				default:
					break;
			}
		}
	}

	std::optional<std::unique_ptr<Mesh>> AssetSystem::CreateMeshFile(const std::string &path)
	{
//in progress
		if(!std::filesystem::exists(path))
		{
			std::println("{} file to import is no valid file '{}'", Escapes::ColoredText("ERROR: ", Escapes::RED), path);
			return {};
		}
		
		Assimp::Importer importer;

		const aiScene * scene { importer.ReadFile(path, aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_ImproveCacheLocality | aiProcess_SortByPType) };

		std::unique_ptr<Mesh> mesh;

		std::vector<int> indices;
		std::vector<Vertex> vertices;
		
		return mesh;
	}
}

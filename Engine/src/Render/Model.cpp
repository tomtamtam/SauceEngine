#include "Model.h"
#include "Render/Vertex.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "Mesh.h"
#include <cstdint>
#include <iostream>
#include <vector>

namespace Sauce
{
    void Model::loadModel(const std::string &path)
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs); 

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode)
        {
            std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
        }
        m_Dir = path.substr(0, path.find_last_of('/'));
    }

    void Model::processNode(aiNode *node, const aiScene *scene)
    {
        for(uint32_t i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            m_Meshes.push_back(processMesh(mesh, scene));
        }
    }

    Mesh processMesh(aiMesh *mesh, const aiScene *scene)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        for(uint32_t i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            vertices.push_back(vertex);
        }
    }
}

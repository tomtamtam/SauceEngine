#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Render/Mesh.h"
#include "assimp/mesh.h"
#include <string>
#include <vector>

namespace Sauce
{

    class Model
    {
    public:
        Model(const std::string &path)
        {
            loadModel(path);
        }

    private:
        std::vector<Mesh> m_Meshes;
        std::string m_Dir;

        void loadModel(const std::string &path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    };
}

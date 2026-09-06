#include <assimp/scene.h>

#include <fstream>
#include <memory>

#include "Mesh.h"


namespace Sauce::Shapes
{

	std::unique_ptr<Mesh> CreatePlaneMesh()
    {
		static const Vertex vertices[]
		{
			{0.0f, 0.0f, 0.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f,0.0f},
			{1.0f, 0.0f, 0.0f,  1.0f,1.0f,1.0f,1.0f,  1.0f,0.0f},
			{1.0f, 0.0f, 1.0f,  1.0f,1.0f,1.0f,1.0f,  1.0f,1.0f},
			{0.0f, 0.0f, 1.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f,1.0f}
		};

		static const int indices[]
		{
			0, 1, 2,
			2, 3, 0
		};

		return std::make_unique<Mesh>(indices, vertices);
    }

	std::unique_ptr<Mesh> CreateCubeMesh()
    {
        static const Vertex vertices[] =
        {
            // Front (+Z)
            { -0.5f, -0.5f,  0.5f,   1.0f,1.0f,1.0f,1.0f,   0,0 },
            {  0.5f, -0.5f,  0.5f,   1.0f,1.0f,1.0f,1.0f,   1,0 },  
            {  0.5f,  0.5f,  0.5f,   1.0f,1.0f,1.0f,1.0f,   1,1 },  
            { -0.5f,  0.5f,  0.5f,   1.0f,1.0f,1.0f,1.0f,   0,1 },  

            // Back (-Z)
            {  0.5f, -0.5f, -0.5f,   1.0f,1.0f,1.0f,1.0f,   0,0 },
            { -0.5f, -0.5f, -0.5f,   1.0f,1.0f,1.0f,1.0f,   1,0 },
            { -0.5f,  0.5f, -0.5f,   1.0f,1.0f,1.0f,1.0f,   1,1 },
            {  0.5f,  0.5f, -0.5f,   1.0f,1.0f,1.0f,1.0f,   0,1 },

            // Left (-X)
            { -0.5f, -0.5f, -0.5f,   1.0f,1.0f,1.0f,1.0f,   0,0 },
            { -0.5f, -0.5f,  0.5f,   1.0f,1.0f,1.0f,1.0f,   1,0 },
            { -0.5f,  0.5f,  0.5f,   1.0f,1.0f,1.0f,1.0f,   1,1 },
            { -0.5f,  0.5f, -0.5f,   1.0f,1.0f,1.0f,1.0f,   0,1 },

            // Right (+X)
            {  0.5f, -0.5f,  0.5f,   1.0f,1.0f,1.0f,1.0f,   0,0 },
            {  0.5f, -0.5f, -0.5f,   1.0f,1.0f,1.0f,1.0f,   1,0 },
            {  0.5f,  0.5f, -0.5f,   1.0f,1.0f,1.0f,1.0f,   1,1 },
            {  0.5f,  0.5f,  0.5f,   1.0f,1.0f,1.0f,1.0f,   0,1 },

            // Top (+Y)
            { -0.5f,  0.5f,  0.5f,   1.0f,1.0f,1.0f,1.0f,   0,0 },
            {  0.5f,  0.5f,  0.5f,   1.0f,1.0f,1.0f,1.0f,   1,0 },
            {  0.5f,  0.5f, -0.5f,   1.0f,1.0f,1.0f,1.0f,   1,1 },
            { -0.5f,  0.5f, -0.5f,   1.0f,1.0f,1.0f,1.0f,   0,1 },

            // Bottom (-Y)
            { -0.5f, -0.5f, -0.5f,   1.0f,1.0f,1.0f,1.0f,   0,0 },
            {  0.5f, -0.5f, -0.5f,   1.0f,1.0f,1.0f,1.0f,   1,0 },
            {  0.5f, -0.5f,  0.5f,   1.0f,1.0f,1.0f,1.0f,   1,1 },
            { -0.5f, -0.5f,  0.5f,   1.0f,1.0f,1.0f,1.0f,   0,1 },
        };

        static const int indices[] =
        {
             0,  1,  2,   2,  3,  0,   // Front
             4,  5,  6,   6,  7,  4,   //

             8,  9, 10,  10, 11,  8,   // Left
            12, 13, 14,  14, 15, 12,   // Right
            16, 17, 18,  18, 19, 16,   // Top
            20, 21, 22,  22, 23, 20,   // Bottom
        };

		return std::make_unique<Mesh>(indices, vertices);
    }

	std::unique_ptr<Mesh> CreateCylinderMesh(float radius, float height, uint32_t cuts)
    {
		std::vector<Vertex> vertices;
		std::vector<int> indices;

		float circleIterations {2*PI / cuts};

		float circumference {PI * 2 * radius};
		float circumferenceStep {circumference / cuts};

		for(size_t i {}; i <= cuts; i++)
		{
			float currentCircleIteration = i * circleIterations;
			float x = std::cos(currentCircleIteration) * radius;
			float z = std::sin(currentCircleIteration) * radius;

			Vertex bottom { x, -height / 2, z,  1.0f,1.0f,1.0f,1.0f,  circumferenceStep * i, -height / 2 };
			Vertex top    { x,  height / 2, z,  1.0f,1.0f,1.0f,1.0f,  circumferenceStep * i,  height / 2 };

			vertices.push_back(bottom);
			vertices.push_back(top);
		}
		Vertex bottomMiddle = {0.0f, -height / 2, 0.0f,  1.0f,1.0f,1.0f,1.0f, 0.0f,0.0f};
		Vertex topMiddle = {0.0f, height / 2, 0.0f,  1.0f,1.0f,1.0f,1.0f, 0.0f,0.0f};
		vertices.push_back(bottomMiddle);
		vertices.push_back(topMiddle);

		for(size_t i {}; i < cuts; i++)
		{
			int currentBottomIdx = i * 2;

			indices.push_back(currentBottomIdx + BOTTOM_RIGHT_OFFSET);
			indices.push_back(currentBottomIdx + BOTTOM_LEFT_OFFSET);
			indices.push_back(currentBottomIdx + TOP_RIGHT_OFFSET);

			indices.push_back(currentBottomIdx + BOTTOM_LEFT_OFFSET);
			indices.push_back(currentBottomIdx + TOP_RIGHT_OFFSET);
			indices.push_back(currentBottomIdx + TOP_LEFT_OFFSET);
		}

		for(size_t i{}; i < cuts; i++)
		{
			int currentBottomIdx = i * 2;

			indices.push_back(currentBottomIdx + BOTTOM_RIGHT_OFFSET);
			indices.push_back(currentBottomIdx + BOTTOM_LEFT_OFFSET);
			indices.push_back(cuts * 2);

			indices.push_back(currentBottomIdx + TOP_RIGHT_OFFSET);
			indices.push_back(currentBottomIdx + TOP_LEFT_OFFSET);
			indices.push_back(cuts * 2 + 1);
		}

    	return std::make_unique<Mesh>(indices, vertices);
    }
}

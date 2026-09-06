#pragma once

#include <cstdint>

#include <memory>
#include <span>
#include <vector>

#include "Render/Vertex.h"

#include "Render/Buffers/IndexBuffer.h"
#include "Render/Buffers/VertexArray.h"
#include "Render/Buffers/VertexBuffer.h"
#include "Render/Buffers/VertexBufferLayout.h"

namespace Sauce
{
    struct Mesh
    {
        const std::vector<int> Indices;
        const std::vector<Vertex> Vertices;

        VertexArray Vao;
        VertexBuffer Vbo;
        IndexBuffer Ibo;

        Mesh(std::span<const int> indices, std::span<const Vertex> vertices)
            : Indices(indices.begin(), indices.end()), Vertices(vertices.begin(), vertices.end()), Vao(), Vbo((const void*)Vertices.data(), Vertices.size() * sizeof(Vertex)), Ibo((const void*) Indices.data(), Indices.size() * sizeof(int))
        {
            VertexBufferLayout layout;

            //Position
            layout.Push<float>(3);

            //Color
            layout.Push<float>(4);

            //UV
            layout.Push<float>(2);

            Vao.AddBuffer(Vbo, layout);
        }
    };

    namespace Shapes
    {
    	constexpr float CYLINDER_RADIUS {0.5f};
    	constexpr float CYLINDER_HEIGHT {2.0f};
    	constexpr uint32_t CYLINDER_CUTS {18};
    	constexpr float PI {3.1415};
    	
    	constexpr int BOTTOM_RIGHT_OFFSET {2};
    	constexpr int TOP_RIGHT_OFFSET {3};
		constexpr int BOTTOM_LEFT_OFFSET {0};
		constexpr int TOP_LEFT_OFFSET {1};

		std::unique_ptr<Mesh> CreateCubeMesh();
    	std::unique_ptr<Mesh> CreatePlaneMesh();
    	std::unique_ptr<Mesh> CreateCylinderMesh(float radius = CYLINDER_RADIUS, float height = CYLINDER_HEIGHT, uint32_t cuts = CYLINDER_CUTS);
    }
};

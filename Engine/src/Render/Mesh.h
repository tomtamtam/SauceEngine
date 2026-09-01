#pragma once

#include "Render/Buffers/IndexBuffer.h"
#include "Render/Buffers/VertexArray.h"
#include "Render/Buffers/VertexBuffer.h"
#include "Render/Buffers/VertexBufferLayout.h"
#include "Render/Vertex.h"
#include <cstdio>
#include <span>
#include <vector>

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

    inline static Mesh *CreateCubeMesh()
    {
        static const Vertex vertices[] =
        {
            // Front (+Z)
            { -0.5f, -0.5f,  0.5f,   1,0,1,1,   0,0 },
            {  0.5f, -0.5f,  0.5f,   1,0,1,1,   1,0 },  
            {  0.5f,  0.5f,  0.5f,   1,0,1,1,   1,1 },  
            { -0.5f,  0.5f,  0.5f,   1,0,1,1,   0,1 },  

            // Back (-Z)
            {  0.5f, -0.5f, -0.5f,   0,1,1,1,   0,0 },
            { -0.5f, -0.5f, -0.5f,   0,1,1,1,   1,0 },
            { -0.5f,  0.5f, -0.5f,   0,1,1,1,   1,1 },
            {  0.5f,  0.5f, -0.5f,   0,1,1,1,   0,1 },

            // Left (-X)
            { -0.5f, -0.5f, -0.5f,   1,1,0,1,   0,0 },
            { -0.5f, -0.5f,  0.5f,   1,1,0,1,   1,0 },
            { -0.5f,  0.5f,  0.5f,   1,1,0,1,   1,1 },
            { -0.5f,  0.5f, -0.5f,   1,1,0,1,   0,1 },

            // Right (+X)
            {  0.5f, -0.5f,  0.5f,   1,0,0,1,   0,0 },
            {  0.5f, -0.5f, -0.5f,   1,0,0,1,   1,0 },
            {  0.5f,  0.5f, -0.5f,   1,0,0,1,   1,1 },
            {  0.5f,  0.5f,  0.5f,   1,0,0,1,   0,1 },

            // Top (+Y)
            { -0.5f,  0.5f,  0.5f,   0,0,1,1,   0,0 },
            {  0.5f,  0.5f,  0.5f,   0,0,1,1,   1,0 },
            {  0.5f,  0.5f, -0.5f,   0,0,1,1,   1,1 },
            { -0.5f,  0.5f, -0.5f,   0,0,1,1,   0,1 },

            // Bottom (-Y)
            { -0.5f, -0.5f, -0.5f,   0.5,1,1,1,   0,0 },
            {  0.5f, -0.5f, -0.5f,   0.5,1,1,1,   1,0 },
            {  0.5f, -0.5f,  0.5f,   0.5,1,1,1,   1,1 },
            { -0.5f, -0.5f,  0.5f,   0.5,1,1,1,   0,1 },
        };

        // 6 Seiten × 2 Dreiecke × 3 Indices = 36
        // Jede Seite: 0,1,2, 2,3,0  (CCW)
        static const int indices[] =
        {
             0,  1,  2,   2,  3,  0,   // Front
             4,  5,  6,   6,  7,  4,   // Back
             8,  9, 10,  10, 11,  8,   // Left
            12, 13, 14,  14, 15, 12,   // Right
            16, 17, 18,  18, 19, 16,   // Top
            20, 21, 22,  22, 23, 20,   // Bottom
        };

        return new Mesh(indices, vertices);
    }
};

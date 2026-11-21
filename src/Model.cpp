
#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <algorithm>
#include <iostream>
#include <vector>
#include <limits>

bool Model::loadOBJ(const std::string& path) {
    shutdown();

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warning;
    std::string error;

    bool loaded = tinyobj::LoadObj(
        &attrib,
        &shapes,
        &materials,
        &warning,
        &error,
        path.c_str(),
        nullptr,
        true
    );

    if (!warning.empty())
        std::cout << warning << '\n';

    if (!loaded) {
        std::cerr << "OBJ loading failed: " << error << '\n';
        return false;
    }

    std::vector<float> vertices;

    // Get the model's coordinate bounds.
    float minX = std::numeric_limits<float>::max();
    float minY = minX;
    float minZ = minX;

    float maxX = std::numeric_limits<float>::lowest();
    float maxY = maxX;
    float maxZ = maxX;

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            if (index.vertex_index < 0)
                continue;

            size_t i = static_cast<size_t>(index.vertex_index) * 3;

            if (i + 2 >= attrib.vertices.size()) {
                std::cerr << "Invalid OBJ vertex index.\n";
                return false;
            }

            float x = attrib.vertices[i];
            float y = attrib.vertices[i + 1];
            float z = attrib.vertices[i + 2];

            minX = std::min(minX, x);
            minY = std::min(minY, y);
            minZ = std::min(minZ, z);

            maxX = std::max(maxX, x);
            maxY = std::max(maxY, y);
            maxZ = std::max(maxZ, z);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    if (vertices.empty()) {
        std::cerr << "OBJ contains no drawable triangles.\n";
        return false;
    }

    // Center the model and fit its longest dimension to 1 unit.
    float centerX = (minX + maxX) * 0.5f;
    float centerY = (minY + maxY) * 0.5f;
    float centerZ = (minZ + maxZ) * 0.5f;

    float sizeX = maxX - minX;
    float sizeY = maxY - minY;
    float sizeZ = maxZ - minZ;

    float largestSize = std::max({ sizeX, sizeY, sizeZ });

    if (largestSize <= 0.0f)
        return false;

    for (size_t i = 0; i < vertices.size(); i += 3) {
        vertices[i] = (vertices[i] - centerX) / largestSize;
        vertices[i + 1] = (vertices[i + 1] - centerY) / largestSize;
        vertices[i + 2] = (vertices[i + 2] - centerZ) / largestSize;
    }

    vertexCount_ = static_cast<GLsizei>(vertices.size() / 3);

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(float), nullptr
    );

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    std::cout << "Loaded OBJ: " << path
        << " (" << vertexCount_ / 3
        << " triangles)\n";

    return true;
}

void Model::draw() const {
    if (vao_ == 0 || vertexCount_ == 0)
        return;

    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount_);
    glBindVertexArray(0);
}

void Model::shutdown() {
    if (vbo_ != 0)
        glDeleteBuffers(1, &vbo_);

    if (vao_ != 0)
        glDeleteVertexArrays(1, &vao_);

    vbo_ = 0;
    vao_ = 0;
    vertexCount_ = 0;
}

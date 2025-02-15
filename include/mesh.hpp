#ifndef MESH_HPP
#define MESH_HPP

#include <iostream>
#include <fstream>
#include <vector>

#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <glm/ext.hpp>

#include "node.hpp"
#include "shader.hpp"

namespace MeshTemplates
{
    namespace Pyrimid
    {
        std::vector<float> vertices = {
            -0.5f, -0.5f, 0.5f, 0.0f, 0.5f, 1.0f,  // 0
            0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,   // 1
            0.0f, 0.5f, 0.0f, 0.0f, 0.5f, 1.0f,    // 2
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 3
            0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 1.0f   // 4
        };
        std::vector<unsigned int> indices = {
            // face z+
            0, 1, 2,
            // bottom face
            3, 4, 1,
            1, 0, 3,
            // face x+
            1, 4, 2,
            // face x-
            0, 3, 2,
            // face z-
            3, 4, 2};
    };
    namespace Triangle
    {
        std::vector<float> vertices = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // 0
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 1
            0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // 2
        };
        std::vector<unsigned int> indices = {
            // front face
            0, 1, 2
        };
    };
    namespace Square
    {
        std::vector<float> vertices = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // 0
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 1
            0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // 2
            -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, // 3
        };
        std::vector<unsigned int> indices = {
            // first triangle
            0, 1, 2,

            // second triangle
            2, 3, 0
        };
    };
};

/// @brief A small placeholder class to manage mesh instances
class Mesh : public Node
{
private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int VAO, VBO, EBO;
public:
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Shader shader;

    void setVertices(std::vector<float> value);
    void setIndices(std::vector<unsigned int> value);

    Mesh(std::vector<float> verts, std::vector<unsigned int> inds);
    ~Mesh();
};

/// @brief Sets the classes vertices and tests them to make sure they're none zero
/// @param value The value to set the vertices to
inline void Mesh::setVertices(std::vector<float> value)
{
    if (value.size() < 0)
    {
        SDL_Log("WARNING!: Invalid vertices size!");
        return;
    }
    vertices = value;
}

/// @brief Sets the classes indices and tests them to make sure they're none zero
/// @param value The value to set the indices to
inline void Mesh::setIndices(std::vector<unsigned int> value)
{
    if (value.size() < 0)
    {
        SDL_Log("WARNING!: Invalid indices size!");
        return;
    }
    indices = value;
}

/// @brief The default contstructor of the Mesh class
/// @param verts The vertices to be generated
/// @param inds The indices to be generated
Mesh::Mesh(std::vector<float> verts, std::vector<unsigned int> inds)
{
    setVertices(verts);
    setIndices(inds);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, 0, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    this->update = [this](float dt) -> void
    {
        SDL_Log("ACTION PERFORMED: user update called on node: %s", name.c_str());
        // this->transform = glm::rotate(this->transform, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    };

    this->render = [this](glm::mat4 pv) -> void
    {
        SDL_Log("rendered: %s", name);

        glBindVertexArray(this->VAO);
        this->shader.use();
        this->shader.setUniformMat4("pvm", pv * this->transform);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    };
}

/// @brief The default destrctor that cleans up memory
Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

#endif
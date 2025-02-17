#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>

#include "glad/gl.h"
#include <SDL3/SDL.h>
#include <glm/ext.hpp>

#include "nmdlib/material.h"

namespace MeshPrimitives
{
    namespace Triangle
    {
        extern std::vector<float> vertices;
        extern std::vector<uint> indices;
    };
};

class Mesh
{
private:
    uint VAO, VBO, EBO;
    glm::mat4 transform;
    Material material;

public:
    struct InitProps
    {
        std::string base_path;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
    };
    struct RenderProps
    {
        glm::mat4 projection;
        glm::mat4 view;
    };

    std::vector<float> vertices;
    std::vector<uint> indices;

    Mesh() = default;
    Mesh(InitProps props);
    Mesh(std::string path);
    ~Mesh();

    void setTransform(glm::mat4 value);
    glm::mat4 getTransform();

    void load_from_file(std::string path);

    void render(RenderProps props);
};

#endif
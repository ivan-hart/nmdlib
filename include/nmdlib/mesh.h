#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>

#include "glad/gl.h"
#include <SDL3/SDL.h>
#include <glm/ext.hpp>

// struct RenderProps
// {
//     glm::mat4 projection;
//     glm::mat4 view;
// };

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

public:
    std::vector<float> vertices;
    std::vector<uint> indices;

    Mesh(std::vector<float> vertices, std::vector<uint> indices);
    Mesh(std::string path);
    ~Mesh();

    void load_from_file(std::string path);

    void render();
};

#endif
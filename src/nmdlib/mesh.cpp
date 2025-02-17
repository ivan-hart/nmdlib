#include "nmdlib/mesh.h"

namespace MeshPrimitives
{
    namespace Triangle
    {
        std::vector<float> vertices = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f};
        std::vector<uint> indices = {
            0, 1, 2};
    };
};

Mesh::Mesh(InitProps props)
{
    this->vertices = props.vertices;
    this->indices = props.indices;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Material::InitProps materialInitProps = {};
    materialInitProps.shader_name = "basic";
    materialInitProps.base_path = props.base_path;
    material.init(materialInitProps);

    transform = glm::mat4(1.0f);
}

Mesh::Mesh(std::string path)
{
    // do nothing for now
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::setTransform(glm::mat4 value)
{
    transform = value;
}

glm::mat4 Mesh::getTransform()
{
    return transform;
}

void Mesh::load_from_file(std::string path)
{
    // do nothing for now
}

void Mesh::render(RenderProps props)
{
    Material::BindProps materialBindProps =  {.pvm = props.projection * props.view * transform };
    material.bindMaterial(materialBindProps);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
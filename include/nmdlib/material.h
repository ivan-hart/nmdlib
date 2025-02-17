#ifndef MATERIAL_h
#define MATERIAL_h

#include <iostream>
#include <memory>

#include <glm/ext.hpp>

#include "nmdlib/shader.h"

class Material
{
private:
    Shader shader;
public:
    struct InitProps
    {
        std::string base_path;
        std::string shader_name;
    };

    struct BindProps
    {
        glm::mat4 pvm;
    };

    Material() = default;
    Material(InitProps props);
    ~Material();

    void init(InitProps props);
    void bindMaterial(BindProps props);
};

#endif
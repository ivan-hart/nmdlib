#include "nmdlib/material.h"

Material::Material(InitProps props)
{
    std::string shader_name = props.shader_name;

    std::string vert_shader_name = ("%s.vert.glsl", shader_name);
    std::string frag_shader_name = ("%s.frag.glsl", shader_name);

    printf("shader name: %s", vert_shader_name.c_str());

    shader.load(vert_shader_name, frag_shader_name);
}

Material::~Material()
{

}

void Material::init(InitProps props)
{
    std::string shader_name = props.shader_name;
    char vertBuff[256];
    char fragBuff[256];

    snprintf(vertBuff, sizeof(vertBuff), "%sassets/shaders/%s.vert.glsl", props.base_path.c_str(), shader_name.c_str());
    snprintf(fragBuff, sizeof(fragBuff), "%sassets/shaders/%s.frag.glsl", props.base_path.c_str(), shader_name.c_str());

    printf("shader name: %s\n", vertBuff);

    shader.load(std::string(vertBuff), std::string(fragBuff));
}

void Material::bindMaterial(BindProps props)
{
    shader.use();

    shader.setMatrix4fv("pvm", props.pvm);
}
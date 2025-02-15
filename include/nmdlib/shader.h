#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>

#include "glad/gl.h"
#include <SDL3/SDL.h>
#include <glm/ext.hpp>

class Shader
{
public:
    ~Shader();

    unsigned int ID;

    uint load_shader_from_file(std::string path);
    void load(std::string vert_path, std::string frag_path);
    void use();
    void setMatrix4fv(std::string name, glm::mat4 value);
};

#endif
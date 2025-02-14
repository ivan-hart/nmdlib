#ifndef SHADER_HPP
#define SHADER_HPP

#include <iostream>
#include <fstream>
#include <vector>

#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <glm/ext.hpp>

class Shader
{
private:
    unsigned int ID;

    unsigned int loadShaderFromFile(std::string path);
public:
    void load(std::string vert_path, std::string frag_path);
    void use();

    bool setUniformMat4(std::string name, glm::mat4 value);
};

unsigned int Shader::loadShaderFromFile(std::string path)
{
    std::ifstream file(path);
    std::string line;
    std::string src_str;

    if (!file.is_open())
    {
        SDL_Log("Failed to file at: %s", path.c_str());
    }

    while (std::getline(file, line))
    {
        src_str += line + "\n";
    }
    file.close();

    const std::string transfer_str = src_str;
    const char * const src[] = { transfer_str.c_str() }; 

    unsigned int shader;

    if (SDL_strstr(path.c_str(), ".vs"))
    {
        shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(shader, 1, src, NULL);
        glCompileShader(shader);
    }
    else if (SDL_strstr(path.c_str(), "fs"))
    {
        shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(shader, 1, src, NULL);
        glCompileShader(shader);
    }

    int status = true;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (!status)
    {
        char log[256];
        glGetShaderInfoLog(shader, sizeof(log), NULL, log);
        SDL_Log("ERROR!: Failed to compile shader! %s", log);
        SDL_Log("Shader:\n %s", transfer_str.c_str());
    }

    return shader;
}

void Shader::load(std::string vertex_path, std::string fragment_path)
{
    unsigned int vertex = loadShaderFromFile(vertex_path);
    unsigned int fragment = loadShaderFromFile(fragment_path);

    unsigned int shader = glCreateProgram();
    glAttachShader(shader, vertex);
    glAttachShader(shader, fragment);
    glLinkProgram(shader);

    glDetachShader(shader, vertex);
    glDetachShader(shader, fragment);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    int status = true;
    glGetProgramiv(shader, GL_LINK_STATUS, &status);

    if (!status)
    {
        char log[256];
        glGetProgramInfoLog(shader, sizeof(log), NULL, log);
        SDL_Log("ERROR!: Failed to link shader! %s", log);
    }

    ID = shader;
}

void Shader::use()
{
    glUseProgram(this->ID);
}

inline bool Shader::setUniformMat4(std::string name, glm::mat4 value)
{
    bool success = true;

    unsigned int location = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));

    return success;
}

#endif
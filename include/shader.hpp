#ifndef SHADER_HPP
#define SHADER_HPP

#include <iostream>
#include <fstream>
#include <vector>

#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <glm/ext.hpp>

/// @brief A class to manage OpenGL shaders
class Shader
{
private:
    unsigned int ID; // the program id

    unsigned int loadShaderFromFile(std::string path);
public:
    void load(std::string vert_path, std::string frag_path);
    void use();

    bool setUniformMat4(std::string name, glm::mat4 value);
};

/// @brief Loads the requested shader from file and validates it several times
/// @param path The path to the shader
/// @return Returns the shader ID 
unsigned int Shader::loadShaderFromFile(std::string path)
{
    std::ifstream file(path); // the file of the shader
    
    // tests to see if the file actually opened
    if (!file.is_open())
    {
        SDL_Log("Failed to file at: %s", path.c_str());
        return 0;
    }

    std::string line; // the current line being read
    std::string src_str; // the entire file contents being read here

    // loops through the file, till the end, reading line by line
    while (std::getline(file, line))
    {
        // adds the current line of the shader file to the entire file contents
        src_str += line + "\n";
    }
    
    file.close(); // closes the file

    const std::string transfer_str = src_str; // the transfer string
    const char * const src[] = { transfer_str.c_str() }; // the full source code of the shader transfered to something readable by opengl

    unsigned int shader; // the final shader result

    // tests to see if the shader is a vertex shader or a fragment shader
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

    int status = true; // the status of the shader compilation static
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status); // tests the compilation statis of the shader

    // tests if the shader failed to compile
    if (!status)
    {
        char log[256]; // the error log of the shader
        glGetShaderInfoLog(shader, sizeof(log), NULL, log); // retrievs the shader error log
        SDL_Log("ERROR!: Failed to compile shader! %s", log); // prints the log out to the console
        SDL_Log("Shader:\n %s", transfer_str.c_str()); // prints the supposed shader contents
    }

    return shader; // returns the shader ID
}

/// @brief Loads the specified shaders and compiles them into a shader program
/// @param vertex_path The path to the vertex shader
/// @param fragment_path The path to the fragment shader
void Shader::load(std::string vertex_path, std::string fragment_path)
{
    // loads and compiles the individual shaders
    unsigned int vertex = loadShaderFromFile(vertex_path);
    unsigned int fragment = loadShaderFromFile(fragment_path);

    // attached and links the shaders to the program
    unsigned int shader = glCreateProgram();
    glAttachShader(shader, vertex);
    glAttachShader(shader, fragment);
    glLinkProgram(shader);

    // cleans up the indivual shaders
    glDetachShader(shader, vertex);
    glDetachShader(shader, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    // tests to see if the shader program linked successfully
    int status = true;
    glGetProgramiv(shader, GL_LINK_STATUS, &status);

    // if not we print the info log to the console
    if (!status)
    {
        char log[256];
        glGetProgramInfoLog(shader, sizeof(log), NULL, log);
        SDL_Log("ERROR!: Failed to link shader! %s", log);
    }

    // sets the ID of the shader after success
    ID = shader;
}

/// @brief Tells OpenGL to use the shader program
void Shader::use()
{
    glUseProgram(this->ID);
}

/// @brief Sets the matrix at the set uniform location
/// @param name The name of the uniform location
/// @param value The value to be set to the uniform location
/// @return Returns true if the uniform location exists, false if not
inline bool Shader::setUniformMat4(std::string name, glm::mat4 value)
{
    bool success = true;

    unsigned int location = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));

    return success;
}

#endif
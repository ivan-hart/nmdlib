#include "nmdlib/shader.h"

Shader::~Shader()
{
    glDeleteProgram(ID);
}

uint Shader::load_shader_from_file(std::string path)
{
    std::ifstream file;
    file.open(path);

    if (!file.is_open())
    {
        SDL_Log("ERROR!: Failed to open shader at: %s", path.c_str());
        return NULL;
    }

    std::string line;
    std::string file_contents;

    while (std::getline(file, line))
    {
        file_contents += line + "\n";
    }

    file.close();

    const std::string trans_string = file_contents;
    const char * const shader_src[] = { trans_string.c_str() };

    uint shader, format;

    if (SDL_strstr(path.c_str(), "vert"))
    {
        format = GL_VERTEX_SHADER;
    }
    else if (SDL_strstr(path.c_str(), "frag"))
    {
        format = GL_FRAGMENT_SHADER;
    }

    shader = glCreateShader(format);
    glShaderSource(shader, 1, shader_src, NULL);
    glCompileShader(shader);

    return shader;
}

void Shader::load(std::string vert_path, std::string frag_path)
{
    uint vert = load_shader_from_file(vert_path);
    uint frag = load_shader_from_file(frag_path);

    uint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    glDetachShader(program, vert);
    glDetachShader(program, frag);

    glDeleteShader(vert);
    glDeleteShader(frag);

    ID = program;
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setMatrix4fv(std::string name, glm::mat4 value)
{
    uint location = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(location, 1, 0, glm::value_ptr(value));
}
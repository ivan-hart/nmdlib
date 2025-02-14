#include <iostream>
#include <vector>

#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <glm/ext.hpp>

int main(int _argc, char **_argv)
{
    SDL_Log("Executable location: %s", SDL_GetBasePath());

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("ERROR!: Failed to initalize SDL! %s", SDL_GetError());
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    SDL_Window * window = SDL_CreateWindow("Test Window", 800, 450, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    if (!window)
    {
        SDL_Log("ERROR!: Failed to initalize SDL! %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_GLContext ctx = SDL_GL_CreateContext(window);
    if (!ctx)
    {
        SDL_Log("ERROR!: Failed to create GL context! %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    int glVersion = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
    if (glVersion == 0)
    {
        SDL_Log("ERROR!: Failed to initialize GLAD! %s", glGetError());
        SDL_GL_DestroyContext(ctx);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 800, 450);
    glClearColor(0.4, 0.6, 1.0, 1.0);

    const std::string vs_str =
#include "shaders/test.vs"
    ;
    const char * const vs_src[] = { vs_str.c_str() };
    unsigned int vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, vs_src, NULL);
    glCompileShader(vert);

    const std::string fs_str =
#include "shaders/test.fs"
    ;
    const char * const fs_src[] = { fs_str.c_str() };
    unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, fs_src, NULL);
    glCompileShader(frag);

    unsigned int shader_id = glCreateProgram();
    glAttachShader(shader_id, vert);
    glAttachShader(shader_id, frag);
    glLinkProgram(shader_id);

    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.5f, 0.0f, 0.5f, 1.0f, // 0
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // 1
        0.0f, 0.5f, 0.0f, 0.0f, 0.5f, 1.0f, // 2
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 3
        0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 1.0f // 4
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
        3, 4, 2
    };

    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, 0, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    SDL_ShowWindow(window);

    bool running = true;

    glm::mat4 proj = glm::perspective(glm::radians(60.0f), 800.0f / 450.0f, 0.01f, 100.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, -2.0f));
    glm::mat4 model = glm::mat4(1.0f);

    view = glm::lookAt(glm::vec3(view[3]), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        model = glm::rotate_slow(model, glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_id);
        glBindVertexArray(VAO);

        glUniformMatrix4fv(glGetUniformLocation(shader_id, "pvm"), 1, false, glm::value_ptr(proj * view * model));

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);

        SDL_Delay(16);
    }

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);

    glDeleteShader(vert);
    glDeleteShader(frag);
    glDeleteProgram(shader_id);

    SDL_GL_DestroyContext(ctx);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
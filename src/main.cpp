#include <iostream>
#include <fstream>
#include <vector>

#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <glm/ext.hpp>

#include "mesh.hpp"

int main(int _argc, char **_argv)
{
    std::string path = SDL_GetBasePath();

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

    Mesh pyrimidMesh(MeshTemplates::Pyrimid::vertices, MeshTemplates::Pyrimid::indices);
    pyrimidMesh.transform = glm::mat4(1.0f);
    pyrimidMesh.shader.load(path + "assets/shaders/test.vs", path + "assets/shaders/test.fs");

    Mesh squareMesh(MeshTemplates::Square::vertices, MeshTemplates::Square::indices);
    squareMesh.shader.load(path + "assets/shaders/test.vs", path + "assets/shaders/test.fs");
    squareMesh.transform = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
    
    SDL_ShowWindow(window);

    bool running = true;

    glm::mat4 proj = glm::perspective(glm::radians(60.0f), 800.0f / 450.0f, 0.01f, 100.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, -2.0f));

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

        pyrimidMesh.transform = glm::rotate_slow(pyrimidMesh.transform, glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        pyrimidMesh.render(proj * view);
        squareMesh.render(proj * view);

        SDL_GL_SwapWindow(window);

        SDL_Delay(16);
    }

    SDL_GL_DestroyContext(ctx);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
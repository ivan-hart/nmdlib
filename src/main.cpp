#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "glad/gl.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <glm/ext.hpp>

#include "nmdlib/mesh.h"
#include "nmdlib/shader.h"

// class Shader; // a class that helps with managing opengl shaders
// class Mesh; // a class that helps with managing mesh data
// class AssetLoader; // a class that acts as a singleton that loads and stores ONE instance of meshes / shaders
// class Node; // a base class for all game objects that contains a init, update, and render function
// class Engine; // a class that manages the game's lifecycle and obejcts, will act as a singleton
// class Camera; // a class that manages the main camera for the game, will act as a singleton
// class texture; // a class that stores and manages all texutres made in the program
// class Material; // a class that stores information for shaders and textures

int main(int _argc, char **_argv)
{
    std::string base_path = SDL_GetBasePath();

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("ERROR!: Failed to initalize SDL! %s", SDL_GetError());
        return -1;
    }
    SDL_Log("SUCCESS!: Successfully initialized SDL!");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window * window = SDL_CreateWindow("", 800, 450, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN); 
    if (!window)
    {
        SDL_Log("ERROR!: Failed to create the window! %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_GLContext ctx = SDL_GL_CreateContext(window);
    if (!ctx)
    {
        SDL_Log("ERROR!: Failed to create the OpenGL contex! %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
    if (version == 0)
    {
        SDL_Log("ERROR!: Failed to initalize glad!", glGetError());
        SDL_GL_DestroyContext(ctx);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 800, 450);
    glClearColor(0.4f, 0.6f, 0.8f, 1.0f);

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), 800.0f / 450.0f, 0.01f, 100.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));

    Mesh::InitProps meshInitProps = {};
    meshInitProps.base_path = base_path;
    meshInitProps.vertices = MeshPrimitives::Triangle::vertices;
    meshInitProps.indices = MeshPrimitives::Triangle::indices;
    Mesh mesh(meshInitProps);

    SDL_ShowWindow(window);

    bool running = true;

    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Mesh::RenderProps meshRenderProps = {};
        meshRenderProps.projection = projection;
        meshRenderProps.view = view;
        mesh.render(meshRenderProps);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DestroyContext(ctx);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
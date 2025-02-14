#include <iostream>
#include <fstream>
#include <vector>

#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <glm/ext.hpp>

#include "node.hpp"
#include "mesh.hpp"
#include "shader.hpp"

SDL_Window * window;
SDL_GLContext ctx;
std::string path;
Node rootNode;

/// @brief Initializes the application and tests to see if any parts of it fail
/// @return Returns true if the application inits successfully, false if not
bool init()
{
    // gets the path to the applicaiton
    // useful for gettings assets such as shaders
    path = SDL_GetBasePath();

    // initalizes SDL with video support
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("ERROR!: Failed to initalize SDL! %s", SDL_GetError());
        return false;
    }

    // sets the OpenGL context version to 4.6 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    // created the window and hide it until the program is ready to render the first frame
    window = SDL_CreateWindow("Test Window", 800, 450, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    if (!window)
    {
        SDL_Log("ERROR!: Failed to initalize SDL! %s", SDL_GetError());
        return false;
    }

    // creates the opengl context
    ctx = SDL_GL_CreateContext(window);
    if (!ctx)
    {
        SDL_Log("ERROR!: Failed to create GL context! %s", SDL_GetError());
        return false;
    }

    // initalizes glad and test if it was successfull
    int glVersion = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
    if (glVersion == 0)
    {
        SDL_Log("ERROR!: Failed to initialize GLAD! %s", glGetError());
        return false;
    }

    // enables depth tests, sets the viewport to 0, 0, 800, 450, and the clear color to a sky blue
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 800, 450);
    glClearColor(0.4, 0.6, 1.0, 1.0);

    return true;
}

/// @brief Cleans up the application
void cleanup()
{
    SDL_DestroyWindow(window);
    SDL_GL_DestroyContext(ctx);
    SDL_Quit();
}

int main(int _argc, char **_argv)
{
    // intializes the application and exists if the doesn't work
    if (!init())
    {
        SDL_Log("ERROR!: Failed to initialize the applicaiton!");
        cleanup();
        return -1;
    }

    // A pyrimid mesh
    Mesh pyrimidMesh(MeshTemplates::Pyrimid::vertices, MeshTemplates::Pyrimid::indices);
    pyrimidMesh.name = "pyridminMesh";
    pyrimidMesh.transform = glm::mat4(1.0f);
    pyrimidMesh.shader.load(path + "assets/shaders/test.vs", path + "assets/shaders/test.fs");

    rootNode.add_child(std::make_shared<Node>(pyrimidMesh));

    // A square mesh
    Mesh squareMesh(MeshTemplates::Square::vertices, MeshTemplates::Square::indices);
    squareMesh.name = "squareMesh";
    squareMesh.shader.load(path + "assets/shaders/test.vs", path + "assets/shaders/test.fs");
    squareMesh.transform = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
    rootNode.add_child(std::make_shared<Node>(squareMesh));

    // sets up the projection and view matrix
    glm::mat4 proj = glm::perspective(glm::radians(60.0f), 800.0f / 450.0f, 0.01f, 100.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, -2.0f));
    view = glm::lookAt(glm::vec3(view[3]), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    // shows the window since we have initialized the program
    SDL_ShowWindow(window);

    // the bool to show the game should continure looping
    bool running = true;

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

        rootNode.n_update(0.016f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        rootNode.n_render(proj * view);

        SDL_GL_SwapWindow(window);

        SDL_Delay(16);
    }

    cleanup();

    return 0;
}
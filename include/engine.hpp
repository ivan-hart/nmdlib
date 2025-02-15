#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>

#include "glad/gl.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include "singleton.hpp"
#include "camera.hpp"
#include "node.hpp"
#include "mesh.hpp"
#include "shader.hpp"

class Engine : public Singleton<Engine>
{
private:
    SDL_Window * window;
    SDL_GLContext ctx;
    std::string path;
    std::shared_ptr<Node> root;

    bool running;
public:
    std::string get_executable_location();
    bool e_init();
    std::function<bool(void)> init;

    std::shared_ptr<Node> get_root();

    bool is_running();
    void run();

    void e_cleanup();
    std::function<void(void)> cleanup;

    void showWindow();
    void hideWindow();
};

inline std::string Engine::get_executable_location()
{
    return path;
}

/// @brief Initializes the application and tests to see if any parts of it fail
/// @return Returns true if the application inits successfully, false if not
bool Engine::e_init()
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
    SDL_Log("SUCCESS!: Successfully initalized SDL!");

    // sets the OpenGL context version to 4.6 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    // created the window and hide it until the program is ready to render the first frame
    window = SDL_CreateWindow("Test Window", 800, 450, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    if (!window)
    {
        SDL_Log("ERROR!: Failed to initalize SDL_Window! %s", SDL_GetError());
        return false;
    }
    SDL_Log("SUCCESS!: Successfully create SDL_Window!");

    // creates the opengl context
    ctx = SDL_GL_CreateContext(window);
    if (!ctx)
    {
        SDL_Log("ERROR!: Failed to create GL context! %s", SDL_GetError());
        return false;
    }
    SDL_Log("SUCCESS!: Successfully created the OpenGL context!");

    // initalizes glad and test if it was successfull
    int glVersion = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
    if (glVersion == 0)
    {
        SDL_Log("ERROR!: Failed to initialize GLAD! %s", glGetError());
        return false;
    }
    SDL_Log("SUCCESS!: Successfully initialized OpenGL!");

    // enables depth tests, sets the viewport to 0, 0, 800, 450, and the clear color to a sky blue
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 800, 450);
    glClearColor(0.4, 0.6, 1.0, 1.0);
    SDL_Log("SUCCESS!: Successfully set some values!");

    root = std::make_shared<Node>();
    root->name = "root";
    SDL_Log("SUCCESS!: Successfully set the root node's name to: ", root->name.c_str());

    Camera::GetInstance().init({.x = 0, .y = 0, .w = 800, .h = 450}, 60.0f);
    SDL_Log("SUCCESS!: Successfully initialied the Camera!");

    if (init)
    {
        if (!this->init())
        {
            return false;
        }
        SDL_Log("SUCCESS!: Successfully performed user initalisation!");
    }

    showWindow();

    running = true;

    return true;
}

std::shared_ptr<Node> Engine::get_root()
{
    return root;
}

bool Engine::is_running()
{
    return running;
}

void Engine::run()
{
    SDL_Log("ACTION PERFORMED: Engine has been ran");
    while (is_running())
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        SDL_Log(root->name.c_str());
        root->n_update(0.016f);
        SDL_Log("SUCCESS!: Successfully called n_update on the root node!");

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        SDL_Log("SUCCESS!: Successfully cleared the screen!");

        root->n_render(Camera::GetInstance().getProjectionViewMatrix());
        SDL_Log("SUCCESS!: Successfully called n_render(glm::mat4 pv)!");

        SDL_GL_SwapWindow(window);
        SDL_Log("SUCCESS!: Successfully swaped the window!");

        SDL_Delay(16);
    }
}

/// @brief Cleans up the application
void Engine::e_cleanup()
{
    if (cleanup)
    {
        this->cleanup();
    }

    SDL_DestroyWindow(window);
    SDL_GL_DestroyContext(ctx);
    SDL_Quit();
}

void Engine::showWindow()
{
    SDL_ShowWindow(window);
}

void Engine::hideWindow()
{
    SDL_HideWindow(window);
}

#endif
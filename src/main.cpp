#include <iostream>
#include <fstream>
#include <vector>

#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <glm/ext.hpp>

#include "engine.hpp"

int main(int _argc, char **_argv)
{
    Engine::GetInstance().init = [](void) -> bool
    {
        auto path = Engine::GetInstance().get_executable_location();
        auto root = Engine::GetInstance().get_root();
        SDL_Log("SUCCESS!: Successfully retrived the root node and executable path in user init!");

        // A pyrimid mesh
        auto p_mesh = std::make_shared<Mesh>(MeshTemplates::Pyrimid::vertices, MeshTemplates::Pyrimid::indices);
        p_mesh->name = "p_mesh";
        p_mesh->transform = glm::mat4(1.0f);
        p_mesh->shader.load(path + "assets/shaders/test.vs", path + "assets/shaders/test.fs");
        root->add_child(p_mesh);
        SDL_Log("SUCCESS!: Successfully created a pyrimid mesh and added it to the root node!");

        // // A square mesh
        auto s_mesh = std::make_shared<Mesh>(MeshTemplates::Square::vertices, MeshTemplates::Square::indices);
        s_mesh->name = "s_mesh";
        s_mesh->transform = glm::mat4(1.0f);
        s_mesh->shader.load(path + "assets/shaders/test.vs", path + "assets/shaders/test.fs");
        root->add_child(s_mesh);
        SDL_Log("SUCCESS!: Successfully created a square mesh and added it to the root node!");

        Camera::GetInstance().move_to(glm::vec3(2.0f, 2.0f, -2.0f));
        Camera::GetInstance().look_at(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        SDL_Log("SUCCESS!: Successfully offset the camera and made it look at 0, 0, 0!");

        return true;
    };

    Engine::GetInstance().cleanup = [](void)
    {
        // do nothing for now
    };

    // intializes the application and exists if the doesn't work
    if (!Engine::GetInstance().e_init())
    {
        SDL_Log("ERROR!: Failed to initialize the applicaiton!");
        Engine::GetInstance().e_cleanup();
        return -1;
    }

    Engine::GetInstance().run();

    Engine::GetInstance().e_cleanup();

    return 0;
}
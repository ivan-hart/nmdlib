#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <iostream>

#include "engine.hpp"

class Camera : public Singleton<Camera>
{
private:
    glm::mat4 projection;
    glm::mat4 view;

    struct ViewPort
    {
        unsigned int x, y, w, h;
    };
    ViewPort view_port;
public:
    void init(ViewPort n_view_port, float FOV);
    glm::mat4 getProjectionViewMatrix();

    // void move_to_at_speed(glm::vec3 new_pos, float speed, float dt);
    // void move_to_at_direction(glm::vec3 dir, float speed, float dt);
    void move_to(glm::vec3 new_pos);
    void look_at(glm::vec3 target, glm::vec3 up);
};

void Camera::init(ViewPort n_view_port, float FOV)
{
    view_port = n_view_port;
    projection = glm::perspective(glm::radians(FOV), (float)view_port.w / (float)view_port.h, 0.01f, 1000.0f);
    view = glm::mat4(1.0f);
}

glm::mat4 Camera::getProjectionViewMatrix()
{
    return projection * view;
}

void Camera::move_to(glm::vec3 new_pos)
{
    view[3] = glm::vec4(new_pos.x, new_pos.y, new_pos.z, 1.0f);
}

void Camera::look_at(glm::vec3 target, glm::vec3 up)
{
    glm::vec3 camera_transform = glm::vec3(view[3].x, view[3].y, view[3].z);
    view = glm::lookAt(camera_transform, target, up);
}

#endif
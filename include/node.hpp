#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>

#include <SDL3/SDL.h>
#include <glm/ext.hpp>

class Node
{
private:
    std::vector<std::shared_ptr<Node>> children;
public:
    std::string name;
    glm::mat4 transform;

    bool add_child(std::shared_ptr<Node> node);
    std::shared_ptr<Node> get_child(std::string name);

    void n_update(float dt);
    std::function<void(float)> update;
    
    void n_render(glm::mat4 pv);
    std::function<void(glm::mat4)> render;
};

inline bool Node::add_child(std::shared_ptr<Node> node)
{
    for (std::shared_ptr<Node> i : this->children)
    {
        if (node->name == i->name)
        {
            SDL_Log("ERROR!: Child already exists with name: %s", node->name.c_str());
            return false;
        }
    }

    SDL_Log("%s ADDED TO: %s", node->name.c_str(), name.c_str());
    children.push_back(node);
    return true;
}

inline std::shared_ptr<Node> Node::get_child(std::string name)
{
    for (std::shared_ptr<Node> i : this->children)
    {
        if (i->name == name)
        {
            return i;
        }
    }
    return NULL;
}

inline void Node::n_update(float dt)
{
    SDL_Log("ACTION PERFORMED: n_update called on node: %s", this->name.c_str());
    if (update)
    {
        this->update(dt);
    }
    for (auto i : this->children)
    {
        i->n_update(dt);
    }
}

inline void Node::n_render(glm::mat4 pv)
{
    if (render)
    {
        this->render(pv);
    }

    for (std::shared_ptr<Node> i : this->children)
    {
        i->n_render(pv);
    }
}

#endif
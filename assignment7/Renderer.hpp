//
// Created by goksu on 2/25/20.
//
#include "Scene.hpp"
#include "thread_pool.hpp"

#pragma once
struct hit_payload
{
    float tNear;
    uint32_t index;
    Vector2f uv;
    Object* hit_obj;
};

class Renderer
{
public:
    Renderer() :pool_(16) {}
    void Render(const Scene& scene);

private:
    ThreadPool pool_;
};

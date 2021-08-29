//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "Scene.hpp"


void Scene::buildBVH() {
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray& ray) const
{
    return this->bvh->Intersect(ray);
}

void Scene::sampleLight(Intersection& pos, float& pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()) {
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()) {
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum) {
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

bool Scene::trace(
    const Ray& ray,
    const std::vector<Object*>& objects,
    float& tNear, uint32_t& index, Object** hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vector2f uvK;
        if (objects[k]->intersect(ray, tNearK, indexK) && tNearK < tNear) {
            *hitObject = objects[k];
            tNear = tNearK;
            index = indexK;
        }
    }


    return (*hitObject != nullptr);
}

// Implementation of Path Tracing
Vector3f Scene::castRay(const Ray& ray, int depth) const
{
    // TO DO Implement Path Tracing Algorithm here
    Intersection inter = intersect(ray);
    if (!inter.happened) return Vector3f();

    if (inter.obj->hasEmit()) return inter.m->getEmission();

    Vector3f L_dir, L_indir;

    Material* m = inter.m;


    const auto& x = inter.coords; // 反射位置
    const auto& n = inter.normal.normalized(); // 反射位置的法线


    float pdf;
    Intersection inter2;
    sampleLight(inter2, pdf);

    const auto& Li = inter2.emit; // 物体发光
    const auto& xx = inter2.coords; // 光源采样位置
    const auto& nn = inter2.normal.normalized(); // 光源采样位置位置的法线

    const auto xp = xx - x;
    const auto ws = normalize(xp); // 采样点到p的入射方向
    const auto wo = normalize(-ray.direction); // 出射方向

    const Ray local_ray(x, ws);
    const auto local_inter = intersect(local_ray);
    // 当前点和采样的光源之间没有碰撞, 也就是碰撞到了光源
    if (fabs((local_inter.coords - xx).norm()) < 0.001)
    {
        L_dir = Li
            * m->eval(wo, ws, n) // BRDF
            * dotProduct(ws, n) // 入射光线和反射位置的夹角
            * dotProduct(-ws, nn) // 入射光线和光源采样位置的夹角
            / (dotProduct(xp, xp) * pdf);
    }

    const auto P_RR = get_random_float();
    if (P_RR < RussianRoulette)
    {
        const auto wi = m->sample(wo, n).normalized();
        const Ray r(x, wi);
        inter = intersect(ray);
        if (inter.happened && !inter.obj->hasEmit()) // 碰撞到物体
        {
            pdf = m->pdf(wi, wo, n);
            if (pdf > 0.0001) {
                L_indir = castRay(r, depth + 1)
                    * m->eval(wi, wo, n) // BRDF;
                    * dotProduct(wi, n)
                    / pdf / RussianRoulette;
            }
        }
    }
    return  L_dir + L_indir;

}
//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "Scene.hpp"


void Scene::buildBVH() {
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}


void Scene::buildBVH(BVHAccel::SplitMethod split) {
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, split);
}

Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

void Scene::sampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum){
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

bool Scene::trace(
        const Ray &ray,
        const std::vector<Object*> &objects,
        float &tNear, uint32_t &index, Object **hitObject)
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
Vector3f Scene::castRay(const Ray &ray, int depth) const
{
    // TO DO Implement Path Tracing Algorithm here
    Intersection intersection = intersect(ray);
    
    //deal with light source
    if(intersection.emit.norm() > 0.0)
        return Vector3f(1);
    if(!intersection.happened)
        return Vector3f(0);
        
    Vector3f wo = normalize(-ray.direction);
    Vector3f p = intersection.coords;
    Vector3f N = normalize(intersection.normal);

    float pdf_light;
    Intersection inter;
    sampleLight(inter,pdf_light);
    Vector3f x = inter.coords;
    Vector3f ws = normalize(x-p);
    Vector3f NN = normalize(inter.normal);
    
    Vector3f L_dir = Vector3f(0);
    if((intersect(Ray(p,ws)).coords - x).norm() < 0.01)
    {
        L_dir = inter.emit * intersection.m->eval(wo,ws,N)*dotProduct(ws,N) * dotProduct(-ws,NN) / (((x-p).norm()* (x-p).norm()) * pdf_light);
    }

    Vector3f L_indir = Vector3f(0);
    if(get_random_float() < RussianRoulette)
    {
        Vector3f wi = intersection.m->sample(wo,N);
        L_indir = castRay(Ray(p,wi),depth) * intersection.m->eval(wi,wo,N) * dotProduct(wi,N) / (intersection.m->pdf(wi,wo,N) * RussianRoulette);
    }

    return L_indir + L_dir;
}
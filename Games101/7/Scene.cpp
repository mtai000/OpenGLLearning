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

    Vector3f light = Vector3f(0);
    //deal with light source
    if(intersection.emit.norm() > 0.0)
        light = intersection.m->getEmission() ;
    if(!intersection.happened)
        return Vector3f(0);
    
    Vector3f L_dir = Vector3f(0);
    Vector3f L_indir = Vector3f(0);

    Vector3f wo = normalize(-ray.direction);
    Vector3f p = intersection.coords;
    Vector3f N = normalize(intersection.normal);

   
    switch (intersection.m->getType())
    {
        case MIRROR:
        {
            if(get_random_float() < RussianRoulette)
            {
                //反射光方向
                Vector3f reflectDir = intersection.m->sample(ray.direction,N).normalized();
                //定义反射光ray
                Ray reflectRay(intersection.coords,reflectDir);
                // 求反射光交点
                Intersection nextInter = intersect(reflectRay);
                if(nextInter.happened)
                {
                    auto reflect_wo = (-reflectRay.direction).normalized();
                    auto t_pdf = intersection.m->pdf(ray.direction,reflectDir,N);
                    if(t_pdf > EPSILON)
                    {
                        // 镜面材质计算
                        auto mirrorEval = intersection.m->eval(ray.direction, reflectDir,N);
                        auto dot = dotProduct(reflectDir,N);
                        L_indir = castRay( reflectRay ,depth + 1) * mirrorEval
                                    * dot
                                    / (t_pdf * RussianRoulette);
                    }
                }
            }
            break;
        }
        default:
        {
            float pdf_light;
            Intersection inter;
            sampleLight(inter,pdf_light);
            Vector3f x = inter.coords;
            Vector3f ws = normalize(x-p);
            Vector3f NN = normalize(inter.normal);
            
            if((intersect(Ray(p,ws)).coords - x).norm() < 0.01)
            {
                L_dir = inter.emit * intersection.m->eval(wo,ws,N)*dotProduct(ws,N) * dotProduct(-ws,NN) / (((x-p).norm()* (x-p).norm()) * pdf_light);
            }

            if(get_random_float() < RussianRoulette)
            {
                Vector3f wi = intersection.m->sample(wo,N).normalized();
                auto t_pdf = intersection.m->pdf(wo,wi,N);
                if(t_pdf > EPSILON)
                    L_indir = castRay(Ray(p,wi),depth + 1) * intersection.m->eval(wi,wo,N) * dotProduct(wi,N) / (t_pdf * RussianRoulette);
            }
            break;
        }
    }

    
   
    auto hitColor = L_indir + L_dir + light;
    hitColor.x = (clamp(0,1,hitColor.x));
    hitColor.y = (clamp(0,1,hitColor.y));
    hitColor.z = (clamp(0,1,hitColor.z));

    return hitColor;
}
//
// Created by LEI XU on 5/16/19.
//

#ifndef RAYTRACING_MATERIAL_H
#define RAYTRACING_MATERIAL_H

#include "Vector.hpp"

enum MaterialType { DIFFUSE , MIRROR , MICROFACE};

class Material{
private:

    // Compute reflection direction
    Vector3f reflect(const Vector3f &I, const Vector3f &N) const
    {
        return l = I - 2 * dotProduct(I, N) * N;
    }

    // Compute refraction direction using Snell's law
    //
    // We need to handle with care the two possible situations:
    //
    //    - When the ray is inside the object
    //
    //    - When the ray is outside.
    //
    // If the ray is outside, you need to make cosi positive cosi = -N.I
    //
    // If the ray is inside, you need to invert the refractive indices and negate the normal N
    //
    // 折射定律
    Vector3f refract(const Vector3f &I, const Vector3f &N, const float &ior) const
    {
        // cosi,夹角
        float cosi = clamp(-1, 1, dotProduct(I, N));
        // ior折射率
        float etai = 1, etat = ior;
        Vector3f n = N;

        //  cosi < 0时，光从介质中射出， cosi求正值，面法线不变
        if (cosi < 0) { cosi = -cosi; } 
        // 否则， 光从介质外射入介质， 法线反向
        else { std::swap(etai, etat); n= -N; }

        // 出射折射率/入射折射率 
        float eta = etai / etat;
        
        // k <= 0 发生反射
        float k = 1 - eta * eta * (1 - cosi * cosi);
        return k < 0 ? 0 : eta * I + (eta * cosi - sqrtf(k)) * n;
    }

    // Compute Fresnel equation
    //
    // \param I is the incident view direction
    //
    // \param N is the normal at the intersection point
    //
    // \param ior is the material refractive index
    //
    // \param[out] kr is the amount of light reflected
    //
    void fresnel(const Vector3f &I, const Vector3f &N, const float &ior, float &kr) const
    {
        float cosi = clamp(-1, 1, dotProduct(I, N));
        float etai = 1, etat = ior;
        if (cosi > 0) {  std::swap(etai, etat); }
        // Compute sini using Snell's law
        float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
        // Total internal reflection
        if (sint >= 1) {
            kr = 1;
        }
        else {
            float cost = sqrtf(std::max(0.f, 1 - sint * sint));
            cosi = fabsf(cosi);
            float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
            float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
            kr = (Rs * Rs + Rp * Rp) / 2;
        }
        // As a consequence of the conservation of energy, transmittance is given by:
        // kt = 1 - kr;
    }


    // a光线的方向，相对坐标 ， N法线
    // 证明 ???
    Vector3f toWorld(const Vector3f &a, const Vector3f &N){
        Vector3f B, C;
        
        if (std::fabs(N.x) > std::fabs(N.y)){
            float invLen = 1.0f / std::sqrt(N.x * N.x + N.z * N.z);
            C = Vector3f(N.z * invLen, 0.0f, -N.x *invLen);
        }
        else {
            float invLen = 1.0f / std::sqrt(N.y * N.y + N.z * N.z);
            C = Vector3f(0.0f, N.z * invLen, -N.y *invLen);
        }
        B = crossProduct(C, N);
        return a.x * B + a.y * C + a.z * N;
    }

public:
    MaterialType m_type;
    //Vector3f m_color;
    Vector3f m_emission;
    float ior;
    Vector3f Kd, Ks;
    float Roughness;
    float specularExponent;
    //Texture tex;

    inline Material(MaterialType t=DIFFUSE, Vector3f e=Vector3f(0,0,0));
    inline MaterialType getType();
    //inline Vector3f getColor();
    inline Vector3f getColorAt(double u, double v);
    inline Vector3f getEmission();
    inline bool hasEmission();

    // sample a ray by Material properties
    inline Vector3f sample(const Vector3f &wi, const Vector3f &N);
    // given a ray, calculate the PdF of this ray
    inline float pdf(const Vector3f &wi, const Vector3f &wo, const Vector3f &N);
    // given a ray, calculate the contribution of this ray
    inline Vector3f eval(const Vector3f &wi, const Vector3f &wo, const Vector3f &N);

};

Material::Material(MaterialType t, Vector3f e){
    m_type = t;
    //m_color = c;
    m_emission = e;
}

MaterialType Material::getType(){return m_type;}
///Vector3f Material::getColor(){return m_color;}
Vector3f Material::getEmission() {return m_emission;}
bool Material::hasEmission() {
    if (m_emission.norm() > EPSILON) return true;
    else return false;
}

Vector3f Material::getColorAt(double u, double v) {
    return Vector3f();
}

//采样   N法线
Vector3f Material::sample(const Vector3f &wi, const Vector3f &N){
    switch(m_type){
        case DIFFUSE:
        {
            // uniform sample on the hemisphere
            // 获得2个随机数
            float x_1 = get_random_float(), x_2 = get_random_float();

            // 反射光随机Z坐标 [-1,1]
            float z = std::fabs(1.0f - 2.0f * x_1);
            
            // 半径1的球的z高度截面圆半径值
            float r = std::sqrt(1.0f - z * z);

            // 截面指定角度， 得出x , y 坐标，与z确定出射光方向
            float phi = 2 * M_PI * x_2;
            Vector3f localRay(r*std::cos(phi), r*std::sin(phi), z);

            //将本地坐标转为世界坐标
            return toWorld(localRay, N);
            
            break;
        }
        case MIRROR:
        {
            //镜面反射入射光
            return reflect(wi,N); 
            break;
        }
        case MICROFACE:
        {
            // uniform sample on the hemisphere
            // 获得2个随机数
            float x_1 = get_random_float(), x_2 = get_random_float();

            // 反射光随机Z坐标 [-1,1]
            float z = std::fabs(1.0f - 2.0f * x_1);
            
            // 半径1的球的z高度截面圆半径值
            float r = std::sqrt(1.0f - z * z);

            // 截面指定角度， 得出x , y 坐标，与z确定出射光方向
            float phi = 2 * M_PI * x_2;
            Vector3f localRay(r*std::cos(phi), r*std::sin(phi), z);

            //将本地坐标转为世界坐标
            return toWorld(localRay, N);
            
            break;
        }
    }
    return Vector3f(0.f);
}

//probability density functions
float Material::pdf(const Vector3f &wi, const Vector3f &wo, const Vector3f &N){
    switch(m_type){
        case DIFFUSE:
        {
            // uniform sample probability 1 / (2 * PI)
            if (dotProduct(wo, N) > 0.0f)
                return 0.5f / M_PI;
            else
                return 0.0f;
            break;
        }
        case MIRROR:
        {
            if(dotProduct(wo,N) > 0.0f)
                return 1.0f;
            else
                return 0.0f;
            break;
        }
        case MICROFACE:
        {
            if (dotProduct(wo, N) > 0.0f)
                return 0.5f / M_PI;
            else
                return 0.0f;
            break;
        }
    }
}

Vector3f Material::eval(const Vector3f &wi, const Vector3f &wo, const Vector3f &N){
    switch(m_type){
        case DIFFUSE:
        {
            // calculate the contribution of diffuse model
            float cosalpha = dotProduct(wo, N);
            if (cosalpha > 0.0f) {
                Vector3f diffuse = Kd / M_PI;
                return diffuse;
            }
            else
                return Vector3f(0.0f);
            break;
        }
        case MIRROR:
        {
            float cosalpha = dotProduct(wo, N);
            float kr;
            if (cosalpha > EPSILON) {
                fresnel(wi,N,ior,kr);
                return kr / cosalpha;
            }
            else
                return Vector3f(0.0f);
            break;
        }
        case MICROFACE:   //202
        {
            float cosalpha = dotProduct(wo, N);
            if (cosalpha > 0.0f) 
            {
                float F,G,D;

                fresnel(wi,N,ior,F);
               
                float G_wi,G_wo;
                G_wi = (-1 + sqrt(Roughness * Roughness * pow(tan(acos(dotProduct(wi,N))),2)))/2;
                G_wo = (-1 + sqrt(Roughness * Roughness * pow(tan(acos(dotProduct(wo,N))),2)))/2;
                float div = 1 + G_wi + G_wo;
                G = div < EPSILON ? 1.f : 1.f / div;

                Vector3f h = (wo-wi).normalized();
                float cos_sita = dotProduct(h,N);
                float div_D = (M_PI * pow(1.0 + cos_sita * cos_sita * (Roughness * Roughness - 1), 2));
                D = div_D < EPSILON ? 1.f : Roughness * Roughness / div_D;

                Vector3f diffuse = (Vector3f(1.f) - F) * Kd /M_PI;
                
                float div_s = 4 * dotProduct(N ,-wi) * dotProduct(N,wo);
                Vector3f specular = div_s < EPSILON ? Vector3f(1) : F*G*D/div_s;
                return diffuse + specular;
            }
            else
                return Vector3f(0.0f);
            break;
        }
    }
}

#endif //RAYTRACING_MATERIAL_H

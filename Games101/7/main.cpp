#include "Renderer.hpp"
#include "Scene.hpp"
#include "Triangle.hpp"
#include "Sphere.hpp"
#include "Vector.hpp"
#include "global.hpp"
#include <chrono>

// In the main function of the program, we create the scene (create objects and
// lights) as well as set the options for the render (image width and height,
// maximum recursion depth, field-of-view, etc.). We then call the render
// function().
int g_size = 512;
int g_spp = 4;
//
int main(int argc, char** argv)
{ 
    if (argc > 1)
    {
        g_size = stringToInt(argv[1]);
        g_spp = stringToInt(argv[2]);
    }

    // Change the definition here to change resolution
    Scene scene(g_size, g_size);
    
    Material* red = new Material(DIFFUSE, Vector3f(0.0f));
    red->Kd = Vector3f(0.63f, 0.065f, 0.05f);
    Material* green = new Material(DIFFUSE, Vector3f(0.0f));
    green->Kd = Vector3f(0.14f, 0.45f, 0.091f);
    Material* white = new Material(DIFFUSE, Vector3f(0.0f));
    white->Kd = Vector3f(0.725f, 0.71f, 0.68f);
    Material* light = new Material(DIFFUSE, (8.0f * Vector3f(0.747f+0.058f, 0.747f+0.258f, 0.747f) + 15.6f * Vector3f(0.740f+0.287f,0.740f+0.160f,0.740f) + 18.4f *Vector3f(0.737f+0.642f,0.737f+0.159f,0.737f)));
    light->Kd = Vector3f(0.65f);


    // 镜面
    Material* mirror = new Material(MIRROR , Vector3f(0.0f));
    mirror->Kd = Vector3f(0.0,0.0,0.0);
    mirror->ior = 40.f;

    // 微表面
    Material* microface = new Material(MICROFACE , Vector3f(0.f));
    microface->Ks = Vector3f(0.65, 0.65, 0.65);
    microface->Kd = Vector3f(0.3, 0.3, 0.3);
    microface->ior = 0.5;
    microface->Roughness = 0;

    Sphere sphere(Vector3f(200, 100, 200), 100, mirror);

    MeshTriangle floor("../models/cornellbox/floor.obj", white);
    MeshTriangle shortbox("../models/cornellbox/shortbox.obj", white);
    MeshTriangle tallbox("../models/cornellbox/tallbox.obj", white);
    MeshTriangle left("../models/cornellbox/left.obj", red);
    MeshTriangle right("../models/cornellbox/right.obj", green);
    MeshTriangle light_("../models/cornellbox/light.obj", light);

    scene.Add(&floor);
    //scene.Add(&shortbox);
    scene.Add(&tallbox);
    scene.Add(&left);
    scene.Add(&right);
    scene.Add(&light_);
    scene.Add(&sphere);

    
    std::cout << argc << std::endl;
    if(argc == 4)
    {
        std::cout << "BVH" << std::endl;
        std::string para = argv[3];
        std::transform(para.begin(),para.end(),para.begin(),::toupper);
        if(para == "BVH")
            scene.buildBVH();
    }
    else
    {
        std::cout << "SAH" << std::endl;
        scene.buildBVH(BVHAccel::SplitMethod::SAH);
    }
    Renderer r;

    auto start = std::chrono::system_clock::now();
    r.Render(scene);
    auto stop = std::chrono::system_clock::now();

    std::cout << "Render complete: \n";
    std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::hours>(stop - start).count() << " hours\n";
    std::cout << "          : " << std::chrono::duration_cast<std::chrono::minutes>(stop - start).count() << " minutes\n";
    std::cout << "          : " << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " seconds\n";

    return 0;
}

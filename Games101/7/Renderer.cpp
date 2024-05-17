//
// Created by goksu on 2/25/20.
//

#include <fstream>
#include <thread>
#include <mutex>
#include "Renderer.hpp"
#include <omp.h>

inline float deg2rad(const float& deg) { return deg * M_PI / 180.0; }

#define SHOWPROGRESS true
#define USE_MULTI_THREAD false
#define USE_OPENMP false
const float EPSILON = 0.00001;
std::mutex g_mutex;
int totals = 0;
omp_lock_t lock;

void render_thread(const Scene& scene, std::vector<Vector3f>& framebuffer, int spp, int y_beg, int y_end)
{
	float scale = tan(deg2rad(scene.fov * 0.5));
	float imageAspectRatio = scene.width / (float)scene.height;
	Vector3f eye_pos(278, 273, -800);
	int m = 0;
	// change the spp value to change sample ammount

	for (uint32_t j = y_beg; j < y_end; ++j) {
		for (uint32_t i = 0; i < scene.width; ++i) {
			// generate primary ray direction
			m = j * scene.width + i;
			
			#ifdef USE_OPENMP
			#pragma omp parallel for
			#endif
			for (int k = 0; k < spp; k++) {
				float x = (2 * (i + get_random_float()) / (float)scene.width - 1) *
					imageAspectRatio * scale;
				float y = (1 - 2 * (j + get_random_float()) / (float)scene.height) * scale;
				Vector3f dir = normalize(Vector3f(-x, y, 1));
				framebuffer[m] += scene.castRay(Ray(eye_pos, dir), 0) / spp;
			}

			m++;
		}
		if(SHOWPROGRESS)
		if (!USE_OPENMP) {
			g_mutex.lock();
			totals++;
			UpdateProgress(totals / (float)scene.height);
			g_mutex.unlock();
		}
		else
		{
			omp_set_lock(&lock);
			totals++;
			UpdateProgress(totals / (float)scene.height);
			omp_unset_lock(&lock);
		}
	}
}

// The main render function. This where we iterate over all pixels in the image,
// generate primary rays and cast these rays into the scene. The content of the
// framebuffer is saved to a file.
void Renderer::Render(const Scene& scene)
{
	std::vector<Vector3f> framebuffer(scene.width * scene.height);

	int numThreads = std::thread::hardware_concurrency();
	int lines = scene.height / numThreads + 1;
	std::vector<std::thread> jobs;

	std::cout << "Size = " << g_size << std::endl;
	if (USE_MULTI_THREAD && !USE_OPENMP)
	{
		std::cout << "USE Multi Thread = " << numThreads << std::endl;
		std::cout << "SPP = " << g_spp << std::endl;
		
		for (int i = 0; i < numThreads; i++)
		{
			int y_begin = i * lines;
			int y_end = std::min(y_begin + lines, scene.height);

			jobs.push_back(std::thread(render_thread, std::ref(scene), std::ref(framebuffer), g_spp, y_begin, y_end));
		}
		for (int i = 0; i < jobs.size(); i++)
			jobs[i].join();

		UpdateProgress(1.f);
	}
	else 
	{
		#ifdef USE_OPENMP
		if(USE_OPENMP)
		std::cout << "USE OPENMP" << std::endl;
		#endif
		render_thread(scene, framebuffer, g_spp, 0, scene.height);
	}

	// save framebuffer to file
	FILE* fp = fopen("binary.ppm", "wb");
	(void)fprintf(fp, "P6\n%d %d\n255\n", scene.width, scene.height);
	for (auto i = 0; i < scene.height * scene.width; ++i) {
		static unsigned char color[3];
		color[0] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].x), 0.6f));
		color[1] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].y), 0.6f));
		color[2] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].z), 0.6f));
		fwrite(color, 1, 3, fp);
	}
	fclose(fp);
}

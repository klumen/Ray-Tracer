#include <iostream>
#include <chrono>

#include "Model.h"
#include "Light.h"
#include "Camera.h"
#include "Scene.h"
#include "Render.h"

int main()
{
	std::cout << "Hello RayTracer!" << std::endl;

	std::cout << "Loading scene..." << std::endl;
	std::string rootPath = "D:/Assets/Shader/wuqie/", fileName = "wuqie.obj";
	Model wuqie(rootPath, fileName);
	rootPath = "D:/Assets/Shader/", fileName = "plane.obj";
	Model floor(rootPath, fileName);

	AmbientLight aLight(Vec3(0.1f));
	DirectLight dLight(Vec3(0.5f), Vec3(-1.f));
	PointLight pLight(Vec3(10.f), Vec3(10.f));
	SpotLight sLight(Vec3(10.f), Vec3(10.f), Vec3(-1.f));

	unsigned int w = 800u, h = 600u;
	Camera camera;
	camera.pos = Vec3(0.f, 2.f, 10.f);

	Scene scene;
	scene.load_model(&wuqie);
	scene.load_model(&floor);
	scene.load_light(&aLight);
	scene.load_light(&pLight);
	scene.set_camera(&camera);

	std::cout << "Rendering..." << std::endl;
	
	RayTracer r(w, h);
	r.load_scene(&scene);

	auto startTime = std::chrono::steady_clock::now();
	r.ray_trace();
	auto endtTime = std::chrono::steady_clock::now();
	std::cout << "Time: " << std::chrono::duration<float>(endtTime - startTime) << std::endl;

	std::cout << "Generating PPM..." << std::endl;
	rootPath = "D:/", fileName = "RayTracer.ppm";
	r.generate_PPM(rootPath, fileName);

	std::cout << "Success!" << std::endl;

	return 0;
}
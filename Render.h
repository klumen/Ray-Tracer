#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>
#include <random>

#include "Vector.h"
#include "Matrix.h"
#include "Scene.h"

const float PI = acos(-1.f);

class RayTracer
{
public:
	RayTracer();
	RayTracer(unsigned int width, unsigned int height);
	~RayTracer();

	void load_scene(Scene* scene);
	float random();
	void ray_trace();
	void clear_buffer();
	void build_BVH();
	void shade();
	Vec3 shade_ray(const Ray& ray, float t0, float t1);
	void generate_PPM(const std::string& rootPath, const std::string& fileName);

	unsigned int width, height;

private:
	Scene* scene;
	std::vector<Vec3> frameBuf;

};
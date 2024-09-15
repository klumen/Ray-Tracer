#pragma once

#include <vector>

#include "Vector.h"
#include "Model.h"
#include "Light.h"
#include "Camera.h"
#include "Surface.h"

class Scene
{
public:
	Scene();
	Scene(const Scene& s);
	Scene(Scene&& s) noexcept;
	~Scene();

	Scene& operator=(const Scene& s);
	Scene& operator=(Scene&& s) noexcept;

	void load_model(Model* model);
	void load_light(Light* light);
	void set_camera(Camera* camera);
	HitRecord hit(const Ray& r, float t0, float t1) const;

	std::vector<Model*> models;
	std::vector<Light*> lights;
	Camera* camera;

private:

};
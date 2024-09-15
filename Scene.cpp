#include "Scene.h"

Scene::Scene() : camera(nullptr) {}
Scene::Scene(const Scene& s) : models(s.models), lights(s.lights), camera(s.camera) {}
Scene::Scene(Scene&& s) noexcept : models(std::move(s.models)), lights(std::move(s.lights)), camera(camera) {}
Scene::~Scene() {}

Scene& Scene::operator=(const Scene& s)
{
	if (this != &s)
	{
		models = s.models, lights = s.lights;
		camera = s.camera;
	}

	return *this;
}
Scene& Scene::operator=(Scene&& s) noexcept
{
	if (this != &s)
	{
		models = std::move(s.models), lights = std::move(s.lights);
		camera = s.camera;
	}

	return *this;
}

void Scene::load_model(Model* model) { models.emplace_back(model); }
void Scene::load_light(Light* light) { lights.emplace_back(light); }
void Scene::set_camera(Camera* camera) { this->camera = camera; }
HitRecord Scene::hit(const Ray& r, float t0, float t1) const
{
	HitRecord res;

	for (auto& model : models)
	{
		HitRecord trec = model->hit(r, t0, t1);
		if (trec.t < std::numeric_limits<float>::infinity())
		{
			res = trec;
			t1 = trec.t;
		}
	}

	return res;
}
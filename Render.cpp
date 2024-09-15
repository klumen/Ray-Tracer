#include "Render.h"

RayTracer::RayTracer() : width(0u), height(0u), scene(nullptr) {}
RayTracer::RayTracer(unsigned int width, unsigned int height) : width(width), height(height), scene(nullptr) { frameBuf.resize((size_t)width * height); }
RayTracer::~RayTracer() {}

void RayTracer::load_scene(Scene* scene) { this->scene = scene; }
float RayTracer::random()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.f, 1.f);

	return dis(gen);
}
void RayTracer::ray_trace()
{
	clear_buffer();

	build_BVH();

	shade();
}
void RayTracer::clear_buffer()
{
	std::fill(frameBuf.begin(), frameBuf.end(), Vec3(0.f));
}
void RayTracer::build_BVH()
{
	for (auto& model : scene->models)
		for (auto& mesh : model->meshes)
		{
			std::vector<Triangle*> tris(mesh.tris.size());
			for (unsigned int i = 0; i < tris.size(); i++)
				tris[i] = &mesh.tris[i];

			mesh.BVH = BVHnode::create_node(tris, 0u);
		}
}
void RayTracer::shade()
{
	Vec3 w = -scene->camera->view.normalized();
	Vec3 u = scene->camera->up.cross(w).normalized();
	Vec3 v = w.cross(u);
	float near = scene->camera->near, far = scene->camera->far;
	float bottom = tan(scene->camera->FoVy / 2.f / 180.f * PI) * near, top = -bottom;
	float right = top * scene->camera->aspect, left = -right;

	for (unsigned int y = 0; y < height; y++)
		for (unsigned int x = 0; x < width; x++)
		{
			unsigned int ind = y * width + x;
			float process = (ind + 1.f) / float(width * height) * 100.f;
			std::cout << "\rProcess: " << std::fixed << std::setprecision(2) << process << "%";
			
			Vec3 color;
			float dx[4] = { 0.25f, 0.75f, 0.25f, 0.75f }, dy[4] = { 0.25f, 0.25f, 0.75f, 0.75f };
			for (unsigned int i = 0u; i < 4u; i++)
			{
				float tu = left + (float(x) + dx[i]) / float(width) * (right - left);
				float tv = bottom + (float(y) + dy[i]) / float(height) * (top - bottom);
				Ray ray;
				ray.o = scene->camera->pos;
				ray.d = (near * w + tu * u + tv * v).normalized();

				color += 0.25f * shade_ray(ray, 0.f, std::numeric_limits<float>::infinity());
			}
			frameBuf[ind] = color;
		}

	std::cout << std::endl;
}
Vec3 RayTracer::shade_ray(const Ray& ray, float t0, float t1)
{
	Vec3 color;
	HitRecord rec = scene->hit(ray, t0, t1);
	
	if (rec.t < std::numeric_limits<float>::infinity())
	{
		for (auto& light : scene->lights)
			color += light->illuminate(ray, rec, *scene);

		float P_RR = 0.8f;
		if (random() < P_RR)
		{
			Vec3 p = rec.position;
			Vec3 r = ray.d - 2 * (ray.d.dot(rec.normal)) * rec.normal;
			color += Vec3(0.1f) * shade_ray(Ray(p, r), 1e-6f, std::numeric_limits<float>::infinity()) / P_RR;
		}
	}

	return color;
}
void RayTracer::generate_PPM(const std::string& rootPath, const std::string& fileName)
{
	std::ofstream res(rootPath + fileName, std::ios::out);

	res << "P3\n" << width << " " << height << "\n" << "255" << "\n";
	for (size_t y = height; y > 0; y--)
	{
		for (size_t x = 0; x < width; x++)
		{
			size_t ind = (y - 1) * width + x;

			res << unsigned int(std::clamp(frameBuf[ind].x, 0.f, 1.f) * 255) << " ";
			res << unsigned int(std::clamp(frameBuf[ind].y, 0.f, 1.f) * 255) << " ";
			res << unsigned int(std::clamp(frameBuf[ind].z, 0.f, 1.f) * 255) << "\n";
		}
	}

	res.close();
}
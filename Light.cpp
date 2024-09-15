#include "Light.h"
#include "Scene.h"

Light::Light() {}
Light::~Light() {}

AmbientLight::AmbientLight() {}
AmbientLight::AmbientLight(const Vec3& i) : intensity(i) {}
AmbientLight::AmbientLight(const AmbientLight& light) : intensity(light.intensity) {}
AmbientLight::~AmbientLight() {}

AmbientLight& AmbientLight::operator=(const AmbientLight& light)
{
	if (&light != this)
	{
		intensity = light.intensity;
	}

	return *this;
}

Vec3 AmbientLight::illuminate(const Ray& ray, const HitRecord& hrec, const Scene& scene) const
{
	Vec3 k = hrec.s->mtl->Ka;
	if (hrec.s->mtl->map_Kd != nullptr)
	{
		k *= hrec.s->mtl->map_Kd->bilinear_interpolate(hrec.texCoord);
	}

	return k * intensity;
}


DirectLight::DirectLight() {}
DirectLight::DirectLight(const Vec3& i, const Vec3& dir) : intensity(i), dir(dir) {}
DirectLight::DirectLight(const DirectLight& light) : intensity(light.intensity), dir(light.dir) {}
DirectLight::~DirectLight() {}

DirectLight& DirectLight::operator=(const DirectLight& light)
{
	if (&light != this)
	{
		intensity = light.intensity;
		dir = light.dir;
	}

	return *this;
}

Vec3 DirectLight::illuminate(const Ray& ray, const HitRecord& hrec, const Scene& scene) const
{
	Vec3 p = hrec.position;
	Vec3 l = -dir.normalized();

	HitRecord srec = scene.hit(Ray(p, l), 1e-6f, std::numeric_limits<float>::infinity());
	if (srec.t == std::numeric_limits<float>::infinity())
		return Vec3(0.f);

	Vec3 n = hrec.normal;
	Vec3 E = std::max(l.dot(n), 0.f) * intensity;
	Vec3 k = hrec.s->mtl->evaluate(l, -ray.d, n, hrec.texCoord);

	return k * E;
}


PointLight::PointLight() {}
PointLight::PointLight(const Vec3& i, const Vec3& pos) : intensity(i), pos(pos) {}
PointLight::PointLight(const PointLight& light) : intensity(light.intensity), pos(light.pos) {}
PointLight::~PointLight() {}

PointLight& PointLight::operator=(const PointLight& light)
{
	if (this != &light)
	{
		intensity = light.intensity;
		pos = light.pos;
		constant = light.constant, linear = light.linear, quadratic = light.quadratic;
	}

	return *this;
}

Vec3 PointLight::illuminate(const Ray& ray, const HitRecord& hrec, const Scene& scene) const
{
	Vec3 p = hrec.position;
	float r = (pos - p).norm();
	Vec3 l = (pos - p) / r;

	HitRecord srec = scene.hit(Ray(p, l), 1e-6f, r);
	if (srec.t < std::numeric_limits<float>::infinity())
		return Vec3(0.f);

	Vec3 n = hrec.normal;
	float attenuation = 1.f / (constant + linear * r + quadratic * r * r);

	Vec3 E = std::max(l.dot(n), 0.f) * attenuation * intensity;

	Vec3 k = hrec.s->mtl->evaluate(l, -ray.d, n, hrec.texCoord);

	return k * E;
}


SpotLight::SpotLight() {}
SpotLight::SpotLight(const Vec3& i, const Vec3& pos, const Vec3& dir) : intensity(i), pos(pos), dir(dir) {}
SpotLight::SpotLight(const SpotLight& light) : intensity(light.intensity), pos(light.pos), dir(light.dir) {}
SpotLight::~SpotLight() {}

SpotLight& SpotLight::operator=(const SpotLight& light)
{
	if (this != &light)
	{
		intensity = light.intensity;
		pos = light.pos;
		dir = light.dir;
		cutoff = light.cutoff, outCutoff = light.outCutoff;
		constant = light.constant, linear = light.linear, quadratic = light.quadratic;
	}

	return *this;
}

Vec3 SpotLight::illuminate(const Ray& ray, const HitRecord& hrec, const Scene& scene) const
{
	Vec3 p = hrec.position;
	float r = (pos - p).norm();
	Vec3 l = (pos - p) / r;

	HitRecord srec = scene.hit(Ray(p, l), 1e-6f, r);
	if (srec.t == std::numeric_limits<float>::infinity())
		return Vec3(0.f);

	Vec3 n = hrec.normal;
	float attenuation = 1.f / (constant + linear * r + quadratic * r * r);

	float theta = dir.normalized().dot(-l);
	float epsilon = cutoff - outCutoff;
	attenuation *= std::clamp((theta - outCutoff) / epsilon, 0.f, 1.f);

	Vec3 E = std::max(l.dot(n), 0.f) * attenuation * intensity;

	Vec3 k = hrec.s->mtl->evaluate(l, -ray.d, n, hrec.texCoord);

	return k * E;
}
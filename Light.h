#pragma once

#include "Vector.h"
#include "Surface.h"

class Scene;

class Light
{
public:
	Light();
	virtual ~Light() = 0;

	virtual Vec3 illuminate(const Ray& ray, const HitRecord& hrec, const Scene& scene) const = 0;

private:

};

class AmbientLight : public Light
{
public:
	AmbientLight();
	AmbientLight(const Vec3& i);
	AmbientLight(const AmbientLight& light);
	~AmbientLight();

	AmbientLight& operator=(const AmbientLight& light);

	Vec3 illuminate(const Ray& ray, const HitRecord& hrec, const Scene& scene) const override;

	Vec3 intensity;

private:

};

class DirectLight : public Light
{
public:
	DirectLight();
	DirectLight(const Vec3& i, const Vec3& dir);
	DirectLight(const DirectLight& light);
	~DirectLight();

	DirectLight& operator=(const DirectLight& light);

	Vec3 illuminate(const Ray& ray, const HitRecord& hrec, const Scene& scene) const override;

	Vec3 intensity;
	Vec3 dir;

private:

};

class PointLight : public Light
{
public:
	PointLight();
	PointLight(const Vec3& i, const Vec3& pos);
	PointLight(const PointLight& light);
	~PointLight();

	PointLight& operator=(const PointLight& light);

	Vec3 illuminate(const Ray& ray, const HitRecord& hrec, const Scene& scene) const override;

	Vec3 intensity;
	Vec3 pos;

	float constant = 1.f;
	float linear = 0.09f;
	float quadratic = 0.032f;

private:

};

class SpotLight : public Light
{
public:
	SpotLight();
	SpotLight(const Vec3& i, const Vec3& pos, const Vec3& dir);
	SpotLight(const SpotLight& light);
	~SpotLight();

	SpotLight& operator=(const SpotLight& light);

	Vec3 illuminate(const Ray& ray, const HitRecord& hrec, const Scene& scene) const override;

	Vec3 intensity;
	Vec3 pos;
	Vec3 dir;

	float cutoff = cos(12.5f), outCutoff = cos(17.5f);

	float constant = 1.f;
	float linear = 0.09f;
	float quadratic = 0.032f;

private:

};
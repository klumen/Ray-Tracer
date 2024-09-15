#pragma once

#include <limits>

#include "Vector.h"
#include "Matrix.h"
#include "Material.h"

class Ray
{
public:
	Ray();
	Ray(const Vec3& ori, const Vec3& dir);
	Ray(const Ray& r);
	~Ray();

	Ray& operator=(const Ray& r);

	Vec3 evaluate(float t) const;

	Vec3 o, d;

private:

};

class Surface;

class HitRecord
{
public:
	HitRecord();
	HitRecord(const HitRecord& rec);
	~HitRecord();

	HitRecord& operator=(const HitRecord& rec);

	const Surface* s;
	float t;
	Vec3 position;
	Vec2 texCoord;
	Vec3 normal;

private:

};

class Vertex
{
public:
	Vertex();
	Vertex(const Vertex& ver);
	~Vertex();

	Vertex& operator=(const Vertex& ver);

	Vec3* position;
	Vec2* texCoord;
	Vec3* normal;

private:

};

class Surface
{
public:
	Surface();
	Surface(const Surface& sur);
	Surface(Surface&& sur) noexcept;
	virtual ~Surface() = 0;

	Surface& operator=(const Surface& sur);
	Surface& operator=(Surface&& sur) noexcept;

	virtual HitRecord hit(const Ray& r, float t0, float t1) const = 0;

	Material* mtl;

private:

};

class AABB : public Surface
{
public:
	AABB();
	AABB(const Vec3& v0, const Vec3& v1);
	AABB(const AABB& box);
	~AABB();

	AABB& operator=(const AABB& box);

	HitRecord hit(const Ray& r, float t0, float t1) const override;
	AABB combine(const AABB& box) const;

	Vec3 v0, v1;

private:

};

class Triangle : public Surface
{
public:
	Triangle();
	Triangle(const Triangle& tri);
	Triangle(Triangle&& tri) noexcept;
	~Triangle();

	Triangle& operator=(const Triangle& tri);
	Triangle& operator=(Triangle&& tri) noexcept;

	HitRecord hit(const Ray& r, float t0, float t1) const override;
	AABB bounding_box() const;

	std::vector<Vertex> vers;

private:

};

class BVHnode : public Surface
{
public:
	BVHnode();
	BVHnode(BVHnode&& node) noexcept;
	~BVHnode();

	BVHnode& operator=(BVHnode&& node) noexcept;

	static BVHnode* create_node(std::vector<Triangle*>& tris, unsigned int axis);
	HitRecord hit(const Ray& r, float t0, float t1) const override;

	Surface* left;
	Surface* right;
	AABB bbox;
	bool leaf;

private:

};
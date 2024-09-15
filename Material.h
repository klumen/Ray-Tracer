#pragma once

#include <string>

#include "Vector.h"
#include "Texture.h"

class Material
{
public:
	Material();
	Material(const Material& mtl);
	~Material();

	Material& operator=(const Material& mtl);

	Vec3 evaluate(const Vec3& l, const Vec3& v, const Vec3& n, const Vec2& coord) const;

	std::string name;
	float Ns;
	Vec3 Ka, Kd, Ks, Ke;
	float Ni, d;
	unsigned int illum;
	Texture* map_Kd, * map_Ks;

private:

};
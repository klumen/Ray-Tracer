#include "Material.h"

Material::Material() : Ns(0.f), Ni(0.f), d(0.f), illum(0u), map_Kd(nullptr), map_Ks(nullptr) {}
Material::Material(const Material& mtl) : name(mtl.name), Ns(mtl.Ns), Ka(mtl.Ka), Kd(mtl.Kd), Ks(mtl.Ks), Ke(mtl.Ke), Ni(mtl.Ni), d(mtl.d), illum(mtl.illum), map_Kd(mtl.map_Kd), map_Ks(mtl.map_Kd) {}
Material::~Material() {}

Material& Material::operator=(const Material& mtl)
{
	if (this != &mtl)
	{
		name = mtl.name;
		Ns = mtl.Ns;
		Ka = mtl.Ka, Kd = mtl.Kd, Ks = mtl.Ks, Ke = mtl.Ke;
		Ni = mtl.Ni, d = mtl.d, illum = mtl.illum;
		map_Kd = mtl.map_Kd, map_Ks = mtl.map_Kd;
	}

	return *this;
}

Vec3 Material::evaluate(const Vec3& l, const Vec3& v, const Vec3& n, const Vec2& coord) const
{
	Vec3 kd = Kd;
	if (map_Kd != nullptr)
	{
		kd *= map_Kd->bilinear_interpolate(coord);
	}
	Vec3 ks = Ks;
	if (map_Ks != nullptr)
	{
		ks *= map_Ks->bilinear_interpolate(coord);
	}
	Vec3 h = (l + v).normalized();

	return kd + std::pow(std::max(0.f, n.dot(h)), Ns) * ks;
}
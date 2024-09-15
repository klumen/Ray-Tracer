#pragma once

#include <vector>
#include <fstream>
#include <map>
#include <string>
#include <sstream>

#include "Surface.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"

class Model
{
public:
	Model();
	Model(const std::string& rootPath, const std::string& fileName);
	Model(Model&& m) noexcept;
	~Model();

	Model& operator=(Model&& m) noexcept;

	void load(const std::string& rootPath, const std::string& fileName);
	HitRecord hit(const Ray& r, float t0, float t1) const;

	std::vector<Mesh> meshes;

private:
	void split_string(const std::string& line, char c, std::vector<std::string>& v);
	void load_mtls(const std::string& rootPath, const std::string& fileName, std::map<std::string, Material*>& mtlMap);

	std::vector<Vec3> vp;
	std::vector<Vec2> vt;
	std::vector<Vec3> vn;
	std::vector<Texture> texs;
	std::vector<Material> mtls;

};
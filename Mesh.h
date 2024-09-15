#pragma once

#include <string>

#include "Surface.h"

class Mesh
{
public:
	Mesh();
	Mesh(Mesh&& m) noexcept;
	~Mesh();

	Mesh& operator=(Mesh&& m) noexcept;

	std::string name;
	std::vector<Triangle> tris;
	BVHnode* BVH;

private:

};
#include "Mesh.h"

Mesh::Mesh() : BVH(nullptr) {}
Mesh::Mesh(Mesh&& m) noexcept : name(std::move(m.name)), tris(std::move(m.tris)), BVH(std::move(m.BVH)) {}
Mesh::~Mesh() 
{
	if (BVH != nullptr)
	{
		delete BVH;
		BVH = nullptr;
	}
}

Mesh& Mesh::operator=(Mesh&& m) noexcept
{
	if (&m != this)
	{
		name = std::move(m.name);
		tris = std::move(m.tris);
		BVH = std::move(m.BVH);
	}

	return *this;
}
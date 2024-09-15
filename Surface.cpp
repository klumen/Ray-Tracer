#include "Surface.h"

Ray::Ray() : o(), d() {}
Ray::Ray(const Vec3& ori, const Vec3& dir) : o(ori), d(dir) {}
Ray::Ray(const Ray& r) : o(r.o), d(r.d) {}
Ray::~Ray() {}

Ray& Ray::operator=(const Ray& r)
{
	if (this != &r)
	{
		o = r.o;
		d = r.d;
	}

	return *this;
}

Vec3 Ray::evaluate(float t) const
{
	return o + t * d;
}

HitRecord::HitRecord() : s(nullptr), t(std::numeric_limits<float>().infinity()) {}
HitRecord::HitRecord(const HitRecord& rec) : s(rec.s), t(rec.t), position(rec.position), texCoord(rec.texCoord), normal(rec.normal) {}
HitRecord::~HitRecord() {}

HitRecord& HitRecord::operator=(const HitRecord& rec)
{
	if (this != &rec)
	{
		s = rec.s;
		t = rec.t;
		position = rec.position;
		texCoord = rec.texCoord;
		normal = rec.normal;
	}

	return *this;
}


Vertex::Vertex() : position(nullptr), texCoord(nullptr), normal(nullptr) {}
Vertex::Vertex(const Vertex& ver) : position(ver.position), texCoord(ver.texCoord), normal(ver.normal) {}
Vertex::~Vertex() {}

Vertex& Vertex::operator=(const Vertex& ver)
{
	if (this != &ver)
	{
		position = ver.position;
		texCoord = ver.texCoord;
		normal = ver.normal;
	}

	return *this;
}


Surface::Surface() : mtl(nullptr) {}
Surface::Surface(const Surface& sur) : mtl(sur.mtl) {}
Surface::Surface(Surface&& sur) noexcept : mtl(sur.mtl) {}
Surface::~Surface() {}

Surface& Surface::operator=(const Surface& sur)
{
	if (this != &sur)
	{
		mtl = sur.mtl;
	}

	return *this;
}
Surface& Surface::operator=(Surface&& sur) noexcept
{
	if (this != &sur)
	{
		mtl = sur.mtl;
	}

	return *this;
}


AABB::AABB() {}
AABB::AABB(const Vec3& v0, const Vec3& v1) : v0(v0), v1(v1) {}
AABB::AABB(const AABB& box) : v0(box.v0), v1(box.v1) {}
AABB::~AABB() {}

AABB& AABB::operator=(const AABB& box)
{
	if (this != &box)
	{
		v0 = box.v0;
		v1 = box.v1;
	}

	return *this;
}

HitRecord AABB::hit(const Ray& r, float t0, float t1) const
{
	HitRecord rec;
	float t_x0, t_x1, t_y0, t_y1, t_z0, t_z1;
	float x_inv = 1.f / r.d.x, y_inv = 1.f / r.d.y, z_inv = 1.f / r.d.z;

	if (x_inv >= 0.f)
	{
		t_x0 = (v0.x - r.o.x) * x_inv;
		t_x1 = (v1.x - r.o.x) * x_inv;
	}
	else
	{
		t_x1 = (v0.x - r.o.x) * x_inv;
		t_x0 = (v1.x - r.o.x) * x_inv;
	}

	if (y_inv >= 0.f)
	{
		t_y0 = (v0.y - r.o.y) * y_inv;
		t_y1 = (v1.y - r.o.y) * y_inv;
	}
	else
	{
		t_y1 = (v0.y - r.o.y) * y_inv;
		t_y0 = (v1.y - r.o.y) * y_inv;
	}

	if (z_inv >= 0.f)
	{
		t_z0 = (v0.z - r.o.z) * z_inv;
		t_z1 = (v1.z - r.o.z) * z_inv;
	}
	else
	{
		t_z1 = (v0.z - r.o.z) * z_inv;
		t_z0 = (v1.z - r.o.z) * z_inv;
	}

	float t_enter = std::max({ t_x0, t_y0, t_z0 });
	float t_exit = std::min({ t_x1, t_y1, t_z1 });

	if (t_enter > t_exit || t_exit <= 0.f || t0 > t_exit || t1 < t_enter)
		return rec;

	rec.t = t_exit;

	return rec;
}
AABB AABB::combine(const AABB& box) const
{
	float x1 = std::min(v0.x, box.v0.x);
	float y1 = std::min(v0.y, box.v0.y);
	float z1 = std::min(v0.z, box.v0.z);
	float x2 = std::max(v1.x, box.v1.x);
	float y2 = std::max(v1.y, box.v1.y);
	float z2 = std::max(v1.z, box.v1.z);

	return AABB(Vec3(x1, y1, z1), Vec3(x2, y2, z2));
}


Triangle::Triangle() : vers(3) {}
Triangle::Triangle(const Triangle& tri) : Surface(tri), vers(tri.vers) {}
Triangle::Triangle(Triangle&& tri) noexcept : Surface(tri), vers(std::move(tri.vers)) {}
Triangle::~Triangle() {}

Triangle& Triangle::operator=(const Triangle& tri)
{
	if (this != &tri)
	{
		Surface::operator=(tri);
		vers = tri.vers;
	}

	return *this;
}
Triangle& Triangle::operator=(Triangle&& tri) noexcept
{
	if (this != &tri)
	{
		Surface::operator=(std::move(tri));
		vers = std::move(tri.vers);
	}

	return *this;
}

HitRecord Triangle::hit(const Ray& r, float t0, float t1) const
{
	HitRecord rec;

	Vec3 AC = *vers[2].position - *vers[0].position;
	Vec3 BC = *vers[2].position - *vers[1].position;
	Vec3 OC = *vers[2].position - r.o;
	const Vec3& d = r.d;
	Mat3 tempM({
		{ d.x, AC.x, BC.x },
		{ d.y, AC.y, BC.y },
		{ d.z, AC.z, BC.z } });
	float M_inv = 1.f / tempM.determinant();

	if (M_inv >= std::numeric_limits<float>::max())
		return rec;

	tempM = Mat3({
		{ OC.x, AC.x, BC.x },
		{ OC.y, AC.y, BC.y },
		{ OC.z, AC.z, BC.z } });
	float t_new = tempM.determinant() * M_inv;

	if (t_new >= t1 || t_new <= t0)
		return rec;

	tempM = Mat3({
		{ d.x, OC.x, BC.x },
		{ d.y, OC.y, BC.y },
		{ d.z, OC.z, BC.z } });
	float alpha = tempM.determinant() * M_inv;

	if (alpha < 0.f || alpha > 1.f)
		return rec;

	tempM = Mat3({
		{ d.x, AC.x, OC.x },
		{ d.y, AC.y, OC.y },
		{ d.z, AC.z, OC.z } });
	float beta = tempM.determinant() * M_inv;

	if (beta < 0.f || beta + alpha > 1.f)
		return rec;

	float gamma = 1.f - alpha - beta;
	rec.s = this;
	rec.t = t_new;
	rec.position = alpha * *vers[0].position + beta * *vers[1].position + gamma * *vers[2].position;
	rec.texCoord = alpha * *vers[0].texCoord + beta * *vers[1].texCoord + gamma * *vers[2].texCoord;
	rec.normal = (alpha * *vers[0].normal + beta * *vers[1].normal + gamma * *vers[2].normal).normalized();

	return rec;
}
AABB Triangle::bounding_box() const
{
	float x1 = std::numeric_limits<float>::max(),
		y1 = std::numeric_limits<float>::max(),
		z1 = std::numeric_limits<float>::max();
	float x2 = std::numeric_limits<float>::lowest(),
		y2 = std::numeric_limits<float>::lowest(),
		z2 = std::numeric_limits<float>::lowest();

	for (auto& ver : vers)
	{
		const Vec3& pos = *ver.position;

		x1 = std::min(x1, pos.x);
		y1 = std::min(y1, pos.y);
		z1 = std::min(z1, pos.z);
		x2 = std::max(x2, pos.x);
		y2 = std::max(y2, pos.y);
		z2 = std::max(z2, pos.z);
	}

	return AABB(Vec3(x1, y1, z1), Vec3(x2, y2, z2));
}


BVHnode::BVHnode() : left(nullptr), right(nullptr), leaf(false) {}
BVHnode::BVHnode(BVHnode&& node) noexcept : left(node.left), right(node.right), bbox(std::move(node.bbox)), leaf(node.leaf)
{
	node.right = node.left = nullptr;
}
BVHnode::~BVHnode() 
{
	if (!leaf)
	{
		if (left != nullptr)
			delete left;

		if (right != nullptr)
			delete right;
	}

	left = right = nullptr;
}

BVHnode& BVHnode::operator=(BVHnode&& node) noexcept
{
	if (this != &node)
	{
		left = node.left;
		right = node.right;
		bbox = std::move(node.bbox);
		leaf = node.leaf;
		node.left = node.right = nullptr;
	}

	return *this;
}

BVHnode* BVHnode::create_node(std::vector<Triangle*>& tris, unsigned int axis)
{
	unsigned int n = unsigned int(tris.size());
	BVHnode* node = new BVHnode();

	if (n == 1u)
	{
		node->left = node->right = tris[0];
		node->bbox = tris[0]->bounding_box();
		node->leaf = true;
	}
	else if (n == 2u)
	{
		node->left = tris[0];
		node->right = tris[1];
		node->bbox = tris[0]->bounding_box().combine(tris[1]->bounding_box());
		node->leaf = true;
	}
	else
	{
		std::sort(tris.begin(), tris.end(), [axis](Triangle* a, Triangle* b) {
			float avg1 = 0.f;
			for (auto& ver : a->vers)
			{
				const Vec3 pos = *ver.position;
				avg1 += pos[axis] / 3.f;
			}

			float avg2 = 0.f;
			for (auto& ver : b->vers)
			{
				const Vec3 pos = *ver.position;
				avg2 += pos[axis] / 3.f;
			}

			return avg1 < avg2;
			});

		std::vector<Triangle*> ltris(tris.begin(), tris.begin() + n / 2);
		std::vector<Triangle*> rtris(tris.begin() + n / 2, tris.end());
		std::vector<Triangle*>().swap(tris);

		BVHnode* left = create_node(ltris, (axis + 1u) % 3);
		BVHnode* right = create_node(rtris, (axis + 1u) % 3);

		node->bbox = left->bbox.combine(right->bbox);
		node->left = std::move(left), node->right = std::move(right);
	}

	return node;
}
HitRecord BVHnode::hit(const Ray& r, float t0, float t1) const
{
	constexpr float inf = std::numeric_limits<float>::infinity();

	HitRecord rec = bbox.hit(r, t0, t1);
	if (rec.t < inf)
	{
		HitRecord lrec, rrec;
		lrec = left->hit(r, t0, t1);
		rrec = right->hit(r, t0, t1);

		return lrec.t <= rrec.t ? lrec : rrec;
	}
	else
		return rec;
}
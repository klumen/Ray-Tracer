#include "Vector.h"

Vec2::Vec2() : x(0.f), y(0.f) {}
Vec2::Vec2(float a) : x(a), y(a) {}
Vec2::Vec2(float x, float y) : x(x), y(y) {}
Vec2::Vec2(const Vec2& v) : x(v.x), y(v.y) {}
Vec2::~Vec2() {}

Vec2 Vec2::operator+(const Vec2& v) const { return Vec2(x + v.x, y + v.y); }
Vec2 Vec2::operator-() const { return Vec2(-x, -y); }
Vec2 Vec2::operator-(const Vec2& v) const { return Vec2(x - v.x, y - v.y); }
Vec2 operator*(float k, const Vec2& v) { return Vec2(k * v.x, k * v.y); }
Vec2 Vec2::operator*(float k) const { return Vec2(x * k, y * k); }
Vec2 Vec2::operator*(const Vec2& v) const { return Vec2(x * v.x, y * v.y); }
Vec2 Vec2::operator/(float k) const { return Vec2(x / k, y / k); }
Vec2& Vec2::operator=(const Vec2& v)
{
	if (this != &v)
	{
		x = v.x, y = v.y;
	}

	return *this;
}
Vec2& Vec2::operator+=(const Vec2& v) { x += v.x, y += v.y; return *this; }
Vec2& Vec2::operator-=(const Vec2& v) { x -= v.x, y -= v.y; return *this; }
Vec2& Vec2::operator*=(float k) { x *= k, y *= k; return *this; }
Vec2& Vec2::operator*=(const Vec2& v) { x *= v.x, y *= v.y; return *this; }
Vec2& Vec2::operator/=(float k) { x /= k, y /= k; return *this; }
const float& Vec2::operator[](unsigned int i) const
{
	if (i == 0)
		return x;
	else if (i == 1)
		return y;
	else
	{
		std::cerr << "(Vec2::operator[]) Index out of range!";
		abort();
	}
}
float& Vec2::operator[](unsigned int i)
{
	if (i == 0)
		return x;
	else if (i == 1)
		return y;
	else
	{
		std::cerr << "(Vec2::operator[]) Index out of range!";
		abort();
	}
}
std::ostream& operator<<(std::ostream& cout, const Vec2& v)
{
	cout << "Vec2(" << v.x << ", " << v.y << ")";

	return cout;
}

float Vec2::norm() const { return sqrt(x * x + y * y); }
Vec2 Vec2::normalized() const { return *this / this->norm(); }
float Vec2::dot(const Vec2& v) const { return x * v.x + y * v.y; }


Vec3::Vec3() : x(0.f), y(0.f), z(0.f) {}
Vec3::Vec3(float a) : x(a), y(a), z(a) {}
Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
Vec3::Vec3(const Vec3& v) : x(v.x), y(v.y), z(v.z) {}
Vec3::Vec3(const Vec4& v) : x(v.x), y(v.y), z(v.z) {}
Vec3::~Vec3() {}

Vec3 Vec3::operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
Vec3 Vec3::operator-() const { return Vec3(-x, -y, -z); }
Vec3 Vec3::operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
Vec3 operator*(float k, const Vec3& v) { return Vec3(k * v.x, k * v.y, k * v.z); }
Vec3 Vec3::operator*(float k) const { return Vec3(x * k, y * k, z * k); }
Vec3 Vec3::operator*(const Vec3& v) const { return Vec3(x * v.x, y * v.y, z * v.z); }
Vec3 Vec3::operator/(float k) const { return Vec3(x / k, y / k, z / k); }
Vec3& Vec3::operator=(const Vec3& v)
{
	if (this != &v)
	{
		x = v.x, y = v.y, z = v.z;
	}

	return *this;
}
Vec3& Vec3::operator+=(const Vec3& v) { x += v.x, y += v.y, z += v.z; return *this; }
Vec3& Vec3::operator-=(const Vec3& v) { x -= v.x, y -= v.y, z -= v.z; return *this; }
Vec3& Vec3::operator*=(float k) { x *= k, y *= k, z *= k; return *this; }
Vec3& Vec3::operator*=(const Vec3& v) { x *= v.x, y *= v.y, z *= v.z; return *this; }
Vec3& Vec3::operator/=(float k) { x /= k, y /= k, z /= k; return *this; }
const float& Vec3::operator[](unsigned int i) const
{
	if (i == 0)
		return x;
	else if (i == 1)
		return y;
	else if (i == 2)
		return z;
	else
	{
		std::cerr << "(Vec3::operator[]) Index out of range!";
		abort();
	}
}
float& Vec3::operator[](unsigned int i)
{
	if (i == 0)
		return x;
	else if (i == 1)
		return y;
	else if (i == 2)
		return z;
	else
	{
		std::cerr << "(Vec3::operator[]) Index out of range!";
		abort();
	}
}
std::ostream& operator<<(std::ostream& cout, const Vec3& v)
{
	cout << "Vec3(" << v.x << ", " << v.y << ", " << v.z << ")";

	return cout;
}

float Vec3::norm() const { return sqrt(x * x + y * y + z * z); }
Vec3 Vec3::normalized() const { return *this / this->norm(); }
float Vec3::dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
Vec3 Vec3::cross(const Vec3& v) const { return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }


Vec4::Vec4() : x(0.f), y(0.f), z(0.f), w(0.f) {}
Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
Vec4::Vec4(const Vec4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
Vec4::Vec4(const Vec3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
Vec4::~Vec4() {}

Vec4 Vec4::operator+(const Vec4& v) const { return Vec4(x + v.x, y + v.y, z + v.z, w + v.w); }
Vec4 Vec4::operator-() const { return Vec4(-x, -y, -z, -w); }
Vec4 Vec4::operator-(const Vec4& v) const { return Vec4(x - v.x, y - v.y, z - v.z, w - v.w); }
Vec4 operator*(float k, const Vec4& v) { return Vec4(k * v.x, k * v.y, k * v.z, k * v.w); }
Vec4 Vec4::operator*(float k) const { return Vec4(x * k, y * k, z * k, w * k); }
Vec4 Vec4::operator*(const Vec4& v) const { return Vec4(x * v.x, y * v.y, z * v.z, w * v.w); }
Vec4 Vec4::operator/(float k) const { return Vec4(x / k, y / k, z / k, w / k); }
Vec4& Vec4::operator=(const Vec4& v)
{
	if (this != &v)
	{
		x = v.x, y = v.y, z = v.z, w = v.w;
	}

	return *this;
}
Vec4& Vec4::operator+=(const Vec4& v) { x += v.x, y += v.y, z += v.z, w += v.w; return *this; }
Vec4& Vec4::operator-=(const Vec4& v) { x -= v.x, y -= v.y, z -= v.z, w -= v.w; return *this; }
Vec4& Vec4::operator*=(float k) { x *= k, y *= k, z *= k, w *= k; return *this; }
Vec4& Vec4::operator*=(const Vec4& v) { x *= v.x, y *= v.y, z *= v.z, w *= v.w; return *this; }
Vec4& Vec4::operator/=(float k) { x /= k, y /= k, z /= k, w /= k; return *this; }
const float& Vec4::operator[](unsigned int i) const
{
	if (i == 0)
		return x;
	else if (i == 1)
		return y;
	else if (i == 2)
		return z;
	else if (i == 3)
		return w;
	else
	{
		std::cerr << "(Vec4::operator[]) Index out of range!";
		abort();
	}
}
float& Vec4::operator[](unsigned int i)
{
	if (i == 0)
		return x;
	else if (i == 1)
		return y;
	else if (i == 2)
		return z;
	else if (i == 3)
		return w;
	else
	{
		std::cerr << "(Vec4::operator[]) Index out of range!";
		abort();
	}
}
std::ostream& operator<<(std::ostream& cout, const Vec4& v)
{
	cout << "Vec4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";

	return cout;
}
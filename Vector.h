#pragma once

#include <iostream>
#include <cmath>

class Vec2
{
	friend Vec2 operator*(float k, const Vec2& v);
	friend std::ostream& operator<<(std::ostream& cout, const Vec2& v);

public:
	Vec2();
	Vec2(float a);
	Vec2(float x, float y);
	Vec2(const Vec2& v);
	~Vec2();

	Vec2 operator+(const Vec2& v) const;
	Vec2 operator-() const;
	Vec2 operator-(const Vec2& v) const;
	Vec2 operator*(float k) const;
	Vec2 operator*(const Vec2& v) const;
	Vec2 operator/(float k) const;
	Vec2& operator=(const Vec2& v);
	Vec2& operator+=(const Vec2& v);
	Vec2& operator-=(const Vec2& v);
	Vec2& operator*=(float k);
	Vec2& operator*=(const Vec2& v);
	Vec2& operator/=(float k);
	const float& operator[](unsigned int i) const;
	float& operator[](unsigned int i);

	float norm() const;
	Vec2 normalized() const;
	float dot(const Vec2& v) const;

	float x, y;

private:

};
Vec2 operator*(float k, const Vec2& v);
std::ostream& operator<<(std::ostream& cout, const Vec2& v);

class Vec4;

class Vec3
{
public:
	Vec3();
	Vec3(float a);
	Vec3(float x, float y, float z);
	Vec3(const Vec3& v);
	Vec3(const Vec4& v);
	~Vec3();

	Vec3 operator+(const Vec3& v) const;
	Vec3 operator-() const;
	Vec3 operator-(const Vec3& v) const;
	Vec3 operator*(float k) const;
	Vec3 operator*(const Vec3& v) const;
	Vec3 operator/(float k) const;
	Vec3& operator=(const Vec3& v);
	Vec3& operator+=(const Vec3& v);
	Vec3& operator-=(const Vec3& v);
	Vec3& operator*=(float k);
	Vec3& operator*=(const Vec3& v);
	Vec3& operator/=(float k);
	const float& operator[](unsigned int i) const;
	float& operator[](unsigned int i);

	float norm() const;
	Vec3 normalized() const;
	float dot(const Vec3& v) const;
	Vec3 cross(const Vec3& v) const;

	float x, y, z;

private:

};
Vec3 operator*(float k, const Vec3& v);
std::ostream& operator<<(std::ostream& cout, const Vec3& v);

class Vec4
{
public:
	Vec4();
	Vec4(float x, float y, float z, float w);
	Vec4(const Vec4& v);
	Vec4(const Vec3& v, float w);
	~Vec4();
	
	Vec4 operator+(const Vec4& v) const;
	Vec4 operator-() const;
	Vec4 operator-(const Vec4& v) const;
	Vec4 operator*(float k) const;
	Vec4 operator*(const Vec4& v) const;
	Vec4 operator/(float k) const;
	Vec4& operator=(const Vec4& v);
	Vec4& operator+=(const Vec4& v);
	Vec4& operator-=(const Vec4& v);
	Vec4& operator*=(float k);
	Vec4& operator*=(const Vec4& v);
	Vec4& operator/=(float k);
	const float& operator[](unsigned int i) const;
	float& operator[](unsigned int i);

	float x, y, z, w;

private:

};
Vec4 operator*(float k, const Vec4& v);
std::ostream& operator<<(std::ostream& cout, const Vec4& v);
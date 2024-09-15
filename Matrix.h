#pragma once

#include <iostream>
#include <vector>

#include "Vector.h"

class Mat4;

class Mat3
{
	friend Mat3 operator*(float k, const Mat3& m);
	friend std::ostream& operator<<(std::ostream& cout, const Mat3& m);

public:
	Mat3();
	Mat3(const std::vector<std::vector<float>>& eles);
	Mat3(const Mat3& m);
	Mat3(Mat3&& m) noexcept;
	Mat3(const Mat4& m);
	~Mat3();

	static Mat3 identity();
	Mat3 operator+(const Mat3& m) const;
	Mat3 operator-(const Mat3& m) const;
	Mat3 operator*(float k) const;
	Mat3 operator*(const Mat3& m) const;
	Vec3 operator*(const Vec3& v) const;
	Mat3 operator/(float k) const;
	Mat3& operator=(const Mat3& m);
	Mat3& operator=(Mat3&& m) noexcept;
	Mat3& operator+=(const Mat3& m);
	Mat3& operator-=(const Mat3& m);
	Mat3& operator*=(float k);
	Mat3& operator/=(float k);
	const std::vector<float>& operator[](unsigned int i) const;
	std::vector<float>& operator[](unsigned int i);

	Mat3 transpose() const;
	float cofactor(unsigned int i, unsigned int j) const;
	Mat3 adjoint() const;
	float determinant() const;
	Mat3 inverse() const;

private:
	std::vector<std::vector<float>> eles;

};

Mat3 operator*(float k, const Mat3& m);
std::ostream& operator<<(std::ostream& cout, const Mat3& m);

class Mat4
{
	friend Mat4 operator*(float k, const Mat4& m);
	friend std::ostream& operator<<(std::ostream& cout, const Mat4& m);

public:
	Mat4();
	Mat4(const std::vector<std::vector<float>>& eles);
	Mat4(const Mat4& m);
	Mat4(Mat4&& m) noexcept;
	Mat4(const Mat3& m);
	~Mat4();

	static Mat4 identity();
	Mat4 operator+(const Mat4& m) const;
	Mat4 operator-(const Mat4& m) const;
	Mat4 operator*(float k) const;
	Mat4 operator*(const Mat4& m) const;
	Vec4 operator*(const Vec4& v) const;
	Mat4 operator/(float k) const;
	Mat4& operator=(const Mat4& m);
	Mat4& operator=(Mat4&& m) noexcept;
	Mat4& operator+=(const Mat4& m);
	Mat4& operator-=(const Mat4& m);
	Mat4& operator*=(float k);
	Mat4& operator/=(float k);
	const std::vector<float>& operator[](unsigned int i) const;
	std::vector<float>& operator[](unsigned int i);

	Mat4 transpose() const;
	float cofactor(unsigned int i, unsigned int j) const;
	Mat4 adjoint() const;
	float determinant() const;
	Mat4 inverse() const;

private:
	std::vector<std::vector<float>> eles;

};

Mat4 operator*(float k, const Mat4& m);
std::ostream& operator<<(std::ostream& cout, const Mat4& m);
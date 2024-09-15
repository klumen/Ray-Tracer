#include "Matrix.h"

Mat3::Mat3() : eles(3, std::vector<float>(3, 0.f)) {}
Mat3::Mat3(const std::vector<std::vector<float>>& eles) : eles(eles)
{
	if (eles.empty() || eles.size() != 3 || eles[0].size() != 3)
	{
		std::cerr << "(Mat3::Mat3) Input elements must be 9 float numbers!";
		abort();
	}
}
Mat3::Mat3(const Mat3& m) : eles(m.eles) {}
Mat3::Mat3(Mat3&& m) noexcept : eles(std::move(m.eles)) {}
Mat3::Mat3(const Mat4& m) : eles(3, std::vector<float>(3, 0.f))
{
	for (unsigned int i = 0; i < 3; i++)
		for (unsigned int j = 0; j < 3; j++)
			eles[i][j] = m[i][j];
}
Mat3::~Mat3() {}

Mat3 Mat3::identity()
{
	std::vector<std::vector<float>> newEles(3, std::vector<float>(3, 0.f));
	for (unsigned int i = 0; i < 3; i++)
		newEles[i][i] = 1.f;

	return Mat3(newEles);
}
Mat3 Mat3::operator+(const Mat3& m) const
{
	std::vector<std::vector<float>> newEles(eles);
	for (unsigned int i = 0; i < 3; i++)
		for (unsigned int j = 0; j < 3; j++)
			newEles[i][j] += m.eles[i][j];

	return Mat3(newEles);
}
Mat3 Mat3::operator-(const Mat3& m) const
{
	std::vector<std::vector<float>> newEles(eles);
	for (unsigned int i = 0; i < 3; i++)
		for (unsigned int j = 0; j < 3; j++)
			newEles[i][j] -= m.eles[i][j];

	return Mat3(newEles);
}
Mat3 operator*(float k, const Mat3& m)
{
	std::vector<std::vector<float>> newEles(m.eles);
	for (unsigned int i = 0; i < 3; i++)
		for (unsigned int j = 0; j < 3; j++)
			newEles[i][j] *= k;

	return Mat3(newEles);
}
Mat3 Mat3::operator*(float k) const
{
	std::vector<std::vector<float>> newEles(eles);
	for (unsigned int i = 0; i < 3; i++)
		for (unsigned int j = 0; j < 3; j++)
			newEles[i][j] *= k;

	return Mat3(newEles);
}
Mat3 Mat3::operator*(const Mat3& m) const
{
	std::vector<std::vector<float>> newEles(3, std::vector<float>(3, 0.f));
	for (unsigned int i = 0; i < 3; i++)
		for (unsigned int j = 0; j < 3; j++)
			for (unsigned int k = 0; k < 3; k++)
				newEles[i][k] += eles[i][j] * m.eles[j][k];

	return Mat3(newEles);
}
Vec3 Mat3::operator*(const Vec3& v) const
{
	Vec3 res;
	for (unsigned int i = 0; i < 3; i++)
		for (unsigned int j = 0; j < 3; j++)
			res[i] += eles[i][j] * v[j];

	return res;
}
Mat3 Mat3::operator/(float k) const
{
	std::vector<std::vector<float>> newEles(eles);
	for (unsigned int i = 0; i < 3; i++)
		for (unsigned int j = 0; j < 3; j++)
			newEles[i][j] /= k;

	return Mat3(newEles);
}
Mat3& Mat3::operator=(const Mat3& m)
{
	if (this != &m)
		eles = m.eles;

	return *this;
}
Mat3& Mat3::operator=(Mat3&& m) noexcept
{
	if (this != &m)
		eles = std::move(m.eles);

	return *this;
}
Mat3& Mat3::operator+=(const Mat3& m)
{
	for (unsigned int i = 0; i < 3; i++)
		for (unsigned int j = 0; j < 3; j++)
			eles[i][j] += m.eles[i][j];

	return *this;
}
Mat3& Mat3::operator-=(const Mat3& m)
{
	for (unsigned int i = 0; i < 3; i++)
		for (unsigned int j = 0; j < 3; j++)
			eles[i][j] -= m.eles[i][j];

	return *this;
}
Mat3& Mat3::operator*=(float k)
{
	for (unsigned int i = 0; i < 3; i++)
		for (unsigned int j = 0; j < 3; j++)
			eles[i][j] *= k;

	return *this;
}
Mat3& Mat3::operator/=(float k)
{
	for (unsigned int i = 0; i < 3; i++)
		for (unsigned int j = 0; j < 3; j++)
			eles[i][j] /= k;

	return *this;
}
std::ostream& operator<<(std::ostream& cout, const Mat3& m)
{
	cout << "Mat3:\n";
	for (unsigned int i = 0; i < 3; i++)
	{
		for (unsigned int j = 0; j < 3; j++)
			cout << m.eles[i][j] << " ";
		cout << "\n";
	}

	return cout;
}
const std::vector<float>& Mat3::operator[](unsigned int i) const
{
	if (i > 2)
	{
		std::cerr << "(Mat3::operator[]) Index out of range!";
		abort();
	}
	else
		return eles[i];
}
std::vector<float>& Mat3::operator[](unsigned int i)
{
	if (i > 2)
	{
		std::cerr << "(Mat3::operator[]) Index out of range!";
		abort();
	}
	else
		return eles[i];
}

Mat3 Mat3::transpose() const
{
	std::vector<std::vector<float>> newEles(3, std::vector<float>(3, 0.f));
	for (unsigned int i = 0; i < 3; i++)
		for (unsigned int j = 0; j < 3; j++)
			newEles[i][j] = eles[j][i];

	return Mat3(newEles);
}
float Mat3::cofactor(unsigned int i, unsigned int j) const
{
	std::vector<float> t;
	for (unsigned int r = 0; r < 3; r++)
	{
		if (r == i)
			continue;
		for (unsigned int c = 0; c < 3; c++)
		{
			if (c == j)
				continue;
			t.emplace_back(eles[r][c]);
		}
	}

	float k = i + j % 2u == 0u ? 1.f : -1.f;

	return k * (t[0] * t[3] - t[1] * t[2]);
}
Mat3 Mat3::adjoint() const
{
	std::vector<std::vector<float>> newEles(3, std::vector<float>(3, 0.f));
	for (unsigned int i = 0; i < 3; i++)
		for (unsigned int j = 0; j < 3; j++)
			newEles[i][j] = cofactor(j, i);

	return Mat3(newEles);
}
float Mat3::determinant() const
{
	return (
		eles[0][0] * eles[1][1] * eles[2][2] +
		eles[0][1] * eles[1][2] * eles[2][0] +
		eles[0][2] * eles[1][0] * eles[2][1] -
		eles[0][2] * eles[1][1] * eles[2][0] -
		eles[0][1] * eles[1][0] * eles[2][2] -
		eles[0][0] * eles[1][2] * eles[2][1]);
}
Mat3 Mat3::inverse() const { return adjoint() / determinant(); }


Mat4::Mat4() : eles(4, std::vector<float>(4, 0.f)) {}
Mat4::Mat4(const std::vector<std::vector<float>>& eles) : eles(eles)
{
	if (eles.empty() || eles.size() != 4 || eles[0].size() != 4)
	{
		std::cerr << "(Mat4::Mat4) Input elements must be 16 float numbers!";
		abort();
	}
}
Mat4::Mat4(const Mat4& m) : eles(m.eles) {}
Mat4::Mat4(Mat4&& m) noexcept : eles(std::move(m.eles)) {}
Mat4::Mat4(const Mat3& m) : eles(4, std::vector<float>(4, 0.f))
{
	for (unsigned int i = 0; i < 3; i++)
		for (unsigned int j = 0; j < 3; j++)
			eles[i][j] = m[i][j];
	eles[3][3] = 1.f;
}
Mat4::~Mat4() {}

Mat4 Mat4::identity()
{
	std::vector<std::vector<float>> newEles(4, std::vector<float>(4, 0.f));
	for (unsigned int i = 0; i < 4; i++)
		newEles[i][i] = 1.f;

	return Mat4(newEles);
}
Mat4 Mat4::operator+(const Mat4& m) const
{
	std::vector<std::vector<float>> newEles(eles);
	for (unsigned int i = 0; i < 4; i++)
		for (unsigned int j = 0; j < 4; j++)
			newEles[i][j] += m.eles[i][j];

	return Mat4(newEles);
}
Mat4 Mat4::operator-(const Mat4& m) const
{
	std::vector<std::vector<float>> newEles(eles);
	for (unsigned int i = 0; i < 4; i++)
		for (unsigned int j = 0; j < 4; j++)
			newEles[i][j] -= m.eles[i][j];

	return Mat4(newEles);
}
Mat4 operator*(float k, const Mat4& m)
{
	std::vector<std::vector<float>> newEles(m.eles);
	for (unsigned int i = 0; i < 4; i++)
		for (unsigned int j = 0; j < 4; j++)
			newEles[i][j] *= k;

	return Mat4(newEles);
}
Mat4 Mat4::operator*(float k) const
{
	std::vector<std::vector<float>> newEles(eles);
	for (unsigned int i = 0; i < 4; i++)
		for (unsigned int j = 0; j < 4; j++)
			newEles[i][j] *= k;

	return Mat4(newEles);
}
Mat4 Mat4::operator*(const Mat4& m) const
{
	std::vector<std::vector<float>> newEles(4, std::vector<float>(4, 0.f));
	for (unsigned int i = 0; i < 4; i++)
		for (unsigned int j = 0; j < 4; j++)
			for (unsigned int k = 0; k < 4; k++)
				newEles[i][k] += eles[i][j] * m.eles[j][k];

	return Mat4(newEles);
}
Vec4 Mat4::operator*(const Vec4& v) const
{
	Vec4 res;
	for (unsigned int i = 0; i < 4; i++)
		for (unsigned int j = 0; j < 4; j++)
			res[i] += eles[i][j] * v[j];

	return res;
}
Mat4 Mat4::operator/(float k) const
{
	std::vector<std::vector<float>> newEles(eles);
	for (unsigned int i = 0; i < 4; i++)
		for (unsigned int j = 0; j < 4; j++)
			newEles[i][j] /= k;

	return Mat4(newEles);
}
Mat4& Mat4::operator=(const Mat4& m)
{
	if (this != &m)
		eles = m.eles;

	return *this;
}
Mat4& Mat4::operator=(Mat4&& m) noexcept
{
	if (this != &m)
		eles = std::move(m.eles);

	return *this;
}
Mat4& Mat4::operator+=(const Mat4& m)
{
	for (unsigned int i = 0; i < 4; i++)
		for (unsigned int j = 0; j < 4; j++)
			eles[i][j] += m.eles[i][j];

	return *this;
}
Mat4& Mat4::operator-=(const Mat4& m)
{
	for (unsigned int i = 0; i < 4; i++)
		for (unsigned int j = 0; j < 4; j++)
			eles[i][j] -= m.eles[i][j];

	return *this;
}
Mat4& Mat4::operator*=(float k)
{
	for (unsigned int i = 0; i < 4; i++)
		for (unsigned int j = 0; j < 4; j++)
			eles[i][j] *= k;

	return *this;
}
Mat4& Mat4::operator/=(float k)
{
	for (unsigned int i = 0; i < 4; i++)
		for (unsigned int j = 0; j < 4; j++)
			eles[i][j] /= k;

	return *this;
}
std::ostream& operator<<(std::ostream& cout, const Mat4& m)
{
	cout << "Mat4:\n";
	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
			cout << m.eles[i][j] << " ";
		cout << "\n";
	}

	return cout;
}
const std::vector<float>& Mat4::operator[](unsigned int i) const
{
	if (i > 3)
	{
		std::cerr << "(Mat4::operator[]) Index out of range!";
		abort();
	}
	else
		return eles[i];
}
std::vector<float>& Mat4::operator[](unsigned int i)
{
	if (i > 3)
	{
		std::cerr << "(Mat4::operator[]) Index out of range!";
		abort();
	}
	else
		return eles[i];
}

Mat4 Mat4::transpose() const
{
	std::vector<std::vector<float>> newEles(4, std::vector<float>(4, 0.f));
	for (unsigned int i = 0; i < 4; i++)
		for (unsigned int j = 0; j < 4; j++)
			newEles[i][j] = eles[j][i];

	return Mat4(newEles);
}
float Mat4::cofactor(unsigned int i, unsigned int j) const
{
	std::vector<float> t;
	for (unsigned int r = 0; r < 4; r++)
	{
		if (r == i)
			continue;
		for (unsigned int c = 0; c < 4; c++)
		{
			if (c == j)
				continue;
			t.emplace_back(eles[r][c]);
		}
	}

	float k = i + j % 2u == 0u ? 1.f : -1.f;

	return k * (
		t[0] * t[4] * t[8] +
		t[1] * t[5] * t[6] +
		t[2] * t[3] * t[7] -
		t[2] * t[4] * t[6] -
		t[1] * t[3] * t[8] -
		t[0] * t[5] * t[7]);
}
Mat4 Mat4::adjoint() const
{
	std::vector<std::vector<float>> newEles(4, std::vector<float>(4, 0.f));
	for (unsigned int i = 0; i < 4; i++)
		for (unsigned int j = 0; j < 4; j++)
			newEles[i][j] = cofactor(j, i);

	return Mat4(newEles);
}
float Mat4::determinant() const
{
	return (eles[0][0] * cofactor(0, 0) + eles[0][1] * cofactor(0, 1)
		+ eles[0][2] * cofactor(0, 2) + eles[0][3] * cofactor(0, 3));
}
Mat4 Mat4::inverse() const { return adjoint() / determinant(); }
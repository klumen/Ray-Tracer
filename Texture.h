#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "Vector.h"

class Texture
{
public:
	Texture();
	Texture(const std::string& rootPath, const std::string& fileName);
	Texture(const Texture& tex);
	Texture(Texture&& tex) noexcept;
	~Texture();

	Texture& operator=(const Texture& tex);

	void load(const std::string& rootPath, const std::string& fileName);
	void generate_mipmap();
	Vec4 look_up(const Vec2& coord) const;
	Vec4 bilinear_interpolate(const Vec2& coord) const;

	unsigned int width, height;

private:
	std::vector<std::vector<Vec4>> mipmap;

};
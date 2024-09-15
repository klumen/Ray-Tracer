#include "Texture.h"

Texture::Texture() : width(0u), height(0u) {}
Texture::Texture(const std::string& rootPath, const std::string& fileName) : width(0u), height(0u) { load(rootPath, fileName); }
Texture::Texture(const Texture& tex) : width(tex.width), height(tex.height), mipmap(tex.mipmap) {}
Texture::Texture(Texture&& tex) noexcept : width(tex.width), height(tex.height), mipmap(std::move(tex.mipmap)) {}
Texture::~Texture() {}

Texture& Texture::operator=(const Texture& tex)
{
	if (&tex != this)
	{
		width = tex.width, height = tex.height;
		mipmap = tex.mipmap;
	}

	return *this;
}

void Texture::load(const std::string& rootPath, const std::string& fileName)
{
	std::ifstream texFile(rootPath + fileName, std::ios::in | std::ios::binary);
	if (!texFile.is_open())
	{
		std::cerr << "(Model::load_texs) Find no file!";
		abort();
	}

	texFile.seekg(0, std::ios::end);
	size_t fileSize = texFile.tellg();
	texFile.seekg(0, std::ios::beg);

	char* buffer = new char[fileSize];
	texFile.read(buffer, fileSize);

	texFile.close();

	std::vector<unsigned int> buf(fileSize);
	for (unsigned int i = 0; i < fileSize; i++)
		buf[i] = buffer[i] & 0xff;

	delete[] buffer;
	buffer = nullptr;

	std::vector<Vec4> data;

	if (buf[0] == 0x42 && buf[1] == 0x4d) // bmp
	{
		unsigned int bfOffBits = buf[13] * 0x1000000 + buf[12] * 0x10000 + buf[11] * 0x100 + buf[10];
		width = buf[19] * 256 + buf[18];
		height = buf[23] * 256 + buf[22];

		unsigned int bitDepth = buf[28], infSize = buf[14] + buf[15] * 0x100 + buf[16] * 0x10000 + buf[17] * 0x1000000;

		data.resize((size_t)width * height);
		unsigned int point = bfOffBits;
		if (bfOffBits == infSize + 14)
		{
			for (unsigned int y = 0u; y < height; y++)
				for (unsigned int x = 0u; x < width; x++)
				{
					unsigned int ind = y * width + x;
					if (bitDepth == 24)
					{
						data[ind] = Vec4(buf[point + 2] * 1.f / 255.f, buf[point + 1] * 1.f / 255.f, buf[point] * 1.f / 255.f, 1.f);
						point += 3;
					}
					else if (bitDepth == 32)
					{
						data[ind] = Vec4(buf[point + 2] * 1.f / 255.f, buf[point + 1] * 1.f / 255.f, buf[point] * 1.f / 255.f, buf[point + 3] * 1.f / 255.f);
						point += 4;
					}
				}
		}
	}

	mipmap.emplace_back(std::move(data));

	texFile.close();
}
void Texture::generate_mipmap()
{
	mipmap.resize(size_t(std::log2(width * 1.0f)) + 1);
	size_t width_i = width, height_i = height;

	for (size_t i = 1; i < mipmap.size(); i++)
	{
		width_i /= 2, height_i /= 2;
		mipmap[i].resize(width_i * height_i);

		for (size_t y = 0; y < height_i; y++)
			for (size_t x = 0; x < width_i; x++)
			{
				size_t ind = y * width_i + x;
				for (size_t y2 = y * 2; y2 < y * 2 + 2; y2++)
					for (size_t x2 = x * 2; x2 < x * 2 + 2; x2++)
					{
						size_t ind2 = y2 * width_i * 2 + x2;
						mipmap[i][ind] += mipmap[i - 1][ind2] / 4.f;
					}
			}
	}
}
Vec4 Texture::look_up(const Vec2& coord) const
{
	unsigned int u = (unsigned int)std::clamp(coord.x * float(width), 0.f, float(width) - 1.f);
	unsigned int v = (unsigned int)std::clamp(coord.y * float(height), 0.f, float(height) - 1.f);
	unsigned int ind = v * width + u;

	return mipmap[0][ind];
}
Vec4 Texture::bilinear_interpolate(const Vec2& coord) const
{
	const std::vector<Vec4>& data = mipmap[0];
	float u = coord.x * float(width) - 0.5f;
	float v = coord.y * float(height) - 0.5f;

	unsigned int du = std::clamp(unsigned int(std::floorf(u)), 0u, width - 1u);
	unsigned int dv = std::clamp(unsigned int(std::floorf(v)), 0u, height - 1u);
	unsigned int ind = dv * width + du;
	Vec4 color_00 = data[ind];

	du = std::clamp(unsigned int(std::floorf(u)), 0u, width - 1u);
	dv = std::clamp(unsigned int(std::ceilf(v)), 0u, height - 1u);
	ind = dv * width + du;
	Vec4 color_01 = data[ind];

	du = std::clamp(unsigned int(std::ceilf(u)), 0u, width - 1u);
	dv = std::clamp(unsigned int(std::floorf(v)), 0u, height - 1u);
	ind = dv * width + du;
	Vec4 color_10 = data[ind];

	du = std::clamp(unsigned int(std::ceilf(u)), 0u, width - 1u);
	dv = std::clamp(unsigned int(std::ceilf(v)), 0u, height - 1u);
	ind = dv * width + du;
	Vec4 color_11 = data[ind];

	float s = u - std::floorf(u), t = v - std::floorf(v);

	Vec4 color_0 = color_00 + s * (color_10 - color_00);
	Vec4 color_1 = color_01 + s * (color_11 - color_01);

	return color_0 + t * (color_1 - color_0);
}
#include "Model.h"

Model::Model() {}
Model::Model(const std::string& rootPath, const std::string& fileName) { load(rootPath, fileName); }
Model::Model(Model&& m) noexcept : meshes(std::move(m.meshes)), vp(std::move(m.vp)), vt(std::move(m.vt)), vn(std::move(m.vn)), texs(std::move(m.texs)), mtls(std::move(m.mtls)) {}
Model::~Model() {}

Model& Model::operator=(Model&& m) noexcept
{
	if (this != &m)
	{
		meshes = std::move(m.meshes);
		vp = std::move(m.vp), vt = std::move(m.vt), vn = std::move(m.vn);
		texs = std::move(m.texs), mtls = std::move(m.mtls);
	}

	return *this;
}

void Model::split_string(const std::string& line, char c, std::vector<std::string>& v)
{
	std::istringstream sStream(line);
	std::string word;

	while (std::getline(sStream, word, c))
		v.emplace_back(word);
}
void Model::load_mtls(const std::string& rootPath, const std::string& fileName, std::map<std::string, Material*>& mtlMap)
{
	std::ifstream mtlFile(rootPath + fileName, std::ios::in);
	if (!mtlFile.is_open())
	{
		std::cerr << "(Model::load_mtls) Find no file!";
		abort();
	}

	std::map<std::string, Texture*> texMap;
	std::vector<std::string> words;
	std::string line;

	while (std::getline(mtlFile, line))
	{
		if (line.substr(0, 6) == "newmtl")
		{
			mtlMap[line.erase(0, 7)] = &mtls.emplace_back();
		}
		else if (line[0] == 'N' && line[1] == 's')
		{
			mtls.back().Ns = std::stof(line.erase(0, 3));
		}
		else if (line[0] == 'K' && line[1] == 'a')
		{
			words.clear();
			split_string(line.erase(0, 3), ' ', words);

			mtls.back().Ka = Vec3(std::stof(words[0]), std::stof(words[1]), std::stof(words[2]));
		}
		else if (line[0] == 'K' && line[1] == 'd')
		{
			words.clear();
			split_string(line.erase(0, 3), ' ', words);

			mtls.back().Kd = Vec3(std::stof(words[0]), std::stof(words[1]), std::stof(words[2]));
		}
		else if (line[0] == 'K' && line[1] == 's')
		{
			words.clear();
			split_string(line.erase(0, 3), ' ', words);

			mtls.back().Ks = Vec3(std::stof(words[0]), std::stof(words[1]), std::stof(words[2]));
		}
		else if (line[0] == 'K' && line[1] == 'e')
		{
			words.clear();
			split_string(line.erase(0, 3), ' ', words);

			mtls.back().Ke = Vec3(std::stof(words[0]), std::stof(words[1]), std::stof(words[2]));
		}
		else if (line[0] == 'N' && line[1] == 'i')
		{
			mtls.back().Ni = std::stof(line.erase(0, 3));
		}
		else if (line[0] == 'd')
		{
			mtls.back().d = std::stof(line.erase(0, 2));
		}
		else if (line.substr(0, 5) == "illum")
		{
			mtls.back().illum = std::stoul(line.erase(0, 6));
		}
		else if (line.substr(0, 6) == "map_Kd")
		{
			const std::string texName = line.erase(0, 7);
			if (texMap.find(texName) == texMap.end())
			{
				texMap[texName] = &texs.emplace_back();
				texs.back().load(rootPath, texName);
			}

			mtls.back().map_Kd = texMap[texName];
		}
		else if (line.substr(0, 6) == "map_Ks")
		{
			const std::string texName = line.erase(0, 7);
			if (texMap.find(texName) == texMap.end())
			{
				texMap[texName] = &texs.emplace_back();
				texs.back().load(rootPath, texName);
			}

			mtls.back().map_Ks = texMap[texName];
		}
	}

	mtlFile.close();
}
void Model::load(const std::string& rootPath, const std::string& fileName)
{
	std::ifstream objFile(rootPath + fileName, std::ios::in);
	if (!objFile.is_open())
	{
		std::cerr << "(Model::load) Find no file!";
		abort();
	}

	std::vector<std::string> words;
	std::map<std::string, Material*> mtlMap;
	Material* nowMtl = nullptr;
	std::string line;

	while (std::getline(objFile, line))
	{
		if (line.substr(0, 6) == "mtllib")
		{
			load_mtls(rootPath, line.erase(0, 7), mtlMap);
		}
		else if (line[0] == 'o')
		{
			meshes.emplace_back();
			meshes.back().name = line.erase(0, 2);
		}
		else if (line[0] == 'v' && line[1] == ' ')
		{
			words.clear();
			split_string(line.erase(0, 2), ' ', words);

			vp.emplace_back(std::stof(words[0]), std::stof(words[1]), std::stof(words[2]));
		}
		else if (line[0] == 'v' && line[1] == 't')
		{
			words.clear();
			split_string(line.erase(0, 3), ' ', words);

			vt.emplace_back(std::stof(words[0]), std::stof(words[1]));
		}
		else if (line[0] == 'v' && line[1] == 'n')
		{
			words.clear();
			split_string(line.erase(0, 3), ' ', words);

			vn.emplace_back(std::stof(words[0]), std::stof(words[1]), std::stof(words[2]));
		}
		else if (line.substr(0, 6) == "usemtl")
		{
			nowMtl = mtlMap[line.erase(0, 7)];
		}
		else if (line[0] == 'f')
		{
			words.clear();
			split_string(line.erase(0, 2), ' ', words);
			Triangle& tri = meshes.back().tris.emplace_back();

			std::vector<std::string> aInds;
			for (unsigned int i = 0; i < words.size(); i++)
			{
				aInds.clear();
				split_string(words[i], '/', aInds);

				std::vector<unsigned int> ind(aInds.size());
				for (unsigned int i = 0; i < ind.size(); i++)
					ind[i] = std::stoul(aInds[i]) - 1;

				Vertex& ver = tri.vers[i];
				ver.position = &vp[ind[0]];
				if (ind.size() >= 2)
					ver.texCoord = &vt[ind[1]];
				if (ind.size() >= 3)
					ver.normal = &vn[ind[2]];
			}

			tri.mtl = nowMtl;
		}
	}

	objFile.close();
}
HitRecord Model::hit(const Ray& r, float t0, float t1) const
{
	HitRecord res;

	for (auto& mesh : meshes)
	{
		HitRecord rec = mesh.BVH->hit(r, t0, t1);
		if (rec.t < std::numeric_limits<float>::infinity())
		{
			res = rec;
			t1 = rec.t;
		}
	}

	return res;
}
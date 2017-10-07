#include <Odin/OBJLoader.h>
#include <Util/File.h>
#include <Util/String.h>

bool OBJLoader::Load(const std::string& filename, Mesh &mesh)
{
	std::string contents = File::Read(filename);
	std::vector<std::string> lines = String::Split(contents, '\n');
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	for (std::string line : lines)
	{
		if (String::StartsWith(line, "v "))
		{
			std::vector<std::string> elements = String::Split(line, ' ');
			vertices.push_back(std::stof(elements[1]));
			vertices.push_back(std::stof(elements[2]));
			vertices.push_back(std::stof(elements[3]));
		}

		if (String::StartsWith(line, "f "))
		{
			std::vector<std::string> elements = String::Split(line, ' ');
			for (size_t i = 1; i < elements.size(); i++)
			{
				std::vector<std::string> in = String::Split(elements[i], '/');
				indices.push_back(std::stoi(in[0]) - 1);
			}
		}
	}

	mesh.Init(vertices, indices);
	return true;
}
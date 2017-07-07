#include <Odin/ColladaLoader.h>
#include <Odin/File.h>
#include <Odin/String.h>

#include <tinyxml2.h>

using namespace tinyxml2;

bool ColladaLoader::Load(const std::string& filename, Mesh &mesh)
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	XMLDocument doc;
	doc.LoadFile(filename.c_str());
	const char* vertexString = doc.FirstChildElement("COLLADA")->FirstChildElement("library_geometries")->
		FirstChildElement("geometry")->FirstChildElement("mesh")->
		FirstChildElement("source")->FirstChildElement("float_array")->GetText();
	std::vector<std::string> values = String::Split(std::string(vertexString), ' ');
	for (auto val : values)
	{
		vertices.push_back(std::stof(val));
	}

	const char* indexString = doc.FirstChildElement("COLLADA")->FirstChildElement("library_geometries")->
		FirstChildElement("geometry")->FirstChildElement("mesh")->
		FirstChildElement("polylist")->FirstChildElement("p")->GetText();
	values = String::Split(std::string(indexString), ' ');
	for (size_t i = 0; i < values.size(); i += 4)
	{
		indices.push_back(std::stoi(values[i]));
	}

	mesh.Init(vertices, indices);
	return true;
}
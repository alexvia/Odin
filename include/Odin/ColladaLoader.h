#pragma once

#include <Odin/Joint.h>
#include <Odin/Mesh.h>

#include <tinyxml2.h>

class ColladaLoader
{
public:
	bool Load(const std::string& filename, Mesh &mesh);

private:
	void LoadGeometyData(tinyxml2::XMLNode* geometries, std::vector<float>& vertices, std::vector<unsigned int>& indices);
	void LoadAnimationData(tinyxml2::XMLNode* animations);
	void LoadSkinningData(tinyxml2::XMLNode* controllers, tinyxml2::XMLNode* scenes);

	Joint LoadJoints(tinyxml2::XMLElement *node);
	Joint GetJointData(tinyxml2::XMLElement *node);
};

#pragma once

#include <Odin/Mesh.h>

class ColladaLoader
{
public:
	bool Load(const std::string& filename, Mesh &mesh);
};
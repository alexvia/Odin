#pragma once

#include <Odin/Mesh.h>

class OBJLoader
{
public:
	bool Load(const std::string& filename, Mesh &mesh);
};
#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

class Joint
{
public:
	Joint(const std::string& name, const glm::mat4& transform);
	void AddChild(Joint child);

private:
	std::string name;
	glm::mat4 transform;
	std::vector<Joint> children;
};

#include <Odin/Joint.h>

Joint::Joint(const std::string& name, const glm::mat4& transform)
{
	this->name = name;
	this->transform = transform;
}

void Joint::AddChild(Joint child)
{
	children.push_back(child);
}

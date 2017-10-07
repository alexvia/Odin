#pragma once

#include <string>

class XmlAttribute
{
private:
	std::string name;
	std::string value;

public:	
	XmlAttribute(const std::string &name, const std::string &value);
};
#pragma once

#include "XmlNode.h"

#include <string>

class XmlParser
{
private:
	XmlNode *root = nullptr;

public:
	//XmlParser();
	//~XmlParser();

	XmlNode* Load(const std::string &filename);

private:
	XmlNode* Parse(std::stringstream &stream);

	std::string RemoveAngleBrackets(const std::string &s);
};
#pragma once

#include "XmlAttribute.h"

#include <string>
#include <vector>

class XmlNode
{
private:
	//XmlNode *root = nullptr;

	std::string name;
	std::string data;
	std::vector<XmlAttribute> attributes;

	XmlNode *parent = nullptr;
	std::vector<XmlNode*> children;

public:
	XmlNode(const std::string &name);
	~XmlNode();

	void AddAttributes(const std::vector<std::string> &parts);
	void AddData(const std::string &data);
	void AddChild(XmlNode *child);
	const std::string& GetData() const;
};
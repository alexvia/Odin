#include <Util/XmlNode.h>
#include <Util/String.h>

#include <regex>

XmlNode::XmlNode(const std::string &name)
	: name(name)
{}

XmlNode::~XmlNode()
{
	for (size_t i = 0; i < children.size(); i++)
		delete children[i];
}

void XmlNode::AddAttributes(const std::vector<std::string> &parts)
{
	for (size_t i = 0; i < parts.size(); i++)
	{
		auto nameValue = String::Split(parts[i], '=');
		std::regex regex(R"x("(.*)".*)x");
		std::smatch match;
		std::regex_match(nameValue[1], match, regex);
		attributes.emplace_back(nameValue[0], match.str(1));
	}
}

void XmlNode::AddData(const std::string &data)
{
	size_t i = 0;
	while (data[i] != '>')
	{
		i++;
		if (i >= data.length() - 1)
			return;
	}
	i++;
	while (data[i] != '<')
	{
		this->data += data[i];
		i++;
	}
}

void XmlNode::AddChild(XmlNode *child)
{
	child->parent = this;
	children.push_back(child);
}

const std::string& XmlNode::GetData() const
{
	return data;
}

#include <Util/XmlParser.h>
#include <Util/File.h>
#include <Util/String.h>

#include <sstream>

XmlNode* XmlParser::Load(const std::string &filename)
{
	// Read the file contents
	std::string contents = File::Read(filename);
	
	// Create a string stream
	std::stringstream stream(contents);

	// Skip the first line
	std::getline(stream, std::string());

	return Parse(stream);
}

XmlNode* XmlParser::Parse(std::stringstream &stream)
{
	std::string line;
	std::getline(stream, line);
	String::Trim(line);

	if (String::StartsWith(line, "</"))
		return nullptr;

	// Remove <, > and <? , >
	std::string tag = RemoveAngleBrackets(line);

	// Split tag into attributes and parse them
	std::vector<std::string> parts = String::Split(tag, ' ');

	XmlNode *node = new XmlNode(parts[0]);
	parts.erase(parts.begin());
	node->AddAttributes(parts);
	node->AddData(line);

	if (line.find("</") != std::string::npos || line.find("/>") != std::string::npos)
		return node;

	XmlNode *child = nullptr;
	while ((child = Parse(stream)) != nullptr)
	{
		node->AddChild(child);
	}

	return node;
}

// TODO: Rewrite this!
std::string XmlParser::RemoveAngleBrackets(const std::string &s)
{
	std::string result;
	size_t i = 0;
	if (s[i] == '<')
		i++;
	while (s.length() > i && s[i] != '>')
	{
		if (i == 1 && s[i] == '?')
		{
			i++;
			continue;
		}

		result += s[i++];
	}

	return result;
}
#include <Odin/String.h>

namespace String
{
	std::vector<std::string> Split(const std::string& contents, char separator)
	{
		std::vector<std::string> list;
		std::istringstream stream(contents);
		std::string string;
		while (std::getline(stream, string, separator)) {
			list.push_back(string);
		}

		return list;
	}

	bool StartsWith(const std::string& string, const std::string& pattern)
	{
		return string.find(pattern) == 0;
	}
}
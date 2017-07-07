#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace String
{
	std::vector<std::string> Split(const std::string& contents, char separator);
	bool StartsWith(const std::string& string, const std::string& pattern);
}
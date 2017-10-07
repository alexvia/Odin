#pragma once

#include <string>

namespace File
{
	std::string Read(const std::string& filename);
	void Write(const std::string& filename, const std::string& contents);
}
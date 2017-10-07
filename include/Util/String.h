#pragma once

#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

namespace String
{
	std::vector<std::string> Split(const std::string& contents, char separator);
	bool StartsWith(const std::string& string, const std::string& pattern);
	
	// trim from start (in place)
	static inline void LTrim(std::string &s) 
	{
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
			return !std::isspace(ch);
		}));
	}

	// trim from end (in place)
	static inline void RTrim(std::string &s) 
	{
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
			return !std::isspace(ch);
		}).base(), s.end());
	}

	// trim from both ends (in place)
	static inline void Trim(std::string &s) 
	{
		LTrim(s);
		RTrim(s);
	}
}
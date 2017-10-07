#include <Odin/Vector.h>
#include <Util/String.h>

#include <algorithm>

#include <glm/gtc/type_ptr.hpp>

std::vector<float> GetFloatList(const char* text)
{
	std::vector<std::string> texts = String::Split(text, ' ');
	std::vector<float> values(texts.size());
	std::transform(texts.begin(), texts.end(), values.begin(), [](const std::string& val)
	{
		return stof(val);
	});
	return values;
}

std::vector<int> GetIntList(const char* text)
{
	std::vector<std::string> texts = String::Split(text, ' ');
	std::vector<int> values(texts.size());
	std::transform(texts.begin(), texts.end(), values.begin(), [](const std::string& val)
	{
		return stoi(val);
	});
	return values;
}

std::vector<glm::mat4> GetMatrixList(const char* text)
{
	std::vector<float> values = GetFloatList(text);
	std::vector<glm::mat4> transforms(values.size() / 16);
	for (size_t i = 0; i < values.size(); i += 16)
	{
		transforms[i / 16] = glm::make_mat4(&values[i]);
	}
	return transforms;
}

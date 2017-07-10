#pragma once

#include <vector>

#include <glm/glm.hpp>

std::vector<float> GetFloatList(const char* text);
std::vector<int> GetIntList(const char* text);
std::vector<glm::mat4> GetMatrixList(const char* text);

#ifndef UTILS_HPP
#define UTILS_HPP

#include <windows.h>
#include <glm/glm.hpp>

BOOL WriteToConsole(const wchar_t* msg);
BOOL WriteToConsole(const char* msg);
float magnitude(glm::vec3 v);
glm::vec3 normalize(glm::vec3 v);

#endif
#pragma once

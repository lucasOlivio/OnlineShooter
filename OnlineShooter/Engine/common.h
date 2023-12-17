#pragma once

#include <glew/glew.h>
#include <freeglut/freeglut.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <map>
#include <iostream>

const int MAX_PLAYERS = 4;

const glm::vec3 UP_VECTOR(0.f, 1.f, 0.f);
const glm::vec3 FORWARD_VECTOR(0.f, 0.f, -1.f);
const glm::vec3 RIGHT_VECTOR(0.f, -1.f, 0.f);

void _CheckGLError(const char* file, int line);

#define CheckGLError() _CheckGLError(__FILE__, __LINE__)

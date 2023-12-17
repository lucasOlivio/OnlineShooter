#pragma once

#include <glew/glew.h>
#include <freeglut/freeglut.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <map>
#include <iostream>

const int MAX_PLAYERS = 4;

void _CheckGLError(const char* file, int line);

#define CheckGLError() _CheckGLError(__FILE__, __LINE__)

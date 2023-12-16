#pragma once

#include "Shader.h"
#include "System/Component.h"
#include <common.h>

class ShaderProgram {
public:
	ShaderProgram(const char* vertexFilepath, const char* fragmentFilepath);
	~ShaderProgram();

	std::string name;
	Shader vertexShader;
	Shader fragmentShader;
	GLuint id;
};

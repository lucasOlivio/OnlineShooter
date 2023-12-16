#pragma once

#include <common.h>

struct Shader {
	std::string filename;
	std::string source;
	bool bIsCompiled;
	GLuint id;
};

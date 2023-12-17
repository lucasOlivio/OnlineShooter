#pragma once

#include "System/Component.h"

#include <common.h>

struct MeshRendererComponent : public Component
{
public:
	MeshRendererComponent() { }
	MeshRendererComponent(GLuint vbo, unsigned int numTriangles, glm::vec3 color) :vbo(vbo), numTriangles(numTriangles), color(color) { }
	GLuint vbo = 0;
	unsigned int numTriangles = 0;
	glm::vec3 color = glm::vec3(1.f);
};

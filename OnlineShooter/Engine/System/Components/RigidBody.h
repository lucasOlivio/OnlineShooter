#pragma once

#include "System/Component.h"

#include <common.h>

struct RigidBodyComponent : public Component
{
public:
	RigidBodyComponent() { }
	RigidBodyComponent(const glm::vec3& v, float r)
		: velocity(v), radius(r){ }

	glm::vec3 velocity;

	// Only circle type components to keep it simple
	float radius;
};

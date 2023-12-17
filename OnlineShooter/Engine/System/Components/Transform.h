#pragma once


#include "System/Component.h"

#include <common.h>

#include <glm/gtx/quaternion.hpp>

struct TransformComponent : public Component
{
public:
	TransformComponent() { }
	TransformComponent(const glm::vec3& p, const glm::vec3& s, const glm::quat& r)
		: position(p), scale(s), orientation(r) { }

	glm::vec3 position;
	glm::vec3 scale;
	glm::quat orientation;

	void AdjustOrientation(glm::vec3 value)
	{
		using namespace glm;

		// To combine quaternion values, you multiply them together
		// Make a quaternion that represents that CHANGE in angle
		quat qChange = quat(radians(value));

		// Multiply them together to get the change
		// Just like with matrix math
		orientation *= qChange;
	}

	glm::vec3 GetRightVector()
	{
		return orientation * RIGHT_VECTOR;
	}

	glm::vec3 GetForwardVector()
	{
		return orientation * FORWARD_VECTOR;
	}
};

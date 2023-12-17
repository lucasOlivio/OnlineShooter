#pragma once

#include "System/Component.h"

#include <common.h>

struct PlayerControllerComponent : public Component
{
public:
	PlayerControllerComponent() { }

	bool moveForward = false;
	bool moveBackward = false;
	bool moveLeft = false;
	bool moveRight = false;
	bool shoot = false;
	bool hasammo = true;

	float movementSpeed = 10.0f;
	float rotationSpeed = 100.0f;

	int bulletId = -1;
};

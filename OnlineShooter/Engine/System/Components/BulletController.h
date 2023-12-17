#pragma once

#include "System/Component.h"

#include <common.h>

struct BulletControllerComponent : public Component
{
public:
	BulletControllerComponent() { }
	BulletControllerComponent(int Id): bulletId(Id) { }

	float movementSpeed = 50.0f;

	float maxX = 600;
	float maxY = 400;
	float minX = -600;
	float minY = -400;

	int bulletId = -1;
};

#pragma once

#include "System/Component.h"

#include <common.h>

struct BulletControllerComponent : public Component
{
public:
	BulletControllerComponent() { }
	BulletControllerComponent(int Id): bulletId(Id) { }

	float movementSpeed = 15.0f;

	float maxX = 60;
	float maxY = 70;
	float minX = -60;
	float minY = -70;

	int bulletId = -1;
};

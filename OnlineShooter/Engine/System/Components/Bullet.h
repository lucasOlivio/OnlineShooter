#pragma once

#include <common.h>

#include <system/component.h>

class BulletComponent : public Component
{
public:
	BulletComponent(bool owner) : IsOwner(owner) { }

	bool IsOwner = false;

	// Custom state to be handled by the custom network system
	int state = 0;
};

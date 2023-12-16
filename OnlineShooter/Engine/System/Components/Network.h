#pragma once

#include <common.h>

#include <system/component.h>

class NetworkComponent : public Component
{
public:
	NetworkComponent(bool owner) : IsOwner(owner) { }

	bool IsOwner = false;

	// Custom state to be handled by the custom network system
	int state = 0;
};
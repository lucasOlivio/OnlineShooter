#pragma once

#include <common.h>

#include <system/component.h>

class NetworkComponent : public Component
{
public:
	NetworkComponent(bool owner) : IsOwner(owner) { }

	bool IsOwner = false;
};

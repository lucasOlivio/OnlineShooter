#pragma once

#include "Entity.h"
#include <common.h>

class iSystem
{
public:
	iSystem() = default;
	virtual ~iSystem() { }

	virtual void Execute(const std::vector<Entity*>& entities, float dt) = 0;
};

#pragma once

#include "Entity.h"
#include <common.h>

class iSystem
{
public:
	iSystem() = default;
	virtual ~iSystem() { }

	virtual bool Start(const std::vector<Entity*>& entities) = 0;

	virtual void Execute(const std::vector<Entity*>& entities, float dt) = 0;

	virtual void End() = 0;
};

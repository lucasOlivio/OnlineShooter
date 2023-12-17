#pragma once

#include <common.h>

#include "System.h"
#include "Entity.h"


class PlayerMovementSystem : public iSystem
{
public:
	void Execute(const std::vector<Entity*>& entities, float dt) override;

	// Inherited via iSystem
	bool Start(const std::vector<Entity*>& entities, int argc, char** argv) override;
	void End() override;
};

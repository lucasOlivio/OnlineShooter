#pragma once

#include <common.h>

#include "System/System.h"
#include "System/Entity.h"

enum eInputType
{
    FORWARD = 119,    // W
    BACKWARD = 115,   // S
    TURN_LEFT = 97,   // A
    TURN_RIGHT = 100, // D
    SHOOT = 32      // SPACE
};

class PlayerSystem : public iSystem
{
public:
	void Execute(const std::vector<Entity*>& entities, float dt) override;

	// Inherited via iSystem
	bool Start(const std::vector<Entity*>& entities, int argc, char** argv) override;
	void End() override;
};

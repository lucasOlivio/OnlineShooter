#pragma once

#include <common.h>

#include "System/Components/PlayerController.h"
#include "System/Components/Transform.h"
#include "System/Components/RigidBody.h"
#include "System/System.h"
#include "System/Entity.h"
#include "Engine/Engine.h"

enum eInputType
{
    FORWARD = 119,    // W
    BACKWARD = 115,   // S
    TURN_LEFT = 97,   // A
    TURN_RIGHT = 100, // D
    SHOOT = 32      // SPACE
};

// TODO: Update flags and handle flags should be separate systems
class PlayerSystem : public iSystem
{
public:
	PlayerSystem(bool hasInput) : m_hasInput(hasInput) {};

	void Execute(const std::vector<Entity*>& entities, float dt) override;
	void UpdateFlags(PlayerControllerComponent* playerController);
	void HandleFlags(RigidBodyComponent* rBody,
					 PlayerControllerComponent* playerController,
					 TransformComponent* transformConponent,
					 Entity* Bullet,
					 bool bulletActive,
					 float dt);
	Entity* findBullet(const std::vector<Entity*>& entities);
	bool isBulletActiveById(const std::vector<Entity*>& entities, int id);

	// Inherited via iSystem
	bool Start(const std::vector<Entity*>& entities, int argc, char** argv) override;
	void End() override;

private:
	bool m_hasInput;
};

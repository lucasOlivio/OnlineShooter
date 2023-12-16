#pragma once

#include "System/System.h"
#include "System/Components/RigidBody.h"
#include "System/Components/Transform.h"

class PhysicsSystem : iSystem
{
public:
	PhysicsSystem() {};
	virtual ~PhysicsSystem() { };

	virtual bool Start(const std::vector<Entity*>& entities, int argc, char** argv);

	virtual void Execute(const std::vector<Entity*>& entities, float dt);

	virtual void End();

	void ApplyForce(TransformComponent* pTransform, RigidBodyComponent* pBody);

	bool CheckCollision(TransformComponent* pTransform, RigidBodyComponent* pBody);
};

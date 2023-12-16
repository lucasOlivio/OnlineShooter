#pragma once

#include "System/System.h"
#include "System/Components/RigidBody.h"
#include "System/Components/Transform.h"

class PhysicsSystem : iSystem
{
public:
	PhysicsSystem() {};
	virtual ~PhysicsSystem() { };

	virtual void Execute(const std::vector<Entity*>& entities, float dt);

	void ApplyForce(TransformComponent* pTransform, RigidBodyComponent* pBody);

	bool CheckCollision(TransformComponent* pTransform, RigidBodyComponent* pBody);
};

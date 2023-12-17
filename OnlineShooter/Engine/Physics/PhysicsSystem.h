#pragma once

#include "System/System.h"
#include "System/Components/RigidBody.h"
#include "System/Components/Transform.h"

class PhysicsSystem : public iSystem
{
public:
	PhysicsSystem(bool resolveCollision) : m_resolveCollision(resolveCollision) {};
	virtual ~PhysicsSystem() { };

	virtual bool Start(const std::vector<Entity*>& entities, int argc, char** argv);

	virtual void Execute(const std::vector<Entity*>& entities, float dt);

	virtual void End();

	void ApplyForce(TransformComponent* pTransform, RigidBodyComponent* pBody, float dt);

	// If collision true returns the entity that collided with on "pEntityBOut" 
	bool CheckCollision(const std::vector<Entity*>& entities, 
						int entityId,
						TransformComponent* pTransformA,
						RigidBodyComponent* pBodyA,
						Entity& entityBOut);

	// Changes the entity state based on what it collided
	void ResolveCollision(Entity* entityA, Entity* entityB);

	static bool SphereSphereCollision(glm::vec3 positionA, float radiusA, 
									  glm::vec3 positionB, float radiusB);

private:
	// Keep track of visited entities on the frame 
	// to avoid checking same collision twice
	std::vector<int> m_visitedEntities;

	// Should deal with the collision detected or not
	// to avoid player dying on the client side
	bool m_resolveCollision;
};

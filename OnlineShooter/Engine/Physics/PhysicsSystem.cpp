#include "PhysicsSystem.h"

bool PhysicsSystem::Start(const std::vector<Entity*>& entities)
{
    return false;
}

void PhysicsSystem::Execute(const std::vector<Entity*>& entities, float dt)
{
}

void PhysicsSystem::End()
{
}

void PhysicsSystem::ApplyForce(TransformComponent* pTransform, RigidBodyComponent* pBody)
{
}

bool PhysicsSystem::CheckCollision(TransformComponent* pTransform, RigidBodyComponent* pBody)
{
    return false;
}

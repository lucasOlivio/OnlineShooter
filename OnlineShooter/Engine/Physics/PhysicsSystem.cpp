#include "PhysicsSystem.h"

bool PhysicsSystem::Start(const std::vector<Entity*>& entities, int argc, char** argv)
{
    return true;
}

void PhysicsSystem::Execute(const std::vector<Entity*>& entities, float dt)
{
    for (int i = 0; i < entities.size(); i++)
    {
        Entity* pEntity = entities[i];
        if (!pEntity->HasComponent<TransformComponent>() ||
            !pEntity->HasComponent<RigidBodyComponent>())
        {
            continue;
        }

        TransformComponent* pTransform = pEntity->GetComponent<TransformComponent>();
        RigidBodyComponent* pBody = pEntity->GetComponent<RigidBodyComponent>();

        ApplyForce(pTransform, pBody, dt);

        Entity* pEntityBOut = nullptr;
        bool isColliding = CheckCollision(entities, i,
						                  pTransform, 
						                  pBody,
						                  pEntityBOut);
        if (isColliding)
        {
            ResolveCollision(pEntity, pEntityBOut);
        }
    }
}

void PhysicsSystem::End()
{
}

void PhysicsSystem::ApplyForce(TransformComponent* pTransform, 
                               RigidBodyComponent* pBody, float dt)
{
    glm::vec3 position = pTransform->position;
    glm::vec3 velocity = pBody->velocity;

    glm::vec3 newPosition = position + (velocity * dt);

    pTransform->position = newPosition;

    return;
}

bool PhysicsSystem::CheckCollision(const std::vector<Entity*>& entities, 
						           int entityId,
						           TransformComponent* pTransformA, 
						           RigidBodyComponent* pBodyB,
						           Entity* pEntityBOut)
{
    bool isColliding = false;
    for (int entityBId : m_visitedEntities)
    {
        Entity* pEntityB = entities[entityBId];
        TransformComponent* pTransformB = pEntityB->GetComponent<TransformComponent>();
        RigidBodyComponent* pBodyB = pEntityB->GetComponent<RigidBodyComponent>();

        isColliding = SphereSphereCollision(pTransformA->position, pBodyB->radius,
                                            pTransformB->position, pBodyB->radius);

        if (isColliding)
        {
            pEntityBOut = pEntityB;
            // Only deal with one collision for now
            break;
        }
    }

    m_visitedEntities.push_back(entityId);

    return isColliding;
}

void PhysicsSystem::ResolveCollision(Entity* entityA, Entity* entityB)
{
    if (entityA->tag == entityB->tag)
    {
        // Only deal with bullet-player collision
        return;
    }

    Entity* pEntityToKill = nullptr;
    if (entityA->tag == "player" && entityB->tag == "bullet")
    {
        pEntityToKill = entityA;
    }
    else if (entityB->tag == "player" && entityA->tag == "bullet")
    {
        pEntityToKill = entityB;
    }

    pEntityToKill->state = StatetType::IS_DEAD;

    return;
}

bool PhysicsSystem::SphereSphereCollision(glm::vec3 positionA, float radiusA, glm::vec3 positionB, float radiusB)
{
    // Calculate squared distance between centers
    glm::vec3 d = positionA - positionB;
    float dist2 = glm::dot(d, d);
    // Spheres intersect if squared distance is less than squared sum of radii
    float radiusSum = radiusA + radiusB;
    if (dist2 > radiusSum * radiusSum)
    {
        // Not hit
        return false;
    }

    return true;
}

#include "BulletSystem.h"
#include "System/Components/BulletController.h"
#include "System/Components/Transform.h"
#include "System/Components/RigidBody.h"

#include "Engine/Engine.h"

void BulletSystem::Execute(const std::vector<Entity*>& entities, float dt)
{
	for (int i = 0; i < entities.size(); i++)
	{
		Entity* entity = entities[i];

		if (entity->state == StatetType::NOT_ACTIVE ||
			entity->state == StatetType::IS_DEAD)
		{
			continue;
		}

		if (!entity->HasComponent<BulletControllerComponent>() ||
			!entity->HasComponent<TransformComponent>() ||
			!entity->HasComponent<RigidBodyComponent>())
		{
			continue;
		}
		Input keyInput = GetEngine().GetInput();
		RigidBodyComponent* rBody = entity->GetComponent<RigidBodyComponent>();
		BulletControllerComponent* bulletController = entity->GetComponent<BulletControllerComponent>();
		TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();

		glm::vec3 position = transformComponent->position;
		if (position.x > bulletController->maxX ||
			position.y > bulletController->maxY ||
			position.x < bulletController->minX ||
			position.y < bulletController->minY)
		{
			entity->state = StatetType::NOT_ACTIVE;
		}
	}
}

bool BulletSystem::Start(const std::vector<Entity*>& entities, int argc, char** argv)
{
    return false;
}

void BulletSystem::End()
{
}

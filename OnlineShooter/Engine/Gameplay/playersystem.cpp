#include "playersystem.h"

#include "System/Components/PlayerController.h"
#include "System/Components/Transform.h"


void PlayerSystem::Execute(const std::vector<Entity*>& entities, float dt)
{
	for (int i = 0; i < entities.size(); i++)
	{
		Entity* entity = entities[i];

		if (entity->state == StatetType::NOT_ACTIVE ||
			entity->state == StatetType::IS_DEAD)
		{
			continue;
		}

		if (!entity->HasComponent<PlayerControllerComponent>() ||
			!entity->HasComponent<TransformComponent>())
		{
			continue;
		}

		// If input forward, apply velocity forward to the entity direction

		// If input backward, apply velocity backwards to the entity direction

		// If input turn left, rotate entity to the left

		// If input turn right, rotate entity to the right

		// If input action and player have ammo, create a bullet entity in front of entity
		// with a velocity relative to the forward direction of entity, 
		// then set player ammo to false
	}
}

bool PlayerSystem::Start(const std::vector<Entity*>& entities, int argc, char** argv)
{
	return true;
}

void PlayerSystem::End()
{
}

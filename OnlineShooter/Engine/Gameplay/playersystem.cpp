#include "playersystem.h"

#include "System/Components/PlayerController.h"
#include "System/Components/Transform.h"


void PlayerSystem::Execute(const std::vector<Entity*>& entities, float dt)
{
	PlayerControllerComponent* controller = nullptr;
	TransformComponent* transform = nullptr;

	const float speed = 7.f;

	for (int i = 0; i < entities.size(); i++)
	{
		Entity* entity = entities[i];

		if (!entity->HasComponent<PlayerControllerComponent>() || 
			!entity->HasComponent<TransformComponent>())
		{
			continue;
		}

		controller = entity->GetComponent<PlayerControllerComponent>();
		transform = entity->GetComponent<TransformComponent>();

		glm::vec3 direction = glm::vec3(0.f);

		direction.z += controller->moveLeft ? -1.f : 0.f;
		//direction.z += controller->moveRight ? 1.f : 0.f;
		direction.z += controller->moveRight ? 1.0f : 0.0f;
		direction.x += controller->moveForward ? 1.f : 0.f;
		direction.x += controller->moveBackward ? -1.f : 0.f;
		transform->position += direction * speed * dt;
	}

	// If input forward, apply velocity forward to the entity direction

	// If input backward, apply velocity backwards to the entity direction

	// If input turn left, rotate entity to the left

	// If input turn right, rotate entity to the right

	// If input action and player have ammo, create a bullet entity in front of entity
	// with a velocity relative to the forward direction of entity, 
	// then set player ammo to false
}

bool PlayerSystem::Start(const std::vector<Entity*>& entities, int argc, char** argv)
{
	return true;
}

void PlayerSystem::End()
{
}

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
}

bool PlayerSystem::Start(const std::vector<Entity*>& entities, int argc, char** argv)
{
	return true;
}

void PlayerSystem::End()
{
}

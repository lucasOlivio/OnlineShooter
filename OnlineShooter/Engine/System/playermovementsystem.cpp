#include "playermovementsystem.h"

#include "Components/PlayerController.h"
#include "Components/Transform.h"


void PlayerMovementSystem::Execute(const std::vector<Entity*>& entities, float dt)
{
	PlayerControllerComponent* controller = nullptr;
	TransformComponent* transform = nullptr;

	const float speed = 7.f;

	for (int i = 0; i < entities.size(); i++)
	{
		Entity* entity = entities[i];
		controller = entity->GetComponent<PlayerControllerComponent>();
		transform = entity->GetComponent<TransformComponent>();


		if (controller != nullptr && transform != nullptr)
		{
			glm::vec3 direction = glm::vec3(0.f);

			direction.z += controller->moveLeft ? -1.f : 0.f;
			//direction.z += controller->moveRight ? 1.f : 0.f;
			direction.z += controller->moveRight ? 1.0f : 0.0f;
			direction.x += controller->moveForward ? 1.f : 0.f;
			direction.x += controller->moveBackward ? -1.f : 0.f;
			transform->position += direction * speed * dt;
		}
	}
}

bool PlayerMovementSystem::Start(const std::vector<Entity*>& entities, int argc, char** argv)
{
	return true;
}

void PlayerMovementSystem::End()
{
}

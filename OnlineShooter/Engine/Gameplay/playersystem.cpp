#include "PlayerSystem.h"

#include "System/Components/PlayerController.h"
#include "System/Components/Transform.h"
#include "System/Components/RigidBody.h"
#include "Engine/Engine.h"


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
			!entity->HasComponent<TransformComponent>() || 
			!entity->HasComponent<RigidBodyComponent>())
		{
			continue;
		}
		Input keyInput = GetEngine().GetInput(); 
		RigidBodyComponent* rBody = entity->GetComponent<RigidBodyComponent>();
		PlayerControllerComponent* playerController = entity->GetComponent<PlayerControllerComponent>();
		TransformComponent* transformConponent = entity->GetComponent<TransformComponent>();


		// If input forward, apply velocity forward to the entity direction
		if (keyInput.IsKeyPressed(eInputType::FORWARD)) {
			rBody->velocity = playerController->movementSpeed * transformConponent->GetForwardVector();
			playerController->moveForward = true;
		}
		// If input backward, apply velocity backwards to the entity direction
		if (keyInput.IsKeyPressed(eInputType::BACKWARD)) {
			rBody->velocity = playerController->movementSpeed * -(transformConponent->GetForwardVector());
			playerController->moveForward = true;
		}
		// If input turn left, rotate entity to the left
		if (keyInput.IsKeyPressed(eInputType::TURN_LEFT)) {
			glm::vec3 velocity = playerController->rotationSpeed * -(transformConponent->GetRightVector());
			transformConponent->AdjustOrientation(velocity);
			playerController->moveLeft = true;
		}
		// If input turn right, rotate entity to the right
		if (keyInput.IsKeyPressed(eInputType::TURN_RIGHT)) {
			glm::vec3 velocity = playerController->rotationSpeed * transformConponent->GetRightVector();
			transformConponent->AdjustOrientation(velocity);
			playerController->moveRight = true;
		}

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

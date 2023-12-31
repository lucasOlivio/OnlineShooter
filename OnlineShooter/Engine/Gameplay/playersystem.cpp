#include "PlayerSystem.h"
#include "System/Components/BulletController.h"
#include "System/Components/MeshRenderer.h"


void PlayerSystem::UpdateFlags(PlayerControllerComponent* playerController) {
	Input keyInput = GetEngine().GetInput();

	// If input forward, apply velocity forward to the entity direction
		playerController->moveForward = keyInput.IsKeyPressed(eInputType::FORWARD);

	// If input backward, apply velocity backwards to the entity direction

		playerController->moveBackward = keyInput.IsKeyPressed(eInputType::BACKWARD);

	// If input turn left, rotate entity to the left
		playerController->moveLeft = keyInput.IsKeyPressed(eInputType::TURN_LEFT);

	// If input turn right, rotate entity to the right
		playerController->moveRight = keyInput.IsKeyPressed(eInputType::TURN_RIGHT);

	// If input action and player have ammo, create a bullet entity in front of entity
	// with a velocity relative to the forward direction of entity, 
	// then set player ammo to false
		playerController->shoot = keyInput.IsKeyPressed(eInputType::SHOOT);
}

void PlayerSystem::HandleFlags(RigidBodyComponent* rBody, 
							   PlayerControllerComponent* playerController, 
							   TransformComponent* transformConponent, 
							   Entity* Bullet,
							   bool bulletActive,
							   float dt) {
	bool isMoving = false;
	// If input forward, apply velocity forward to the entity direction
	if (playerController->moveForward) {
		rBody->velocity = playerController->movementSpeed * transformConponent->GetForwardVector();
		isMoving = true;
	}
	// If input backward, apply velocity backwards to the entity direction
	if (playerController->moveBackward) {
		rBody->velocity = playerController->movementSpeed * -(transformConponent->GetForwardVector());
		isMoving = true;
	}

	if (!isMoving)
	{
		rBody->velocity = glm::vec3(0);
	}

	// If input turn left, rotate entity to the left
	if (playerController->moveRight) {
		glm::vec3 velocity = playerController->rotationSpeed * -(glm::vec3(0, 1.0,0) * dt);
		transformConponent->AdjustOrientation(velocity);
	}
	// If input turn right, rotate entity to the right
	if (playerController->moveLeft) {
		glm::vec3 velocity = playerController->rotationSpeed * glm::vec3(0, 1.0, 0) * dt;
		transformConponent->AdjustOrientation(velocity);
	}

	// If input action and player have ammo, create a bullet entity in front of entity
	// with a velocity relative to the forward direction of entity, 
	// then set player ammo to false
	if (playerController->shoot && Bullet != nullptr && playerController->hasammo) {

		glm::vec3 direction = transformConponent->GetForwardVector();
		RigidBodyComponent* rBullet = Bullet->GetComponent<RigidBodyComponent>();
		TransformComponent* transBullet = Bullet->GetComponent<TransformComponent>();
		BulletControllerComponent* bulletController = Bullet->GetComponent<BulletControllerComponent>();
		//GetRenderSystem();
		//RenderSystem* pRender = GetEngine().GetSystem(1);
		//const glm::vec3 bulletColor = glm::vec3(1.f, 0.5f, 0.5f);
		//Bullet->AddComponent<MeshRendererComponent>(pRenderSystem->models["sphere"].Vbo,
		//pRenderSystem->models["sphere"].NumTriangles,
		//bulletColor);

		glm::vec3 velocity = bulletController->movementSpeed * direction;
		rBullet->velocity = velocity;
		transBullet->position = transformConponent->position + (direction * (rBody->radius + rBullet->radius + 0.2f));

		playerController->hasammo = false;
		playerController->bulletId = bulletController->bulletId;

		Bullet->state = StatetType::IS_ACTIVE;
	}
	else if (!playerController->hasammo && !bulletActive) {
		playerController->hasammo = true;
	}


}

Entity* PlayerSystem::findBullet(const std::vector<Entity*>& entities)
{
	Entity* entity;
	for (int i = 0; i < entities.size(); i++)
	{
		entity = entities[i];
		if (entity->tag == "bullet" && entity->state == StatetType::NOT_ACTIVE) {
			return entity;
		}
	}
	return nullptr;
}

bool PlayerSystem::isBulletActiveById(const std::vector<Entity*>& entities, int id)
{
	Entity* entity;
	for (int i = 0; i < entities.size(); i++)
	{
		entity = entities[i];
		if (entity->tag == "bullet" && entity->GetComponent<BulletControllerComponent>()->bulletId) {
			if (entity->state = StatetType::NOT_ACTIVE) {
				return false;
			}
			else if (entity->state = StatetType::IS_ACTIVE) {
				return true;
			}
		}
	}
	return false;
}

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
			!entity->HasComponent<RigidBodyComponent>() )
		{
			continue;
		}
		RigidBodyComponent* rBody = entity->GetComponent<RigidBodyComponent>();
		PlayerControllerComponent* playerController = entity->GetComponent<PlayerControllerComponent>();
		TransformComponent* transformConponent = entity->GetComponent<TransformComponent>();
		Entity* bullet = findBullet(entities);

		if (m_hasInput)
		{
			UpdateFlags(playerController);
		}
		bool active = isBulletActiveById(entities, playerController->bulletId);
		HandleFlags(rBody, playerController, transformConponent, bullet, active, dt);
	}
}

bool PlayerSystem::Start(const std::vector<Entity*>& entities, int argc, char** argv)
{
	return true;
}

void PlayerSystem::End()
{
}

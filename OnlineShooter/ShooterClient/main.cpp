#include "Network/NetworkClient.h"
#include "System/components.h"
#include "Graphics/RenderSystem.h"
#include "Gameplay/PlayerSystem.h"
#include "Physics/PhysicsSystem.h"
#include "System/Components/PlayerController.h"
#include "System/Components/BulletController.h"

#include <Engine/Engine.h>

int main(int argc, char** argv)
{
	GetEngine().Initialize();

	// Setup systems
	ClientSystem* pClientSystem = new ClientSystem();
	RenderSystem* pRenderSystem = GetRenderSystem();
	PlayerSystem* pPlayerSystem = new PlayerSystem(true);
	PhysicsSystem* pPhysicsSystem = new PhysicsSystem(false);

	GetEngine().AddSystem(pClientSystem);
	GetEngine().AddSystem(pRenderSystem);
	GetEngine().AddSystem(pPlayerSystem);
	GetEngine().AddSystem(pPhysicsSystem);

	// Setup meshes
	pRenderSystem->InitGlut(argc, argv);
	pRenderSystem->LoadMeshes();

	// Setup all players
	const glm::vec3 origin(0.f);
	const glm::vec3 unscaled(1.f);
	const glm::quat identity(1.f, 0.f, 0.f, 0.f);
	const glm::vec3 playerColor = glm::vec3(1.f, 0.f, 0.f);
	const glm::vec3 bulletColor = glm::vec3(1.f, 0.5f, 0.5f);
	const float radius = 5;
	EntityManager* pEntityManager = GetEngine().GetEntityManager();
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		// Player #i
		Entity* player = pEntityManager->CreateEntity();
		player->tag = "player";
		player->AddComponent<RigidBodyComponent>(glm::vec3(0, 0, 0), radius);
		player->AddComponent<TransformComponent>(origin, unscaled, identity);
		player->AddComponent<NetworkComponent>(false);
		player->AddComponent<MeshRendererComponent>(pRenderSystem->models["sphere"].Vbo,
													pRenderSystem->models["sphere"].NumTriangles,
													playerColor);
	}

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		// Bullet #i
		Entity* bullet = pEntityManager->CreateEntity();
		bullet->tag = "bullet";
		bullet->AddComponent<RigidBodyComponent>(glm::vec3(0, -190, 0), 2);
		bullet->AddComponent<BulletControllerComponent>();
		bullet->AddComponent<TransformComponent>(origin, unscaled, identity);
		bullet->AddComponent<MeshRendererComponent>(pRenderSystem->models["sphere"].Vbo,
			pRenderSystem->models["sphere"].NumTriangles,
			bulletColor);
		bullet->state = StatetType::NOT_ACTIVE;
	}

	GetEngine().Run(argc, argv);

	GetEngine().Destroy();

	system("pause");

	return 0;
}
#include "Network/NetworkServer.h"
#include "System/components.h"
#include "Graphics/RenderSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Gameplay/PlayerSystem.h"

#include <Engine/Engine.h>

int main(int argc, char** argv)
{
	GetEngine().Initialize();

	// Setup systems
	ServerSystem* pServer = new ServerSystem();
	PlayerSystem* pPlayerSystem = new PlayerSystem();
	PhysicsSystem* pPhysicsSystem = new PhysicsSystem(true);

	GetEngine().AddSystem(pServer);
	GetEngine().AddSystem(pPlayerSystem);
	GetEngine().AddSystem(pPhysicsSystem);

	// Setup all players
	const glm::vec3 origin(0.f);
	const glm::vec3 unscaled(1.f);
	const glm::quat identity(1.f, 0.f, 0.f, 0.f);
	const float radius = 5;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		// Player #i
		Entity* player = GetEngine().GetEntityManager()->CreateEntity();
		player->tag = "player";
		player->AddComponent<RigidBodyComponent>(glm::vec3(0, 0, 0), radius);
		player->AddComponent<TransformComponent>(origin, unscaled, identity);
		player->AddComponent<NetworkComponent>(false);
		player->AddComponent<PlayerControllerComponent>();
	}

	GetEngine().Run(argc, argv);

	GetEngine().Destroy();

	system("pause");

	return 0;
}
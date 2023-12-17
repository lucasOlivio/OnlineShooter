#include "Network/NetworkServer.h"
#include "System/components.h"
#include "Graphics/RenderSystem.h"

#include <Engine/Engine.h>

int main(int argc, char** argv)
{
	GetEngine().Initialize();

	// Setup systems
	ServerSystem* pServer = new ServerSystem();

	GetEngine().AddSystem(pServer);

	// Setup all players
	const glm::vec3 origin(0.f);
	const glm::vec3 unscaled(1.f);
	const glm::quat identity(1.f, 0.f, 0.f, 0.f);
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		// Player #i
		Entity* player = GetEngine().GetEntityManager()->CreateEntity();
		player->AddComponent<TransformComponent>(origin, unscaled, identity);
		player->AddComponent<NetworkComponent>(false);
		player->AddComponent<PlayerControllerComponent>();
	}

	GetEngine().Run(argc, argv);

	GetEngine().Destroy();

	return 0;
}
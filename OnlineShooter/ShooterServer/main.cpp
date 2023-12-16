#include "Network/NetworkServer.h"
#include "System/components.h"

#include <Engine/Engine.h>

int main(int argc, char** argv)
{
	// Setup systems
	ServerSystem* pServer = new ServerSystem();

	GetEngine().AddSystem(pServer);

	// Setup all players
	Model sphere = GetEngine().models["sphere"];
	const glm::vec3 origin(0.f);
	const glm::vec3 unscaled(1.f);
	const glm::quat identity(1.f, 0.f, 0.f, 0.f);
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		// Player #i
		Entity* player = GetEngine().GetEntityManager()->CreateEntity();
		player->AddComponent<MeshRendererComponent>(sphere.Vbo, sphere.NumTriangles,
			glm::vec3(1.f, 0.f, 0.f));
		player->AddComponent<TransformComponent>(origin, unscaled, identity);
		player->AddComponent<NetworkComponent>(false);
	}

	GetEngine().Initialize(argc, argv);

	GetEngine().Run();

	GetEngine().Destroy();

	return 0;
}
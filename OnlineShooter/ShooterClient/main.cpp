#include "Network/NetworkClient.h"
#include "System/components.h"
#include "Graphics/RenderSystem.h"
#include "Gameplay/playersystem.h"
#include "System/Components/PlayerController.h"

#include <Engine/Engine.h>

int main(int argc, char** argv)
{
	GetEngine().Initialize();

	// Setup systems
	ClientSystem* pClient = new ClientSystem();
	RenderSystem* pRender = GetRenderSystem();
	PlayerSystem* pPSystem = new PlayerSystem();

	GetEngine().AddSystem(pClient);
	GetEngine().AddSystem(pRender);
	GetEngine().AddSystem(pPSystem);

	// Setup meshes
	pRender->InitGlut(argc, argv);
	pRender->LoadMeshes();

	// Setup all players
	const glm::vec3 origin(0.f);
	const glm::vec3 unscaled(1.f);
	const glm::quat identity(1.f, 0.f, 0.f, 0.f);
	const glm::vec3 playerColor = glm::vec3(1.f, 0.f, 0.f);
	const float radius = 5;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		// Player #i
		Entity* player = GetEngine().GetEntityManager()->CreateEntity();
		player->tag = "player";
		player->AddComponent<RigidBodyComponent>(glm::vec3(0, 0, 0), radius);
		player->AddComponent<TransformComponent>(origin, unscaled, identity);
		player->AddComponent<NetworkComponent>(false);
		player->AddComponent<MeshRendererComponent>(pRender->models["sphere"].Vbo,
													pRender->models["sphere"].NumTriangles,
													playerColor);
	}

	GetEngine().Run(argc, argv);

	GetEngine().Destroy();

	system("pause");

	return 0;
}

//TODO: put input in the render system form th engine
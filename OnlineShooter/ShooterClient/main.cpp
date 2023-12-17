#include "Network/NetworkClient.h"
#include "System/components.h"
#include "Graphics/RenderSystem.h"
#include "System/playermovementsystem.h"
#include "System/Components/PlayerController.h"

#include <Engine/Engine.h>

int main(int argc, char** argv)
{
	GetEngine().Initialize();

	// Setup systems
	RenderSystem* pRender = GetRenderSystem();
	std::vector<Entity*> entities;

	GetEngine().AddSystem(pRender);
	GetEngine().GetEntityManager()->GetEntities(entities);

	pRender->Start(entities, argc, argv);

	// Setup all players
	const glm::vec3 origin(0.f);
	const glm::vec3 unscaled(1.f);
	const glm::quat identity(1.f, 0.f, 0.f, 0.f);
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		// Player #i
		Entity* player = GetEngine().GetEntityManager()->CreateEntity();
		player->AddComponent<TransformComponent>(origin, unscaled, identity);
		//GetEngine();
		player->AddComponent<MeshRendererComponent>(pRender->models["sphere"].Vbo, pRender->models["sphere"].NumTriangles, glm::vec3(1.f, 0.f, 0.f));
		//player->AddComponent<MeshRendererComponent>();
		player->AddComponent<NetworkComponent>(false);
		player->AddComponent<PlayerControllerComponent>();
	}

	GetEngine().AddSystem(new PlayerMovementSystem());

	//glutIdleFunc(Engine_Callback(argc, argv));
	//glutIdleFunc(Engine_Callback);

	GetEngine().Run(argc, argv);

	GetEngine().Destroy();

	system("pause");

	return 0;
}

//TODO: put input in the render system form th engine
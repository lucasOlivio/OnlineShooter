#include "Engine.h"
#include "Graphics/RenderSystem.h"
#include "System/components.h"

#include <sstream>


Engine::Engine()
{
	m_pEntityManager = new EntityManager();
}

Engine::~Engine()
{
}

Engine& Engine::GetInstance()
{
	static Engine instance; // Singleton instance
	return instance;
}

void Engine::Initialize()
{
	m_LastTime = std::chrono::high_resolution_clock::now();
	
}

void Engine::Destroy()
{
	for (int i = 0; i < m_Systems.size(); i++)
	{
		m_Systems[i]->End();
	}

	delete m_pEntityManager;
}

void Engine::Run(int argc, char** argv)
{
	std::vector<Entity*> entities;
	m_pEntityManager->GetEntities(entities);

	for (int i = 0; i < m_Systems.size(); i++)
	{
		bool isStarted = m_Systems[i]->Start(entities, argc, argv);
		if (!isStarted)
		{
			printf("Engine: Error to start all systems!");
			exit(1);
		}
	}

	m_Running = true;
	while (m_Running)
	{
		Update();
	}
}

void Engine::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> dt = currentTime - m_LastTime;
	m_LastTime = currentTime;

	std::vector<Entity*> entities;
	m_pEntityManager->GetEntities(entities);

	for (int i = 0; i < m_Systems.size(); i++)
	{
		m_Systems[i]->Execute(entities, dt.count());
	}
}


void Engine::PressKey(unsigned char key)
{
	std::vector<Entity*> entities;
	m_pEntityManager->GetEntities(entities);
	m_Input.PressKey(key);
}

void Engine::ReleaseKey(unsigned char key)
{
	m_Input.ReleaseKey(key);
}

Input& Engine::GetInput()
{
	return m_Input;
}

void Engine::AddSystem(iSystem* system)
{
	m_Systems.push_back(system);
}

EntityManager* Engine::GetEntityManager()
{
	return m_pEntityManager;
}

void Engine::SetRunning(bool isRunning)
{
	m_Running = isRunning;
}

#pragma once

#include "System/System.h"
#include "System/Input.h"
#include "System/EntityManager.h"
#include <common.h>
#include <chrono>

const std::string ASSETS_PATH = "../assets/";

class Engine
{
public:
	// Singleton
	static Engine& GetInstance();

	// Lifecycle
	void Initialize();
	void Destroy();

	// Start systems and run main loop
	void Run(int argc, char** argv);

	// Update
	void Update();

	// Keyboard 
	void PressKey(unsigned char key);
	void ReleaseKey(unsigned char key);

	Input& GetInput();

	// Engine
	void AddSystem(iSystem* system);
	EntityManager* GetEntityManager();

	void SetRunning(bool isRunning);

	void* GetSystem(int id);

private:
	// Singleton
	Engine();
	~Engine();
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	bool m_Initialized = false;
	bool m_Running = false;

	// Systems
	std::vector<iSystem*> m_Systems;

	// Input
	Input m_Input;

	// Entity manager
	EntityManager* m_pEntityManager;

	// Time
	std::chrono::high_resolution_clock::time_point m_LastTime;
};

#define GetEngine() Engine::GetInstance()

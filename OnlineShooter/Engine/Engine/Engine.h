#pragma once

#include "Graphics/ShaderProgram.h"
#include "Graphics/Model.h"
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
	void Initialize(int argc, char** argv);
	void Destroy();

	// Glut main loop
	void Run();

	// Window
	void Resize(int w, int h);

	// Update
	void Update();

	// Render
	void Render();

	// Keyboard 
	void PressKey(unsigned char key);
	void ReleaseKey(unsigned char key);

	Input& GetInput();

	// Engine
	void AddSystem(iSystem* system);
	EntityManager* GetEntityManager();

	std::map<std::string, Model> models;

private:
	void LoadMeshes();
	void LoadShaders();
	void LoadCamera();
	void LoadEntities();

	// Singleton
	Engine();
	~Engine();
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	bool m_Initialized = false;

	// Systems
	std::vector<iSystem*> m_Systems;

	// Window information
	int m_WindowX = 0;
	int m_WindowY = 0;
	int m_WindowWidth = 0;
	int m_WindowHeight = 0;
	int m_WindowRatio = 0;

	// Shader stuff
	std::vector<ShaderProgram> m_ShaderPrograms;
	GLuint m_ViewMatrixUL;
	GLuint m_ModelMatrixUL;
	GLuint m_ProjectionMatrixUL;
	GLuint m_ColorUL;

	// Input
	Input m_Input;

	// Entity manager
	EntityManager* m_pEntityManager;

	// Camera
	Entity* m_CameraEntity;

	// Time
	std::chrono::high_resolution_clock::time_point m_LastTime;
};

#define GetEngine() Engine::GetInstance()

// Wrappers for freeglut callbacks
void PressKey_Callback(unsigned char key, int x, int y);

void ReleaseKey_Callback(unsigned char key, int x, int y);

void Idle_Callback();

void Reshape_Callback(int w, int h);

void Render_Callback();

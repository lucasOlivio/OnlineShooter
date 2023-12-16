#pragma once

#include "Graphics/ShaderProgram.h"
#include "Graphics/Model.h"
#include "System/System.h"
#include "System/Input.h"
#include "System/EntityManager.h"
#include <chrono>

class Engine
{
public:
	Engine();
	~Engine();

	// Lifecycle
	void Initialize();
	void Destroy();

	// Window
	void Resize(int w, int h);

	// Update
	void Update();

	// Render
	void Render();

	// Keyboard 
	void PressKey(unsigned char key);
	void ReleaseKey(unsigned char key);

	// Engine
	void AddSystem(iSystem* system);

private:
	void LoadAssets();

	bool m_Initialized = false;

	// Systems
	std::vector<iSystem*> m_Systems;

	// Window information
	int m_WindowX = 0;
	int m_WindowY = 0;
	int m_WindowWidth = 0;
	int m_WindowHeight = 0;
	int m_WindowRatio = 0;

	// Model stuff
	std::vector<Model> m_Models;

	// Shader stuff
	std::vector<ShaderProgram> m_ShaderPrograms;
	GLuint m_ViewMatrixUL;
	GLuint m_ModelMatrixUL;
	GLuint m_ProjectionMatrixUL;
	GLuint m_ColorUL;

	// Input
	Input m_Input;

	// Entity manager
	EntityManager m_EntityManager;

	// Camera
	Entity* m_CameraEntity;

	// Time
	std::chrono::high_resolution_clock::time_point m_LastTime;
};

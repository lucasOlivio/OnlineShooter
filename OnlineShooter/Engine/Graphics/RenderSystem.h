#pragma once

#include "System/System.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Model.h"

class RenderSystem : iSystem
{
public:
	// Singleton
	static RenderSystem& GetInstance();

	virtual bool Start(const std::vector<Entity*>& entities, int argc, char** argv);

	virtual void Execute(const std::vector<Entity*>& entities, float dt);

	virtual void End();

	void Resize(int w, int h);

	void Render();

	std::map<std::string, Model> models;
private:
	// Singleton
	RenderSystem();
	~RenderSystem() {};
	RenderSystem(const RenderSystem&) = delete;
	RenderSystem& operator=(const RenderSystem&) = delete;

	void LoadShaders();
	void LoadCamera();
	void LoadMeshes();

	// Window information
	int m_WindowX;
	int m_WindowY;
	int m_WindowWidth;
	int m_WindowHeight;
	int m_WindowRatio;

	// Shader stuff
	std::vector<ShaderProgram> m_ShaderPrograms;
	GLuint m_ViewMatrixUL;
	GLuint m_ModelMatrixUL;
	GLuint m_ProjectionMatrixUL;
	GLuint m_ColorUL;

	// Camera
	Entity* m_CameraEntity;
};

#define GetRenderSystem() RenderSystem::GetInstance()

// Wrappers for freeglut callbacks
void PressKey_Callback(unsigned char key, int x, int y);

void ReleaseKey_Callback(unsigned char key, int x, int y);

void Idle_Callback();

void Reshape_Callback(int w, int h);

void Render_Callback();
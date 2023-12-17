#pragma once

#include "System/System.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Model.h"

class RenderSystem : public iSystem
{
public:
	~RenderSystem() {};

	// Singleton
	static RenderSystem* GetInstance();

	void InitGlut(int argc, char** argv);

	virtual bool Start(const std::vector<Entity*>& entities, int argc, char** argv);

	virtual void Execute(const std::vector<Entity*>& entities, float dt);

	virtual void End();

	void Resize(int w, int h);

	void Render();

	void LoadShaders();
	void LoadCamera();
	void LoadMeshes();

	std::map<std::string, Model> models;
private:
	// Singleton
	RenderSystem() {};
	RenderSystem(const RenderSystem&) = delete;
	RenderSystem& operator=(const RenderSystem&) = delete;
	static RenderSystem* m_pInstance;

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

void Reshape_Callback(int w, int h);

void Render_Callback();

void Idle_Callback();
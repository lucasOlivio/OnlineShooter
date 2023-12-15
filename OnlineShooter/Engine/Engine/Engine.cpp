#include "Engine.h"
#include "System/Components/Transform.h"
#include "System/Components/MeshRenderer.h"

#include <sstream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

Engine::Engine()
{
}

Engine::~Engine()
{
}

void Engine::Initialize()
{
	LoadAssets();

	m_EntityManager = EntityManager();

	m_LastTime = std::chrono::high_resolution_clock::now();
}

void Engine::Destroy()
{
}

void Engine::Resize(int w, int h)
{
	if (w <= 0 || h <= 0) return;
	m_WindowWidth = w;
	m_WindowHeight = h;
	m_WindowRatio = static_cast<float>(w) / static_cast<float>(h);
	glViewport(0, 0, m_WindowWidth, m_WindowHeight);
}

void Engine::Update()
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> dt = currentTime - m_LastTime;
	m_LastTime = currentTime;

	// Deal with other engine stuff here or extend it from other to deal
	// with the other details needed from server/client?
}

void Engine::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	const glm::vec3 up(0.f, 1.f, 0.f);
	const glm::vec3 forward(0.f, 0.f, -1.f);

	glm::mat4 projectionMatrix = glm::perspective(
		glm::radians(45.0f),
		((GLfloat)m_WindowWidth) / ((GLfloat)m_WindowHeight),
		0.1f,
		10000.0f
	);
	glUniformMatrix4fv(m_ProjectionMatrixUL, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	CheckGLError();


	TransformComponent* cameraTransform = m_CameraEntity->GetComponent<TransformComponent>();
	glm::vec3 cameraPosition = cameraTransform->position;
	glm::vec3 cameraForward = cameraTransform->orientation * forward;

	glm::vec3 toOrigin = glm::normalize(-cameraPosition);
	glm::mat4 viewMatrix = glm::lookAt(cameraPosition, toOrigin, up);
	//glm::mat4 viewMatrix = glm::lookAt(glm::vec3(-10.f, 0.f, 0.f), glm::vec3(-9.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	glUniformMatrix4fv(m_ViewMatrixUL, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	CheckGLError();

	std::vector<Entity*> entities;
	m_EntityManager.GetEntities(entities);

	for (int i = 0; i < entities.size(); i++)
	{
		Entity* entity = entities[i];
		if (!entity->HasComponent<MeshRendererComponent>() || 
			!entity->HasComponent<TransformComponent>())
		{
			continue;
		}

		MeshRendererComponent* renderer = entity->GetComponent<MeshRendererComponent>();
		TransformComponent* transform = entity->GetComponent<TransformComponent>();

		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), transform->position);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.f), transform->scale);
		glm::mat4 rotationMatrix = glm::mat4_cast(transform->orientation);
		glm::mat4 modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

		glUniformMatrix4fv(m_ModelMatrixUL, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		CheckGLError();

		glUniform3fv(m_ColorUL, 1, glm::value_ptr(renderer->color));
		CheckGLError();

		glBindVertexArray(renderer->vbo);
		CheckGLError();

		glDrawElements(GL_TRIANGLES, renderer->numTriangles * 3, GL_UNSIGNED_INT, (GLvoid*)0);
		CheckGLError();
	}


	glutSwapBuffers();
}

void Engine::PressKey(unsigned char key)
{
	m_Input.PressKey(key);
}

void Engine::ReleaseKey(unsigned char key)
{
	m_Input.ReleaseKey(key);
}

void Engine::LoadAssets()
{
	// Load shader
	ShaderProgram simpleShader(
		"assets/shaders/SimpleShader.vertex.glsl",
		"assets/shaders/SimpleShader.fragment.glsl"
	);

	m_ShaderPrograms.push_back(simpleShader);
	CheckGLError();

	//
	// Shader uniforms locations
	glUseProgram(simpleShader.id);
	CheckGLError();
	m_ProjectionMatrixUL = glGetUniformLocation(simpleShader.id, "ProjectionMatrix");
	m_ViewMatrixUL = glGetUniformLocation(simpleShader.id, "ViewMatrix");
	m_ModelMatrixUL = glGetUniformLocation(simpleShader.id, "ModelMatrix");
	m_ColorUL = glGetUniformLocation(simpleShader.id, "Color");
	CheckGLError();

	//
	// Meshes
	Model cone("assets/models/cone.obj");
	Model cube("assets/models/cube.obj");
	Model cylinder("assets/models/cylinder.obj");
	Model sphere("assets/models/sphere.obj");
	m_Models.push_back(cone);
	m_Models.push_back(cube);
	m_Models.push_back(cylinder);
	m_Models.push_back(sphere);

	//
	// Maybe create all 4 players entities here already setting their ID?

	//
	// Entities
	const glm::vec3 origin(0.f);
	const glm::vec3 unscaled(1.f);
	const glm::quat identity(1.f, 0.f, 0.f, 0.f);

	//
	// Camera
	m_CameraEntity = m_EntityManager.CreateEntity();
	glm::quat rotation = identity * glm::vec3(0.f, -1.f, -0.1f);
	m_CameraEntity->AddComponent<TransformComponent>(glm::vec3(-20.f, 50.f, 0.f), unscaled, rotation);
}

void Engine::AddSystem(iSystem* system)
{
	m_Systems.push_back(system);
}

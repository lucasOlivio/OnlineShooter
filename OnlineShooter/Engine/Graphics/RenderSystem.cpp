#include "RenderSystem.h"
#include "System/Components/RigidBody.h"
#include "System/Components/Transform.h"
#include "System/Components/MeshRenderer.h"
#include "Engine/Engine.h"

#include <common.h>

#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

RenderSystem* RenderSystem::m_pInstance = nullptr;
bool doRender = true;

RenderSystem* RenderSystem::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new RenderSystem();
	}
	return m_pInstance;
}

bool RenderSystem::Start(const std::vector<Entity*>& entities, int argc, char** argv)
{
	if (!doRender) {
		return true;
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1200, 800);
	Resize(1200, 800);
	glutCreateWindow("UDP Multiplayer game!");

	glClearColor(0.3f, 0.3f, 0.4f, 1.0f);

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "GLEW initialization failed: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	glutIgnoreKeyRepeat(1);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Callbacks setup
	glutKeyboardFunc(PressKey_Callback);
	glutKeyboardUpFunc(ReleaseKey_Callback);

	glutReshapeFunc(Reshape_Callback);
	glutDisplayFunc(Render_Callback);
	//glutIdleFunc(Render_Callback);

	glDisable(GL_CULL_FACE);

	LoadShaders();
	LoadCamera();
	LoadMeshes();


   // return false;
	doRender = false;
    return true;
}

void RenderSystem::Execute(const std::vector<Entity*>& entities, float dt)
{
	//Render();
	Render();
}

void RenderSystem::End()
{
	delete m_CameraEntity;
}

void RenderSystem::Resize(int w, int h)
{
	if (w <= 0 || h <= 0) return;
	m_WindowWidth = w;
	m_WindowHeight = h;
	m_WindowRatio = static_cast<float>(w) / static_cast<float>(h);
	glViewport(0, 0, m_WindowWidth, m_WindowHeight);
}

void RenderSystem::Render()
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
	GetEngine().GetEntityManager()->GetEntities(entities);

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

void RenderSystem::LoadShaders()
{
	// Load shader
	ShaderProgram simpleShader(
		(ASSETS_PATH + "shaders/SimpleShader.vertex.glsl").c_str(),
		(ASSETS_PATH + "shaders/SimpleShader.fragment.glsl").c_str()
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
}

void RenderSystem::LoadCamera()
{
	const glm::vec3 origin(0.f);
	const glm::vec3 unscaled(1.f);
	const glm::quat identity(1.f, 0.f, 0.f, 0.f);

	//
	// Camera
	m_CameraEntity = GetEngine().GetEntityManager()->CreateEntity();  //hehe?
	glm::quat rotation = identity * glm::vec3(0.f, -1.f, -0.1f);
	m_CameraEntity->AddComponent<TransformComponent>(glm::vec3(-20.f, 50.f, 0.f), unscaled, rotation);
}

void RenderSystem::LoadMeshes()
{
	//
	// Meshes
	Model cone((ASSETS_PATH + "models/cone.obj").c_str());
	Model cube((ASSETS_PATH + "models/cube.obj").c_str());
	Model cylinder((ASSETS_PATH + "models/cylinder.obj").c_str());
	Model sphere((ASSETS_PATH + "models/sphere.obj").c_str());

	models["cone"] = cone;
	models["cube"] = cube;
	models["cylinder"] = cylinder;
	models["sphere"] = sphere;
}

// Wrappers for freeglut callbacks

void PressKey_Callback(unsigned char key, int x, int y)
{
	std::cout << "keypressed" << std::endl;
	GetEngine().PressKey(key);
}

void ReleaseKey_Callback(unsigned char key, int x, int y)
{
	GetEngine().ReleaseKey(key);
}

void Reshape_Callback(int w, int h)
{
	GetRenderSystem()->Resize(w, h);
}
void Render_Callback()
{
	GetRenderSystem()->Render();
}


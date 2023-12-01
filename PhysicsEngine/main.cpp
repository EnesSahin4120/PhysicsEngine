#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

#include "Cube.h"
#include "Sphere.h"
#include "Rigidbody.h"
#include "SimulationController.h"

#include "Skybox.h"

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
uniformSpecularIntensity = 0, uniformShininess = 0,
uniformDirectionalLightTransform = 0, uniformOmniLightPos = 0, uniformFarPlane = 0;

Window mainWindow;

std::vector<Shader> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;

Camera camera;

Texture cubeTexture;
Texture ballTexture;
Texture groundTexture;

std::vector<Sphere> spheres;
std::vector<Cube> cubes;
std::vector<Rigidbody> rigidBodies;

Material gameMaterial;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Skybox skybox;

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
bool isStart = false;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateObjects()
{
	rigidBodies.push_back(Rigidbody(glm::vec3(-2.0f, 2.0f, 0), 0.0f * toRadians, 1.0f, 0.5f, false, 0.0f, 0.0f, 0.4f, Rigidbody::Shape::Sphere));
	rigidBodies.push_back(Rigidbody(glm::vec3(2.5f, 5.0f, 0.0f), 90.0f * toRadians, 1.0f, 0.5f, false, 0.0f, 0.0f, 0.6f, Rigidbody::Shape::Sphere));
	rigidBodies.push_back(Rigidbody(glm::vec3(-2.0f, 4.0f, 0.0f), 30.0f * toRadians, 1.0f, 0.5f, false, 1.0f, 1.5f, 0.0f, Rigidbody::Shape::Cube));
	rigidBodies.push_back(Rigidbody(glm::vec3(0.0f, -2.0f, 0.0f), 10.0f * toRadians, 1.0f, 0.5f, true, 15.0f, 1.5f, 0.0f, Rigidbody::Shape::Cube));
	rigidBodies.push_back(Rigidbody(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f * toRadians, 1.0f, 0.5f, false, 0.0f, 0.0f, 0.8f, Rigidbody::Shape::Sphere));
	rigidBodies.push_back(Rigidbody(glm::vec3(-0.5f, 4.0f, 0.0f), 0.0f * toRadians, 1.0f, 0.5f, false, 1.0f, 1.0f, 0.0f, Rigidbody::Shape::Cube));

	spheres.push_back(Sphere(40.0f, 40.0f, rigidBodies[0].radius, rigidBodies[0].center, rigidBodies[0].rotZ, 0));
	spheres.push_back(Sphere(40.0f, 40.0f, rigidBodies[1].radius, rigidBodies[1].center, rigidBodies[1].rotZ, 1));
	cubes.push_back(Cube(rigidBodies[2].width, rigidBodies[2].height, 1.0f, rigidBodies[2].rotZ, rigidBodies[2].center, 2));
	cubes.push_back(Cube(rigidBodies[3].width, rigidBodies[3].height, 1.0f, rigidBodies[3].rotZ, rigidBodies[3].center, 3));
	spheres.push_back(Sphere(40.0f, 40.0f, rigidBodies[4].radius, rigidBodies[4].center, rigidBodies[4].rotZ, 4));
	cubes.push_back(Cube(rigidBodies[5].width, rigidBodies[5].height, 1.0f, rigidBodies[5].rotZ, rigidBodies[5].center, 5));
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
	omniShadowShader.CreateFromFiles("Shaders/omni_shadow_map.vert", "Shaders/omni_shadow_map.geom", "Shaders/omni_shadow_map.frag");
}

void RenderScene()
{
	glm::mat4 model(1.0f);
	for (int i = 0; i < cubes.size(); i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubes[i].pos);
		model = glm::rotate(model, cubes[i].angle, glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(cubes[i].width, cubes[i].height, cubes[i].depth));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		if (!rigidBodies[cubes[i].rigidBodyIndex].isStatic)
			cubeTexture.UseTexture();
		else
			groundTexture.UseTexture();

		gameMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		cubes[i].Update(model);
		cubes[i].RenderMesh();
	}

	for (int i = 0; i < spheres.size(); i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, spheres[i].pos);
		model = glm::rotate(model, spheres[i].angle, glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ballTexture.UseTexture();
		gameMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		spheres[i].mesh->RenderMesh();
	}
}

void GameStart(bool* keys) {
	if (keys[GLFW_KEY_O])
	{
		isStart = true;
	}
};


void DirectionalShadowMapPass(DirectionalLight* light)
{
	directionalShadowShader.UseShader();

	glViewport(0, 0, light->getShadowMap()->GetShadowWidth(), light->getShadowMap()->GetShadowHeight());

	light->getShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();
	directionalShadowShader.SetDirectionalLightTransform(&light->CalculateLightTransform());

	directionalShadowShader.Validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight* light)
{
	omniShadowShader.UseShader();

	glViewport(0, 0, light->getShadowMap()->GetShadowWidth(), light->getShadowMap()->GetShadowHeight());

	light->getShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = omniShadowShader.GetModelLocation();
	uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
	uniformFarPlane = omniShadowShader.GetFarPlaneLocation();

	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformFarPlane, light->GetFarPlane());
	omniShadowShader.SetLightMatrices(light->CalculateLightTransform());

	omniShadowShader.Validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	glViewport(0, 0, 1366, 768);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox.DrawSkybox(viewMatrix, projectionMatrix);

	shaderList[0].UseShader();

	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformModel = shaderList[0].GetModelLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformShininess = shaderList[0].GetShininessLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	shaderList[0].SetDirectionalLight(&mainLight);
	shaderList[0].SetPointLights(pointLights, pointLightCount, 3, 0);
	shaderList[0].SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
	shaderList[0].SetDirectionalLightTransform(&mainLight.CalculateLightTransform());

	mainLight.getShadowMap()->Read(GL_TEXTURE2);
	shaderList[0].SetTexture(1);
	shaderList[0].SetDirectionalShadowMap(2);

	glm::vec3 lowerLight = camera.getCameraPosition();
	lowerLight.y -= 0.3f;
	spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

	shaderList[0].Validate();

	RenderScene();
}

int main()
{
	mainWindow = Window(1366, 768); 
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 2.0f, 9.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

	cubeTexture = Texture("Textures/cube.png");
	cubeTexture.LoadTextureA();
	ballTexture = Texture("Textures/ball.jpg");
	ballTexture.LoadTexture();
	groundTexture = Texture("Textures/ground.bmp");
	groundTexture.LoadTextureA();

	gameMaterial = Material(0.3f, 4);

	mainLight = DirectionalLight(2048, 2048,
		1.0f, 0.53f, 0.3f,
		0.1f, 0.9f,
		0.0f, -1.0f, 0.0f);


	spotLights[0] = SpotLight(1024, 1024,
		0.01f, 100.0f,
		1.0f, 1.0f, 1.0f,
		0.4f, 0.4f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		100.0f);
	spotLightCount++;

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/Left_Tex.png");
	skyboxFaces.push_back("Textures/Skybox/Right_Tex.png");
	skyboxFaces.push_back("Textures/Skybox/Up_Tex.png");
	skyboxFaces.push_back("Textures/Skybox/Down_Tex.png");
	skyboxFaces.push_back("Textures/Skybox/Front_Tex.png");
	skyboxFaces.push_back("Textures/Skybox/Back_Tex.png");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); 
		deltaTime = now - lastTime; 
		lastTime = now;

		glfwPollEvents();

		if (isStart) {
			SimulationController::SphereCollisions(spheres, rigidBodies);
			SimulationController::CubeCollisions(cubes, rigidBodies);
			SimulationController::SphereCubeCollisions(spheres, cubes, rigidBodies);


			for (int i = 0; i < rigidBodies.size(); i++) {
				rigidBodies[i].Update(deltaTime);
			}
			for (int i = 0; i < spheres.size(); i++) {
				spheres[i].pos = rigidBodies[spheres[i].rigidBodyIndex].center;
				spheres[i].angle = rigidBodies[spheres[i].rigidBodyIndex].rotZ;
			}
			for (int i = 0; i < cubes.size(); i++) {
				cubes[i].pos = rigidBodies[cubes[i].rigidBodyIndex].center;
				cubes[i].angle = rigidBodies[cubes[i].rigidBodyIndex].rotZ;
			}
		}

		GameStart(mainWindow.getsKeys());
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		DirectionalShadowMapPass(&mainLight);
		for (size_t i = 0; i < pointLightCount; i++)
		{
			OmniShadowMapPass(&pointLights[i]);
		}
		for (size_t i = 0; i < spotLightCount; i++)
		{
			OmniShadowMapPass(&spotLights[i]);
		}
		RenderPass(camera.calculateViewMatrix(), projection);

		mainWindow.swapBuffers();
	}

	return 0;
}
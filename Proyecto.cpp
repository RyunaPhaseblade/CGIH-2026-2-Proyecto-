/*
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada
Sesión 2
Compleja: Por medio de funciones y algoritmos.
Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "CameraTP.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
/*bool aelGira = false;*/
// 90 segundos para un ciclo completo
const float duracionCiclo = 90.0f;


//Camaras

//para que giren los engranajes
float rotEngranaje;
float rotEngranajeOffset;
float rotEngranaje2;
float rotEngranaje2Offset;
float rotEngranaje3;
float rotEngranaje3Offset;
float rotEngranaje4;
float rotEngranaje4Offset;
float rotEngranaje5;
float rotEngranaje5Offset;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
CameraTP cameraTP;


//Testuras
/*Texture brickTexture;*/
Texture pisoTexture;

//Modelos
Model Fortaleza_M;
Model EngranajeM_M;//Engranajes de la muralla
Model EngranajeD_M;//Engranaje dorados
Model EngranajeC_M;//Engranaje craneo
Model EngranajeCono1_M;//Engranaje cono inferior
Model EngranajeCono2_M;//Engranaje cono superior
Model EngranajeF1_M;//Engranaje frotando derecho
Model EngranajeF2_M;//Engranaje frotando izquierdo

//modelos de zero
Model ZeroCuerpo_M;
Model ZeroBrazoDer_M;
Model ZeroBrazoIzq_M;
Model ZeroAntebrazoIzq_M;

/*Model Kitt_M;*/


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};



	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();

	//fortaleza
	Fortaleza_M = Model();
	Fortaleza_M.LoadModel("Models/Fortaleza_Final.obj");
	//engranajes de la muralla
	EngranajeM_M = Model();
	EngranajeM_M.LoadModel("Models/engranaje_muralla.obj");
	EngranajeD_M = Model();
	EngranajeD_M.LoadModel("Models/engranaje_dorado.obj");
	EngranajeC_M = Model();
	EngranajeC_M.LoadModel("Models/engranaje_craneo.obj");
	EngranajeCono1_M = Model();
	EngranajeCono1_M.LoadModel("Models/engranaje_cono1.obj");
	EngranajeCono2_M = Model();
	EngranajeCono2_M.LoadModel("Models/engranaje_cono2.obj");
	EngranajeF1_M = Model();
	EngranajeF1_M.LoadModel("Models/engranaje_frotando1.obj");
	EngranajeF2_M = Model();
	EngranajeF2_M.LoadModel("Models/engranaje_frotando2.obj");

	//Zero
	ZeroCuerpo_M = Model();
	ZeroCuerpo_M.LoadModel("Models/zero_cuerpo.obj");
	ZeroBrazoDer_M = Model();
	ZeroBrazoDer_M.LoadModel("Models/zero_brazoDer.obj");
	ZeroBrazoIzq_M = Model();
	ZeroBrazoIzq_M.LoadModel("Models/zero_brazoIzq.obj");
	ZeroAntebrazoIzq_M = Model();
	ZeroAntebrazoIzq_M.LoadModel("Models/zero_antebrazoIzq.obj");

	
	/*
	Model relojM;
	*/
	/*
	reloj = Model();
	reloj.LoadModel("Models/test.obj");
	*/
	

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	/*
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	*/
	unsigned int spotLightCount = 0;
	/*
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	*/


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::vec3 lowerLight(0.0f, 0.0f, 0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelauxD(1.0);
	glm::mat4 modelauxF(1.0);
	glm::mat4 modelauxE(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);


	//inicializacion de las variables que van hacer girar a los engranajes
	rotEngranaje = 0;
	rotEngranajeOffset = 2.0f;
	rotEngranaje2 = 0;
	rotEngranaje2Offset = 0.5f;
	rotEngranaje3 = 0;
	rotEngranaje3Offset = 0.25f;
	rotEngranaje4 = 0;
	rotEngranaje4Offset = 0.2f;


	// Posiciones 
	glm::vec3 posModelo = glm::vec3(0.0f, 0.0f, 0.0f);

	bool engineCameraTP = false;
	Camera cameraFP;
	cameraFP = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);
	CameraTP cameraTP(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 20.0f, 10.0f, 0.5f);
	
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		/*
		if (mainWindow.getCam()) {
			cameraTP.keyControl(mainWindow.getsKeys(), deltaTime);
			cameraTP.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		} else {
			cameraFP.keyControl(mainWindow.getsKeys(), deltaTime);
			cameraFP.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		}*/
		

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		// --- LÓGICA DE CICLO DÍA/NOCHE ---
		float tiempoActual = glfwGetTime();
		// Progreso de 0.0 a 1.0 cada 90 segundos
		float progreso = fmod(tiempoActual, duracionCiclo) / duracionCiclo;

		// Convertir progreso a radianes (360 grados)
		float anguloSol = progreso * 2.0f * 3.14159265f;

		// El sol se mueve en el plano XY (atardece en X, sube en Y)
		float dirX = sin(anguloSol);
		float dirY = cos(anguloSol); // Positivo = Día, Negativo = Noche
		float dirZ = -1.0f;          // Mantener ligera inclinación para sombras interesantes

		// Variables para los nuevos estados de la luz
		float ambientIntensity, diffuseIntensity;
		glm::vec3 colorSol;

		if (dirY > 0) {
			// ESTADO: DÍA (Sol sobre el horizonte)
			// El color varía de naranja (amanecer) a blanco (cenit)
			float factorAmanecer = std::min(1.0f, dirY * 5.0f); // Transición rápida al salir el sol
			colorSol = glm::mix(glm::vec3(1.0f, 0.4f, 0.1f), glm::vec3(1.0f, 1.0f, 1.0f), factorAmanecer);

			ambientIntensity = 0.2f + (dirY * 0.3f); // Más luz ambiente al mediodía
			diffuseIntensity = dirY * 0.8f;          // Luz directa fuerte
		}
		else {
			// ESTADO: NOCHE (Sol bajo el horizonte)
			colorSol = glm::vec3(0.1f, 0.1f, 0.2f);  // Tono azulado nocturno
			ambientIntensity = 0.05f;                // Luz mínima para ver la Fortaleza
			diffuseIntensity = 0.0f;                 // No hay luz directa del sol
		}

		// Como no tienes SetDirectionalLight, creamos un objeto nuevo en cada iteración 
		// o actualizamos el existente si tu clase lo permite. 
		// La forma más segura con tu código actual:
		mainLight = DirectionalLight(colorSol.r, colorSol.g, colorSol.b,
			ambientIntensity, diffuseIntensity,
			dirX, dirY, dirZ);

		// --- FIN LÓGICA DÍA/NOCHE ---

		// Luego sigues con el envío al shader que ya tenías:
		shaderList[0].SetDirectionalLight(&mainLight);

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		//reiniciar variables antes de que sean enviadas al shader

		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);

		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, posModelo);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//FORTALEZA DEL DR. WILLY
		//el esqueleto: muralla, torres, torres con cañon, calavera, mansion, cono superior
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(170.0f, -2.0f, 75.0f));
		modelauxF = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Fortaleza_M.RenderModel();//*/

		//ENGRANAJES MURALLA
		//engranajes de la muralla, frente izq
		model = modelauxF;
		model = glm::translate(model, glm::vec3(-2.75f, 1.5f, -1.0f));
		modelauxE = model;
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//engranajes de la muralla, frente der
		model = modelauxE;
		model = glm::translate(model, glm::vec3(5.5f, 0.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//LADO DERECHO 1
		//engranajes de la muralla, der1 izq
		model = modelauxF;
		model = glm::translate(model, glm::vec3(16.05f, 1.5f, -1.42f));
		model = glm::rotate(model, 19 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//engranajes de la muralla, der1 der
		model = modelauxF;
		model = glm::translate(model, glm::vec3(21.15f, 1.5f, -3.0f));
		model = glm::rotate(model, 19 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//engranajes de la muralla, der2 izq
		model = modelauxF;
		model = glm::translate(model, glm::vec3(32.25f, 1.5f, -11.5f));
		model = glm::rotate(model, 44 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//engranajes de la muralla, der2 der
		model = modelauxF;
		model = glm::translate(model, glm::vec3(36.90f, 1.5f, -15.4f));
		model = glm::rotate(model, 44 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//engranajes de la muralla, der izq
		model = modelauxF;
		model = glm::translate(model, glm::vec3(42.25f, 1.5f, -28.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//engranajes de la muralla, der der
		model = modelauxF;
		model = glm::translate(model, glm::vec3(42.25f, 1.5f, -34.25f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//LADO IZQUIERDO 1
		//engranajes de la muralla, izq1 der
		model = modelauxF;
		model = glm::translate(model, glm::vec3(-16.0f, 1.5f, -1.7f));
		model = glm::rotate(model, -19 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//engranajes de la muralla, izq1 izq
		model = modelauxF;
		model = glm::translate(model, glm::vec3(-21.5f, 1.5f, -3.5f));
		model = glm::rotate(model, -19 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//engranajes de la muralla, izq2 der
		model = modelauxF;
		model = glm::translate(model, glm::vec3(-30.25f, 1.5f, -12.0f));
		model = glm::rotate(model, -44 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//engranajes de la muralla, izq2 izq
		model = modelauxF;
		model = glm::translate(model, glm::vec3(-34.9f, 1.5f, -15.9f));
		model = glm::rotate(model, -44 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//engranajes de la muralla, izq der
		model = modelauxF;
		model = glm::translate(model, glm::vec3(-39.5f, 1.5f, -27.8f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//engranajes de la muralla, izq izq
		model = modelauxF;
		model = glm::translate(model, glm::vec3(-39.5f, 1.5f, -34.05f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//LADO DERECHO 2
		//engranajes de la muralla, der3 izq
		model = modelauxF;
		model = glm::translate(model, glm::vec3(36.5f, 1.5f, -44.7f));
		model = glm::rotate(model, -44 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//engranajes de la muralla, der3 der
		model = modelauxF;
		model = glm::translate(model, glm::vec3(31.85f, 1.5f, -48.6f));
		model = glm::rotate(model, -44 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//engranajes de la muralla, der4 izq
		model = modelauxF;
		model = glm::translate(model, glm::vec3(23.1f, 1.5f, -57.1));
		model = glm::rotate(model, -19 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//engranajes de la muralla, der4 der
		model = modelauxF;
		model = glm::translate(model, glm::vec3(17.6f, 1.5f, -58.9f));
		model = glm::rotate(model, -19 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//engranajes de la muralla, trasera izq
		model = modelauxF;
		model = glm::translate(model, glm::vec3(4.7f, 1.5f, -61.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//engranajes de la muralla, trasera der
		model = modelauxF;
		model = glm::translate(model, glm::vec3(-1.0f, 1.5f, -61.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//engranajes de la muralla, der5 izq
		model = modelauxF;
		model = glm::translate(model, glm::vec3(-14.3f, 1.5f, -60.58f));
		model = glm::rotate(model, 19 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//engranajes de la muralla, der5 der
		model = modelauxF;
		model = glm::translate(model, glm::vec3(-19.4f, 1.5f, -59.0f));
		model = glm::rotate(model, 19 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//engranajes de la muralla, der6 izq
		model = modelauxF;
		model = glm::translate(model, glm::vec3(-30.5f, 1.5f, -50.5f));
		model = glm::rotate(model, 44 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//engranajes de la muralla, der6 der
		model = modelauxF;
		model = glm::translate(model, glm::vec3(-35.15f, 1.5f, -46.6f));
		model = glm::rotate(model, 44 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeM_M.RenderModel();//*/

		//ENGRANAJES DORADOS
		//engranajes dorado, debajo de la calavera 1
		model = modelauxF;
		model = glm::translate(model, glm::vec3(-4.5f, 5.0f, -15.0f));
		model = glm::rotate(model, rotEngranaje2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelauxD = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeD_M.RenderModel();//*/

		//engranajes dorado, debajo de la calavera 2
		model = modelauxD;
		model = glm::translate(model, glm::vec3(3.5f, -1.5f, -1.5f));
		//model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeD_M.RenderModel();//*/

		//engranajes dorado, debajo de la calavera 3
		model = modelauxD;
		model = glm::translate(model, glm::vec3(7.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeD_M.RenderModel();//*/

		//engranajes dorado, debajo de la calavera 4
		model = modelauxD;
		model = glm::translate(model, glm::vec3(10.5f, -1.5f, -1.5f));
		//model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeD_M.RenderModel();//*/

		//engranajes dorado, debajo de la calavera 5
		model = modelauxD;
		model = glm::translate(model, glm::vec3(14.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeD_M.RenderModel();//*/

		//engranajes dorado, frotando
		model = modelauxF;
		model = glm::translate(model, glm::vec3(28.211f, 30.0f, -40.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje3 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeD_M.RenderModel();//*/

		//ENGRANAJE CRANEO
		model = modelauxF;
		model = glm::translate(model, glm::vec3(1.5f, 20.0f, -30.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeC_M.RenderModel();//*/

		//ENGRANAJE CONO
		//engranaje inferior
		model = modelauxF;
		model = glm::translate(model, glm::vec3(2.25f, 36.0f, -44.5f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeCono1_M.RenderModel();//*/

		//engranaje superior
		model = modelauxF;
		model = glm::translate(model, glm::vec3(2.3f, 39.667f, -44.5f));
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeCono2_M.RenderModel();//*/

		//ENGRANAJES FROTANTES
		//engranaje frotante derecho
		model = modelauxF;
		model = glm::translate(model, glm::vec3(22.5f, 15.0f, -25.0f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje4 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeF1_M.RenderModel();//*/

		//engranaje frotante izq
		model = modelauxF;
		model = glm::translate(model, glm::vec3(-20.0f, 20.0f, -43.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 25 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -rotEngranaje4 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EngranajeF2_M.RenderModel();//*/

	

		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
		
	}
	return 0;
	
}

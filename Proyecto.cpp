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
#include "CameraA.h"
#include "CameraF.h"
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
const float duracionCiclo = 60.0f;


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

//variables para la animacion de zero
float timerEspera = 0.0f;//timer para la animacion del hombro der
bool inicio = 0;//bandera que va iniciar el timer y el inicio de la animacion del hombro derecho

float timerHI = 0.0f;//timer para la animacion del hombro izq
bool siguiente = 0;//bandera que va iniciar la animacion del hombro izq

float timerD = 0.0f;//timer para la animacion de las esferas de energia disparadas
float timerD2 = 0.0f;
float timerD3 = 0.0f;
//variables para que inicie la animacion del disparo de energia
bool disparo1 = 0;
bool disparo2 = 0;
bool disparo3 = 0;
bool nuevoDisparo = 0;

//variables para las rotaciones de zero y sus incrementos
float rotHombroD;//para el hombro derecho
float rotHombroDOffset;
float rotHombroI;//para el hombro izq
float rotHombroIOffset;
float rotCodoI;//para el codo
float rotCodoIOffset;
float movEsferaE;//para mover las esferas de energia
float movEsferaEOffset;
float movEsferaE2;//para mover las esferas de energia
float movEsferaE2Offset;
float movEsferaE3;//para mover las esferas de energia
float movEsferaE3Offset;

//variables para la animacion de mega man
float timerM = 0.0f;
bool mega = 0;
bool mega1 = 0;
bool cambio = 0;
bool mega2 = 0;
bool cambioI = 0;

//variables para las rotaciones de mega y sus incrementos
float rotHombroDM;//para el hombro derecho
float rotHombroDMOffset;
float rotHombroDM1;//para el hombro derecho
float rotHombroDM1Offset;
float rotHombroDM2;//para el hombro derecho
float rotHombroDM2Offset;
float rotHombroDM3;//para el hombro derecho
float rotHombroDM3Offset;

float rotHombroIM;//para el hombro izq
float rotHombroIMOffset;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

//Variables para Cacodemonio
glm::vec3 cacoPos = glm::vec3(0.0f, 5.0f, 0.0f);   
glm::vec3 cacoTarget = glm::vec3(5.0f, 5.0f, 5.0f); 
float cacoVel = 0.05f;         
float cacoAngle = 0.0f;        
float timerCacoY = 0.0f;       
float intervaloCacoY = 5.0f;
float cacoPitch = 0.0f;        
float targetPitch = 0.0f;      

Camera camera;
CameraTP cameraTP;


//Testuras
/*Texture brickTexture;*/
Texture pisoTexture;
Texture testTexture;

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
Model ZeroHombroDer_M;
Model ZeroBrazoDer_M;
Model ZeroHombroIzq_M;
Model ZeroBrazoIzq_M;
Model ZeroCodoIzq_M;
Model ZeroManoIzq_M;
Model ZeroEsferaE_M;

//Modelos de Megaman
Model MegaManCuerpo_M;
Model MegaManBrazoDer_M;
Model MegaManBrazoIzq_M;

//Doom
Model GoreNest;
Model GoldNest;
Model Cacodemon;
Model MarauderB;
Model MarauderR;
Model MarauderL;
Model Esfera;

//Modelos Casa del arbol
Model CasaArbol_M;

//Modelos Molino
Model Molino_M;

//Modelos luces
Model Farola;

//Model relojM
Model relojM;

//Modelo bmo
Model BMO_M;

//Model jake el perro
Model jake_M;
//Model vias
Model vias_M;
//modelo tren
Model tren_M;

//Modelos extras
Model arboles1;
Model arboles2;
Model arbolesL;
Model Silla;
Model Bote;

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



	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
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
	Shader* shader1 = new Shader();
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
	testTexture = Texture("Textures/blanco.png");
	testTexture.LoadTextureA();

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

	//Zero5
	ZeroCuerpo_M = Model();
	ZeroCuerpo_M.LoadModel("Models/zero_cuerpo.obj");
	ZeroHombroDer_M = Model();
	ZeroHombroDer_M.LoadModel("Models/zero_hombroDer.obj");
	ZeroBrazoDer_M = Model();
	ZeroBrazoDer_M.LoadModel("Models/zero_brazoDer.obj");
	ZeroHombroIzq_M = Model();
	ZeroHombroIzq_M.LoadModel("Models/zero_hombroIzq.obj");
	ZeroBrazoIzq_M = Model();
	ZeroBrazoIzq_M.LoadModel("Models/zero_brazoIzq.obj");
	ZeroCodoIzq_M = Model();
	ZeroCodoIzq_M.LoadModel("Models/zero_codoIzq.obj");
	ZeroManoIzq_M = Model();
	ZeroManoIzq_M.LoadModel("Models/zero_manoIzq.obj");
	ZeroEsferaE_M = Model();
	ZeroEsferaE_M.LoadModel("Models/zero_esferaE.obj");

	CasaArbol_M = Model();
	CasaArbol_M.LoadModel("Models/CASARBOL.obj");

	//
	Molino_M = Model();
	Molino_M.LoadModel("Models/Molino.obj");

	//REloj central

	relojM = Model();
	relojM.LoadModel("Models/reloj.obj");

	//BMO 

	BMO_M = Model();
	BMO_M.LoadModel("Models/bmo.obj");

	//jake el perro
	jake_M = Model();
	jake_M.LoadModel("Models/jake_M.obj");

	//vias
	vias_M = Model();
	vias_M.LoadModel("Models/vias.obj");

	//tren
	tren_M = Model();
	tren_M.LoadModel("Models/tren.obj");

	//Mega Man
	MegaManCuerpo_M = Model();
	MegaManCuerpo_M.LoadModel("Models/megaman_cuerpo.obj");
	MegaManBrazoDer_M = Model();
	MegaManBrazoDer_M.LoadModel("Models/megaman_brazoDer.obj");
	MegaManBrazoIzq_M = Model();
	MegaManBrazoIzq_M.LoadModel("Models/megaman_brazoIzq.obj");


	//Doom Modelos
	GoreNest = Model();
	GoreNest.LoadModel("Models/gorenest.obj");
	GoldNest = Model();
	GoldNest.LoadModel("Models/golden_Nest.obj");
	Cacodemon = Model();
	Cacodemon.LoadModel("Models/cacodemon.obj");
	MarauderB = Model();
	MarauderB.LoadModel("Models/marauder_b.obj");
	MarauderR = Model();
	MarauderR.LoadModel("Models/marauder_ra.obj");
	MarauderL = Model();
	MarauderL.LoadModel("Models/marauder_la.obj");
	Esfera = Model();
	Esfera.LoadModel("Models/esfera.obj");

	//Extras
	Farola = Model();
	Farola.LoadModel("Models/farola.obj");
	arboles1 = Model();
	arboles1.LoadModel("Models/arboles_grupo1.obj");
	arboles2 = Model();
	arboles2.LoadModel("Models/arboles_grupo2.obj");
	arbolesL = Model();
	arbolesL.LoadModel("Models/arboles_hilera.obj");
	Silla = Model();
	Silla.LoadModel("Models/silla_parque.obj");
	Bote = Model();
	Bote.LoadModel("Models/bote_basura.obj");

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

	//GoldNest
	pointLights[0] = PointLight(
		1.0f, 0.2f, 0.0f,       
		10.0f, 15.0f,          
		-100.0f, 23.0f, 35.0f,     
		1.0f, 0.1f, 0.16f
	);

	//Farola 1
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		-20.0f, 4.5f, 43.8f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[2] = PointLight(1.0f, 0.0f, 0.0f,
		1.0f, 1.0f,
		-40.0f, 6.0f, 55.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[3] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		-60.0f, 4.5f, 43.8f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[4] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		-40.0f, 4.5f, 16.2f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[5] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		-80.0f, 4.5f, 16.2f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;


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
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::vec3 lowerLight(0.0f, 0.0f, 0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	//modelos auxiliares de la fortaleza
	glm::mat4 modelauxD(1.0);
	glm::mat4 modelauxE(1.0);
	glm::mat4 modelauxF(1.0);
	//modelos auxiliares de zero
	glm::mat4 modelauxZ(1.0);
	glm::mat4 modelauxHDerZ(1.0);
	glm::mat4 modelauxHIzqZ(1.0);

	//modelos auxiliares de mega man
	glm::mat4 modelauxM(1.0);

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


	//inicializacion de las variables que van hacer girar el hombro derecho de zero
	rotHombroD = 0.0f;
	rotHombroDOffset = 2.5;

	//inicializacion de las variables que van hacer girar el hombro izq de zero
	rotHombroI = 0.0f;
	rotHombroIOffset = 2.5;

	//inicializacion de las variables que van hacer girar el codo izq de zero
	rotCodoI = 0.0f;
	rotCodoIOffset = 2.5;

	//inicializacion de las variables que avancen las esferas de energia
	movEsferaE = 0.0f;
	movEsferaEOffset = 0.4;
	movEsferaE2 = 0.0f;
	movEsferaE2Offset = 0.4;
	movEsferaE3 = 0.0f;
	movEsferaE3Offset = 0.4;


	//inicializacion de las variables que van hacer girar el hombro derecho de mega
	rotHombroDM = 0.0f;
	rotHombroDMOffset = 2.5;

	rotHombroDM1 = 0.0f;
	rotHombroDM1Offset = 0.27;

	rotHombroDM2 = 0.0f;
	rotHombroDM2Offset = 1.25;

	rotHombroDM3 = 0.0f;
	rotHombroDM3Offset = 1.25;

	//inicializacion de las variables que van hacer girar el hombro izquierdo de mega
	rotHombroIM = 0.0f;
	rotHombroIMOffset = 0.15;


	//#####
	//Dia/noche variables
	float duracionCiclo = 90.0f; // Tiempo del ciclo
	float dirZ = 0.0f;

	// Colores 
	glm::vec3 MedioDia = glm::vec3(1.0f, 1.0f, 0.95f);
	glm::vec3 Atardecer = glm::vec3(1.0f, 0.4f, 0.15f);
	glm::vec3 Noche = glm::vec3(0.15f, 0.18f, 0.25f);

	// Variables de estado de la luz
	float ambientIntensity = 0.15f;
	float diffuseIntensity = 0.10;
	glm::vec3 colorSol = glm::vec3(0.0f);

	//####


	// Posiciones 
	glm::vec3 PersonajeMain = glm::vec3(0.0f, 0.0f, 0.0f);
	float rotacionModelo = 0.0f;
	glm::vec3 posModelo = glm::vec3(0.0f, 0.0f, 0.0f);	

	int tipoCamara = 0; // 0: Libre, 1: TP, 2: Aérea
	bool f5Presionada = false;
	bool TPV_C = false;
	Camera cameraFP;
	cameraFP = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);


	CameraTP cameraTP(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 20.0f, 10.0f, 0.5f);
	// Ligamos la cámara a la dirección de memoria de PersonajeMain
	cameraTP.establecerObjetivo(&PersonajeMain, 15.0f, 5.0f);

	CameraA cameraA;
	cameraA = CameraA(glm::vec3(0.0f, 75.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);


	CameraF cameraF = CameraF(glm::vec3(10.0f, 20.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	CameraF cameraF2 = CameraF(glm::vec3(-10.0f, 20.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


	//######

	//#####
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		glfwPollEvents();
		glm::mat4 vistaActual;
		glm::vec3 posOjo, dirOjo;

		// Camaras
		tipoCamara = mainWindow.getCam();

		if (tipoCamara == 1) { // TERCERA PERSONA
			cameraTP.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

			glm::vec3 camFront = cameraTP.getCameraDirection();
			glm::vec3 walkForward = glm::normalize(glm::vec3(camFront.x, 0.0f, camFront.z));
			glm::vec3 walkRight = glm::normalize(glm::cross(walkForward, glm::vec3(0.0f, 1.0f, 0.0f)));

			glm::vec3 direccionMovimiento(0.0f);
			float velocidadModelo = 1.0f * deltaTime;

			// 
			if (mainWindow.getsKeys()[GLFW_KEY_W]) direccionMovimiento += walkForward;
			if (mainWindow.getsKeys()[GLFW_KEY_S]) direccionMovimiento -= walkForward;
			if (mainWindow.getsKeys()[GLFW_KEY_A]) direccionMovimiento -= walkRight;
			if (mainWindow.getsKeys()[GLFW_KEY_D]) direccionMovimiento += walkRight;

			// 
			if (glm::length(direccionMovimiento) > 0.0f) {
				direccionMovimiento = glm::normalize(direccionMovimiento);
				PersonajeMain += direccionMovimiento * velocidadModelo;
				rotacionModelo = glm::degrees(atan2(direccionMovimiento.x, direccionMovimiento.z));
			}

			vistaActual = cameraTP.calculateViewMatrix();
			posOjo = cameraTP.getCameraPosition();
			dirOjo = cameraTP.getCameraDirection();
		}
		else if (tipoCamara == 2) { // AÉREA (NUEVA)
			cameraA.keyControl(mainWindow.getsKeys(), deltaTime);
			// mouseControl no hace nada por definición en CameraA
			vistaActual = cameraA.calculateViewMatrix();
			posOjo = cameraA.getCameraPosition();
			dirOjo = cameraA.getCameraDirection();
		}
		else if (tipoCamara == 3) {
			vistaActual = cameraF.calculateViewMatrix();
			posOjo = cameraF.getCameraPosition();
			dirOjo = cameraF.getCameraDirection();
		}
		else if (tipoCamara == 4) {
			vistaActual = cameraF2.calculateViewMatrix();
			posOjo = cameraF2.getCameraPosition();
			dirOjo = cameraF2.getCameraDirection();
		}
		else {
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			vistaActual = camera.calculateViewMatrix();
			posOjo = camera.getCameraPosition();
			dirOjo = camera.getCameraDirection();
		}


		//para el incremento en las rotaciones de los engranajes
		rotEngranaje += rotEngranajeOffset * deltaTime;
		rotEngranaje2 += rotEngranaje2Offset * deltaTime;
		rotEngranaje3 += rotEngranaje3Offset * deltaTime;
		rotEngranaje4 += rotEngranaje4Offset * deltaTime;

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
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(vistaActual));
		glUniform3f(uniformEyePosition, posOjo.x, posOjo.y, posOjo.z);


		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		///#####


		//######
		// Ciclo Dia Noche
		// Cálculo de tiempo y posición del sol
		float tiempoActual = (float)glfwGetTime();
		float progreso = fmod(tiempoActual, duracionCiclo) / duracionCiclo;
		float anguloSol = progreso * 2.0f * 3.14159265f;

		float dirX = sin(anguloSol);
		float dirY = cos(anguloSol);

		// Transiciones
		float factorDia = glm::smoothstep(-0.2f, 0.3f, dirY);
		float facAt = glm::smoothstep(0.4f, -0.1f, std::abs(dirY));

		// Mezcla dinámica 
		glm::vec3 colorAtardecer = glm::mix(MedioDia, Atardecer, facAt);
		colorSol = glm::mix(Noche, colorAtardecer, factorDia);

		if (dirY > 0) {
			diffuseIntensity = std::max(0.10f, dirY * 0.8f);
		}
		else {
			diffuseIntensity = 0.10f;
		}

		mainLight = DirectionalLight(
			colorSol.r, colorSol.g, colorSol.b,
			ambientIntensity,
			diffuseIntensity,
			dirX, dirY, dirZ
		);

		//#####


		shaderList[0].SetDirectionalLight(&mainLight);
		PointLight lucesActivas[MAX_POINT_LIGHTS];
		unsigned int conteoActivas = 0;
		lucesActivas[conteoActivas] = pointLights[0];
		conteoActivas++;
		lucesActivas[conteoActivas] = pointLights[2];
		conteoActivas++;
		if (dirY < 0) {
			lucesActivas[conteoActivas] = pointLights[1];
			conteoActivas++;
			lucesActivas[conteoActivas] = pointLights[3];
			conteoActivas++;
			lucesActivas[conteoActivas] = pointLights[4];
			conteoActivas++;
			lucesActivas[conteoActivas] = pointLights[5];
			conteoActivas++;

		}
		shaderList[0].SetPointLights(lucesActivas, conteoActivas);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		//reiniciar variables antes de que sean enviadas al shader

		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);

		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f, 1.0f, 15.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();


		//#####
		model = glm::mat4(1.0);
		model = glm::translate(model, PersonajeMain);
		model = glm::rotate(model, glm::radians(rotacionModelo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		testTexture.UseTexture();
		meshList[0]->RenderMesh();



		//DOOM Models

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, -2.0f, 55.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GoreNest.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, -2.0f, 35.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GoldNest.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 50.0f, 0.0f));
		model = glm::scale(model, glm::vec3(250.0f, 250.0f, 250.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Esfera.RenderModel();

		//Cacodemon movimiento

		float diffY = cacoTarget.y - cacoPos.y;

		if (std::abs(diffY) > 0.1f) {
			targetPitch = (diffY > 0) ? -15.0f : 15.0f;
		}
		else {
			targetPitch = -5.0f;
		}

		cacoPitch += (targetPitch - cacoPitch) * 0.05f * deltaTime;

		glm::vec3 direccion = cacoTarget - cacoPos;
		float distancia = glm::length(glm::vec2(direccion.x, direccion.z)); 

		if (distancia > 0.5f) {
			glm::vec3 dirNormalizada = glm::normalize(direccion);
			cacoPos.x += dirNormalizada.x * cacoVel * deltaTime;
			cacoPos.z += dirNormalizada.z * cacoVel * deltaTime;
			float targetAngle = glm::degrees(atan2(dirNormalizada.x, dirNormalizada.z));
			cacoAngle = targetAngle;
		}
		else {
			cacoTarget.x = (rand() % 40) - 20.0f;
			cacoTarget.z = (rand() % 40) - 20.0f;
		}

		timerCacoY += deltaTime * 0.01f; 
		if (timerCacoY >= intervaloCacoY) {
			cacoTarget.y = (rand() % 8) + 2.0f; 
			timerCacoY = 0.0f;
			intervaloCacoY = (rand() % 5) + 3.0f;
		}

		cacoPos.y += (cacoTarget.y - cacoPos.y) * 0.01f * deltaTime;

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-90.0f, 15.0f, 35.0f));
		model = glm::translate(model, cacoPos);
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, cacoAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, cacoPitch * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		float balanceoZ = sin(glfwGetTime() * 1.5f) * 3.0f;
		model = glm::rotate(model, balanceoZ * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		float flotado = sin(glfwGetTime() * 2.0f) * 0.2f;
		model = glm::translate(model, glm::vec3(0.0f, flotado, 0.0f));
		model = glm::scale(model, glm::vec3(0.290f, 0.290f, 0.290f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cacodemon.RenderModel();

		//Marauder

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, -2.0f, 45.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MarauderB.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 53.0f, -7.05f));
		model = glm::rotate(model, -80 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MarauderL.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 50.0f, 7.05f));
		model = glm::rotate(model, 80 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MarauderR.RenderModel();

		//Repetir cuantas veces sea necesario, para dar mas iluminacion por la noche.
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 0.0f, 45.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.5f, 7.5f, 7.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Farola.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 15.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.5f, 7.5f, 7.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Farola.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, 0.0f, 45.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.5f, 7.5f, 7.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Farola.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-80.0f, 0.0f, 15.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.5f, 7.5f, 7.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Farola.RenderModel();


		//Arboleda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-75.0f, 0.0f, -50.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arboles1.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, -20.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, -20.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();

		//####
		// 
		// 
		//FORTALEZA DEL DR. WILLY
		//el esqueleto: muralla, torres, torres con cañon, calavera, mansion, cono superior
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, -2.0f, 100.0f));
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

		//PARA LA ANIMACION DE ZERO
		//para el inicio de la animacion del hombro derecho de zero
		if (mainWindow.getsKeys()[GLFW_KEY_Z]) {//cuando se presione la tecla Z
			inicio = 1;//bandera
		}

		//para el reseteo de la animacion en general
		if (mainWindow.getsKeys()[GLFW_KEY_X]) {//cuando se presione la tecla X
			//para el hombro der
			inicio = 0;
			timerEspera = 0;
			rotHombroD = 0;

			//para el hombro izq
			siguiente = 0;
			timerHI = 0;
			rotHombroI = 0;
			rotCodoI = 0;

			//para la esfera de energia
			disparo1 = 0;
			disparo2 = 0;
			disparo3 = 0;
			timerD = 0;
			timerD2 = 0;
			timerD3 = 0;
			movEsferaE = 0;
			movEsferaE2 = 0;
			movEsferaE3 = 0;
			nuevoDisparo = 0;
		}

		//para que se repita las veces que sea los tres disparos
		if (mainWindow.getsKeys()[GLFW_KEY_C]) {//cuando se presione la tecla C
			disparo1 = 1;
			disparo2 = 0;
			disparo3 = 0;
			timerD = 0;
			timerD2 = 0;
			timerD3 = 0;
			movEsferaE = 0;
			movEsferaE2 = 0;
			movEsferaE3 = 0;
		}

		//PARA EL HOMBRO DERECHO
		if (inicio) {
			timerEspera++;//inicio del timer
		}

		if (timerEspera < 300 and inicio) {//de 0 a 300
			if (rotHombroD < 90) {//para que el limite de la rotacion sea 90º
				rotHombroD += rotHombroDOffset * deltaTime;//incremento
			}
			/*else {
				siguiente = 1;
				//printf("\ntimerEspera %f", timerEspera);
			}//*/
		}

		if (timerEspera > 50) {//para activar la siguiente animacion (hombro izq) a la mitad de la animacion del hombro der
			siguiente = 1;
		}

		//PARA EL HOMBRO IZQUIERDO
		if (siguiente) {
			timerHI++;
		}

		if (timerHI < 300 and siguiente) {//de 0 a 300
			if (rotHombroI < 90) {//para que el limite de la rotacion sea 90º
				rotHombroI += rotHombroIOffset * deltaTime;//incremento
				rotCodoI += rotCodoIOffset * deltaTime;
			}
			else {
				disparo1 = 1;
				nuevoDisparo = 1;
				//printf("\ntimerEspera %f", timerEspera);
			}//*/
		}

		//PARA LAS ESFERAS DE ENERGIA
		//primera esfera de energia
		if (disparo1 and nuevoDisparo) {
			timerD++;
		}

		if (timerD < 1000 and nuevoDisparo) {//de 0 a 1000
			movEsferaE += movEsferaEOffset * deltaTime;//incremento
		}

		if (timerD > 30) {
			disparo2 = 1;
		}

		//segunda esfera de energia
		if (disparo2) {
			timerD2++;
		}

		if (timerD2 < 1000 and disparo2) {//de 0 a 1000
			movEsferaE2 += movEsferaE2Offset * deltaTime;//incremento
		}

		if (timerD2 > 30) {
			disparo3 = 1;
		}

		//tercera esfera de energia
		if (disparo3) {
			timerD3++;
		}

		if (timerD3 < 1000 and disparo3) {//de 0 a 1000
			movEsferaE3 += movEsferaE3Offset * deltaTime;//incremento
		}


		//ZERO
		//cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, -2.0f, 90.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelauxZ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ZeroCuerpo_M.RenderModel();//*/

		//hombro derecho
		model = modelauxZ;
		model = glm::translate(model, glm::vec3(-0.265f, 3.125f, -0.05f));
		if (inicio) {
			model = glm::rotate(model, -rotHombroD * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelauxHDerZ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ZeroHombroDer_M.RenderModel();//*/

		//brazo derecho
		model = modelauxHDerZ;
		model = glm::translate(model, glm::vec3(-0.2f, -0.17625f, 0.0f));
		modelauxHDerZ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ZeroBrazoDer_M.RenderModel();//*/

		//hombro izq
		model = modelauxZ;
		model = glm::translate(model, glm::vec3(0.3245f, 3.10125f, -0.05f));
		if (siguiente) {
			model = glm::rotate(model, -rotHombroI * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelauxHIzqZ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ZeroHombroIzq_M.RenderModel();//*/

		//brazo izq
		model = modelauxHIzqZ;
		model = glm::translate(model, glm::vec3(0.15125f, -0.17625f, 0.025f));
		modelauxHIzqZ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ZeroBrazoIzq_M.RenderModel();//*/

		//codo izq
		model = modelauxHIzqZ;
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		if (siguiente) {
			model = glm::rotate(model, -rotCodoI * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, -rotCodoI * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelauxHIzqZ = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ZeroCodoIzq_M.RenderModel();//*/

		//mano izq
		model = modelauxHIzqZ;
		model = glm::translate(model, glm::vec3(0.02f, 0.01f, 0.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ZeroManoIzq_M.RenderModel();//*/

		//primera esfera de energia
		model = modelauxHDerZ;
		model = glm::translate(model, glm::vec3(0.01f, -0.8f, -0.04f));
		if (disparo1) {
			model = glm::translate(model, glm::vec3(0.0f, -movEsferaE - 0.0f, 0.0f));
		}
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ZeroEsferaE_M.RenderModel();//*/

		//segunda esfera de energia
		model = modelauxHDerZ;
		model = glm::translate(model, glm::vec3(0.01f, -0.8f, -0.04f));
		if (disparo1) {
			model = glm::translate(model, glm::vec3(0.0f, -movEsferaE2 - 0.0f, 0.0f));
		}
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ZeroEsferaE_M.RenderModel();//*/

		//tercera esfera de energia
		model = modelauxHDerZ;
		model = glm::translate(model, glm::vec3(0.01f, -0.8f, -0.04f));
		if (disparo1) {
			model = glm::translate(model, glm::vec3(0.0f, -movEsferaE3 - 0.0f, 0.0f));
		}
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ZeroEsferaE_M.RenderModel();//*/

		//CASA del arbol (ADventure time)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(85.0f, -4.0f, -50.0f));
		model = glm::scale(model, glm::vec3(0.65f, 0.65f, 0.65f));
		model = glm::rotate(model, 5 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CasaArbol_M.RenderModel();//*/

		//Molino
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, -5.0f, 120.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Molino_M.RenderModel();

		//reloj principal

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		relojM.RenderModel();

		//bmo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, -1.0f, 120.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BMO_M.RenderModel();

		//jake
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 120.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		jake_M.RenderModel();

		//vias
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-135.0f, -1.75f, 100.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vias_M.RenderModel();

		//vias 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-135.0f, -1.75f, -20.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vias_M.RenderModel();


		//vias 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(90.0f, -1.9f, -130.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vias_M.RenderModel();
		//TREN
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(90.0f, 10.0f, -130.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tren_M.RenderModel();

		//PARA LA ANIMACION DE MEGA MAN
		//para el inicio de la animacion
		if (mainWindow.getsKeys()[GLFW_KEY_M]) {//cuando se presione la tecla M
			mega = 1;//bandera
		}

		//para el reseteo de la animacion en general
		if (mainWindow.getsKeys()[GLFW_KEY_N]) {//cuando se presione la tecla N
			//para el hombro derecho
			mega = 0;
			timerM = 0;
			rotHombroDM = 0;
			rotHombroDM1 = 0;
			rotHombroDM2 = 0;
			mega1 = 0;
			rotHombroDM3 = 0;

			//para el hombro izquierdo
			mega2 = 0;
			rotHombroIM = 0;

		}

		if (mega) {
			timerM++;
		}

		if (timerM < 450 and mega) {//de 0 a 600
			if (rotHombroDM < 180) {//para que el limite de la rotacion sea 90º
				rotHombroDM += rotHombroDMOffset * deltaTime;//incremento
				if (rotHombroDM1 < 20) {
					rotHombroDM1 += rotHombroDM1Offset * deltaTime;
				}
				if (rotHombroDM2 < 90) {
					rotHombroDM2 += rotHombroDM2Offset * deltaTime;
				}
			}
			else {
				//siguiente = 1;
				//printf("\ntimerEspera %f", timerM);
			}//*/
		}

		if (timerM > 300) {
			mega1 = 1;
			mega2 = 1;
		}

		if (mega1) {
			if (rotHombroDM3 < 50 and !cambio) {
				rotHombroDM3 += rotHombroDM3Offset * deltaTime;
			}
			else {
				cambio = 1;
			}

			if (rotHombroDM3 > 5 and cambio) {
				rotHombroDM3 -= rotHombroDM3Offset * deltaTime;
			}
			else {
				cambio = 0;
			}
		}

		if (mega2) {
			if (rotHombroIM < 5 and !cambioI) {
				rotHombroIM += rotHombroIMOffset * deltaTime;
			}
			else {
				cambioI = 1;
			}
			if (rotHombroIM > 0 and cambioI) {
				rotHombroIM -= rotHombroIMOffset * deltaTime;
			}
			else {
				cambioI = 0;
			}
		}

		//MEGAMAN
		//cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, -2.0f, 90.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		if (mega2) {
			model = glm::rotate(model, rotHombroIM * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		modelauxM = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MegaManCuerpo_M.RenderModel();

		//brazo derecho
		model = modelauxM;
		model = glm::translate(model, glm::vec3(-0.3762f, 2.7191f, -0.1178f));
		model = glm::rotate(model, -20 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));//inclinacion inicial
		if (mega) {
			model = glm::rotate(model, -rotHombroDM * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));//rota el brazo para que este arriba
			model = glm::rotate(model, -rotHombroDM1 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));//reacomoda el brazo de la rotacion inicial
			model = glm::rotate(model, rotHombroDM2 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));//gira el brazo, para que la palma este de frente
		}
		//model = glm::rotate(model, -180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));//rota el brazo para que este arriba
		//model = glm::rotate(model, -20 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));//reacomoda el brazo de la rotacion inicial
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));//gira el brazo, para que la palma este de frente
		if (mega1) {
			model = glm::rotate(model, rotHombroDM3 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));//rota la mano para hacer el saludo, el limite
		}
		//model = glm::rotate(model, 45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));//rota la mano para hacer el saludo, el limite
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MegaManBrazoDer_M.RenderModel();

		//brazo izquierdo
		model = modelauxM;
		model = glm::translate(model, glm::vec3(0.3762f, 2.7191f, -0.1178f));
		model = glm::rotate(model, 20 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		if (mega2) {
			model = glm::rotate(model, rotHombroIM * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MegaManBrazoIzq_M.RenderModel();

		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();

	}
	return 0;

}

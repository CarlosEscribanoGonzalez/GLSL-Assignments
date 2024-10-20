#include "BOX.h"
#include "auxiliar.h"


#include <gl/glew.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h>  //Glut se encarga de coger eventos de interactuar con el sistema operativo

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>


//////////////////////////////////////////////////////////////
// Datos que se almacenan en la memoria de la CPU
//////////////////////////////////////////////////////////////

//Matrices
glm::mat4	proj = glm::mat4(1.0f);
glm::mat4	view = glm::mat4(1.0f);
glm::mat4	model = glm::mat4(1.0f);
glm::mat4	model2 = glm::mat4(1.0f);

float viewAlterationZ = 0.0f;
float viewAlterationX = 0.0f;
float viewAlterationRotation = 0.0f;

static float angle;

//////////////////////////////////////////////////////////////
// Variables que nos dan acceso a Objetos OpenGL
//////////////////////////////////////////////////////////////
//Shaders
unsigned int vshader;
unsigned int vshader2;

unsigned int fshader;
unsigned int fshader2;

unsigned int program;
unsigned int program2;

//Texturas
unsigned int colorTexId;
unsigned int colorTexId2;

unsigned int emiTexId;
unsigned int emiTexId2;

//Variables Uniform
int uViewMat;
int uViewMat2;
int uModelViewMat;
int uModelViewProjMat;
int uModelViewMat2;
int uModelViewProjMat2;
int uNormalMat;
int uNormalMat2;

int uLightPos;
int uLightPos2;
int uLightI;
int uLightI2;

//Texturas Uniform
int uColorTex;
int uColorTex2;

int uEmiTex;
int uEmiTex2;


//Atributos
int inPos;
int inPos2;

int inColor;
int inColor2;

int inNormal;
int inNormal2;

int inTexCoord;
int inTexCoord2;

glm::vec3 lPos = glm::vec3(0.0, 0.0, 5.0);
glm::vec3 lInt = glm::vec3(1.0f);

//VAO
unsigned int vao;
//VBOs que forman parte del objeto
unsigned int posVBO;
unsigned int colorVBO;
unsigned int normalVBO;
unsigned int texCoordVBO;
unsigned int triangleIndexVBO;

//Declaración de CB
void renderFunc();
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);

void initContext(int argc, char** argv);
void initOGL();
void destroy();
void initShader(const char* vname, const char* fname);
void initShader2(const char* vname, const char* fname);
void initObj();

GLuint loadShader(const char* fileName, GLenum type);
unsigned int loadTex(const char* fileName);

int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)

	initContext(argc, argv);
	initOGL();
	initShader("../shaders_P3/shader.v1.vert", "../shaders_P3/shader.v1.frag");
	initShader2("../shaders_P3/shader.v2.vert", "../shaders_P3/shader.v2.frag");
	initObj();
	glutMainLoop();
	destroy();

	return 0;
}

//////////////////////////////////////////
// Funciones auxiliares 
// 
//Funciones de inicialización y destrucción
void initContext(int argc, char** argv) { //Esto es para pasarle parámetros si lo hicieramos
	//Se crea el contexto
	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	//Se crea el Frame Buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Prácticas OGL");
	//Inicación de extensiones
	glewExperimental = GL_TRUE; //Para cargar extensiones aunque no funcionen en la tarjeta gráfica que tengamos
	GLenum err = glewInit();
	if (GLEW_OK != err) //Comprueba si todo está bien o hay algun problema en la iniciación
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		exit(-1); //si hay algo mal me sale -1 en la pantalla
	}
	const GLubyte* oglVersion = glGetString(GL_VERSION);
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;
	//Con esto a glut se le define que funciones callbacks se llaman cuando se produce un evento
	glutReshapeFunc(resizeFunc);
	glutDisplayFunc(renderFunc);
	glutIdleFunc(idleFunc);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);
}

void initOGL() {
	glEnable(GL_DEPTH_TEST); //Comprobamos qué fragmentos tienen que colorearse dependiendo de lo cerca que este de la pantalla (mira su z)
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	glFrontFace(GL_CCW); //Counterclockwise. Se define las caras al sentido contrario de las agujas del reloj
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Se llenan las caras frontal y trasera
	glEnable(GL_CULL_FACE);

	proj = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 50.0f);
	view = glm::mat4(1.0f);
	view[3].z = -12;
}

void destroy() {
	glDetachShader(program, vshader);
	glDetachShader(program, fshader);
	glDeleteShader(vshader);
	glDeleteShader(fshader);
	glDeleteProgram(program);

	glDetachShader(program2, vshader2);
	glDetachShader(program2, fshader2);
	glDeleteShader(vshader2);
	glDeleteShader(fshader2);
	glDeleteProgram(program2);

	if (inPos != -1) glDeleteBuffers(1, &posVBO);
	if (inColor != -1) glDeleteBuffers(1, &colorVBO);
	if (inNormal != -1) glDeleteBuffers(1, &normalVBO);
	if (inTexCoord != -1) glDeleteBuffers(1, &texCoordVBO);
	if (inPos2 != -1) glDeleteBuffers(1, &posVBO); 
	if (inColor2 != -1) glDeleteBuffers(1, &colorVBO);
	if (inNormal2 != -1) glDeleteBuffers(1, &normalVBO);
	if (inTexCoord2 != -1) glDeleteBuffers(1, &texCoordVBO);
	glDeleteBuffers(1, &triangleIndexVBO);
	glDeleteVertexArrays(1, &vao);

	glDeleteTextures(1, &colorTexId);
	glDeleteTextures(1, &emiTexId);
	glDeleteTextures(1, &colorTexId2);
	glDeleteTextures(1, &emiTexId2);
}

void initShader(const char* vname, const char* fname) {
	vshader = loadShader(vname, GL_VERTEX_SHADER);
	fshader = loadShader(fname, GL_FRAGMENT_SHADER);
	//Se activa un programa con todos los subprogramas
	program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);

	glBindAttribLocation(program, 0, "inPos");
	glBindAttribLocation(program, 1, "inColor");
	glBindAttribLocation(program, 2, "inNormal");
	glBindAttribLocation(program, 3, "inTexCoord");

	glLinkProgram(program);

	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		char* logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;

		glDeleteProgram(program);
		program = 0;
		exit(-1);
	}

	uNormalMat = glGetUniformLocation(program, "normal");
	uModelViewMat = glGetUniformLocation(program, "modelView");
	uModelViewProjMat = glGetUniformLocation(program, "modelViewProj");
	uViewMat = glGetUniformLocation(program, "view");

	uLightPos = glGetUniformLocation(program, "lightPos");
	uLightI = glGetUniformLocation(program, "lightI");

	inPos = glGetAttribLocation(program, "inPos");
	inColor = glGetAttribLocation(program, "inColor");
	inNormal = glGetAttribLocation(program, "inNormal");
	inTexCoord = glGetAttribLocation(program, "inTexCoord");

	uColorTex = glGetUniformLocation(program, "colorTex");
	uEmiTex = glGetUniformLocation(program, "emiTex");
}

void initShader2(const char* vname, const char* fname) {
	vshader2 = loadShader(vname, GL_VERTEX_SHADER);
	fshader2 = loadShader(fname, GL_FRAGMENT_SHADER);
	//Se activa un programa con todos los subprogramas
	program2 = glCreateProgram();
	glAttachShader(program2, vshader2);
	glAttachShader(program2, fshader2);

	glBindAttribLocation(program2, 0, "inPos");
	glBindAttribLocation(program2, 1, "inColor");
	glBindAttribLocation(program2, 2, "inNormal");
	glBindAttribLocation(program2, 3, "inTexCoord");

	glLinkProgram(program2);

	int linked;
	glGetProgramiv(program2, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetProgramiv(program2, GL_INFO_LOG_LENGTH, &logLen);
		char* logString = new char[logLen];
		glGetProgramInfoLog(program2, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;

		glDeleteProgram(program2);
		program2 = 0;
		exit(-1);
	}

	uNormalMat2 = glGetUniformLocation(program2, "normal");
	uModelViewMat2 = glGetUniformLocation(program2, "modelView");
	uModelViewProjMat2 = glGetUniformLocation(program2, "modelViewProj");
	uViewMat2 = glGetUniformLocation(program2, "view");

	uLightPos2 = glGetUniformLocation(program2, "lightPos");
	uLightI2 = glGetUniformLocation(program2, "lightI");

	inPos2 = glGetAttribLocation(program2, "inPos");
	inColor2 = glGetAttribLocation(program2, "inColor");
	inNormal2 = glGetAttribLocation(program2, "inNormal");
	inTexCoord2 = glGetAttribLocation(program2, "inTexCoord");

	uColorTex2 = glGetUniformLocation(program2, "colorTex");
	uEmiTex2 = glGetUniformLocation(program2, "emiTex");
}

void initObj() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	if (inPos != -1)
	{
		glGenBuffers(1, &posVBO);
		glBindBuffer(GL_ARRAY_BUFFER, posVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3,
			cubeVertexPos, GL_STATIC_DRAW);
		glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inPos);
	}
	if (inColor != -1)
	{
		glGenBuffers(1, &colorVBO);
		glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3,
			cubeVertexColor, GL_STATIC_DRAW);
		glVertexAttribPointer(inColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inColor);
	}
	if (inNormal != -1)
	{
		glGenBuffers(1, &normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3,
			cubeVertexNormal, GL_STATIC_DRAW);
		glVertexAttribPointer(inNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inNormal);
	}
	if (inTexCoord != -1)
	{
		glGenBuffers(1, &texCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 2,
			cubeVertexTexCoord, GL_STATIC_DRAW);
		glVertexAttribPointer(inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTexCoord);
	}

	glGenBuffers(1, &triangleIndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		cubeNTriangleIndex * sizeof(unsigned int) * 3, cubeTriangleIndex,
		GL_STATIC_DRAW);

	colorTexId = loadTex("../img/color2.png");
	emiTexId = loadTex("../img/emissive.png");

	model = glm::mat4(1.0f);

	if (inPos2 != -1)
	{
		glGenBuffers(1, &posVBO);
		glBindBuffer(GL_ARRAY_BUFFER, posVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3,
			cubeVertexPos, GL_STATIC_DRAW);
		glVertexAttribPointer(inPos2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inPos2);
	}
	if (inColor2 != -1)
	{
		glGenBuffers(1, &colorVBO);
		glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3,
			cubeVertexColor, GL_STATIC_DRAW);
		glVertexAttribPointer(inColor2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inColor2);
	}
	if (inNormal2 != -1)
	{
		glGenBuffers(1, &normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3,
			cubeVertexNormal, GL_STATIC_DRAW);
		glVertexAttribPointer(inNormal2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inNormal2);
	}
	if (inTexCoord2 != -1)
	{
		glGenBuffers(1, &texCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 2,
			cubeVertexTexCoord, GL_STATIC_DRAW);
		glVertexAttribPointer(inTexCoord2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTexCoord2);
	}

	glGenBuffers(1, &triangleIndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		cubeNTriangleIndex * sizeof(unsigned int) * 3, cubeTriangleIndex,
		GL_STATIC_DRAW);

	colorTexId2 = loadTex("../img/color2.png");
	emiTexId2 = loadTex("../img/emissive.png");

	model2 = glm::mat4(1.0f);
}


//Carga el shader indicado, devuele el ID del shader
//!Por implementar
GLuint loadShader(const char* fileName, GLenum type) {
	unsigned int fileLen;
	char* source = loadStringFromFile(fileName, fileLen);
	//////////////////////////////////////////////
	//Creación y compilación del Shader
	GLuint shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1,
		(const GLchar**)&source, (const GLint*)&fileLen);
	glCompileShader(shader);
	delete[] source;
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		//Calculamos una cadena de error
		GLint logLen;
		//Sacamos la longitud del log almacenado
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

		char* logString = new char[logLen];
		glGetShaderInfoLog(shader, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString; //Borra la cadena de texto

		glDeleteShader(shader); //Borra el shaders
		exit(-1); //Indica con -1 que hay un error
	}
	return shader;
}


unsigned int loadTex(const char* fileName) { 

	unsigned char* map;
	unsigned int w, h;
	map = loadTexture(fileName, w, h);
	if (!map)
	{
		std::cout << "Error cargando el fichero: "
			<< fileName << std::endl;
		exit(-1);
	}

	unsigned int texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, (GLvoid*)map);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

	delete[] map;


	return texId;
}

//Crea una textura, la configura, la sube a OpenGL, 
//y devuelve el identificador de la textura 
//!!Por implementar
//unsigned int loadTex(const char *fileName);


void renderFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program); //Activamos el shader
	//→ pintado del objeto!!!!
	model = glm::mat4(1.0f);
	model = glm::rotate(model, angle, glm::vec3(1.0f, 1.0f, 0.0f));

	glm::mat4 modelView = view * model;
	glm::mat4 modelViewProj = proj * view * model;
	glm::mat4 normal = glm::transpose(glm::inverse(modelView));

	if (uModelViewMat != -1)
		glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE,
			&(modelView[0][0]));

	if (uViewMat != -1)
		glUniformMatrix4fv(uViewMat, 1, GL_FALSE,
			&(view[0][0]));

	if (uModelViewProjMat != -1)
		glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE,
			&(modelViewProj[0][0]));

	if (uNormalMat != -1)
		glUniformMatrix4fv(uNormalMat, 1, GL_FALSE,
			&(normal[0][0]));

	if (uLightPos != -1)
		glUniform3fv(uLightPos, 1,
			&(lPos.x));

	if (uLightI != -1)
		glUniform3fv(uLightI, 1,
			&(lInt.x));

	//Texturas
	if (uColorTex != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorTexId);
		glUniform1i(uColorTex, 0);
	}
	if (uEmiTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, emiTexId);
		glUniform1i(uEmiTex, 1);
	}

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3,
		GL_UNSIGNED_INT, (void*)0);

	
	glUseProgram(program2);

	model2 = glm::mat4(1.0f);

	model2 = glm::rotate(model2, angle, glm::vec3(0.0f, 0.0f, 1.0f));

	model2 = glm::translate(model2, glm::vec3(0.0f, 4.0f, 0.0f));

	model2 = glm::rotate(model2, angle, glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 modelView2 = view * model2;
	glm::mat4 modelViewProj2 = proj * view * model2;
	glm::mat4 normal2 = glm::transpose(glm::inverse(modelView2));


	if (uModelViewMat2 != -1)
		glUniformMatrix4fv(uModelViewMat2, 1, GL_FALSE,
			&(modelView2[0][0]));

	if (uViewMat2 != -1)
		glUniformMatrix4fv(uViewMat2, 1, GL_FALSE,
			&(view[0][0]));

	if (uModelViewProjMat2 != -1)
		glUniformMatrix4fv(uModelViewProjMat2, 1, GL_FALSE,
			&(modelViewProj2[0][0]));

	if (uNormalMat2 != -1)
		glUniformMatrix4fv(uNormalMat2, 1, GL_FALSE,
			&(normal2[0][0]));

	if (uLightPos2 != -1)
		glUniform3fv(uLightPos2, 1,
			&(lPos.x));
	if (uLightI2 != -1)
		glUniform3fv(uLightI2, 1,
			&(lInt.x));

	//Texturas
	if (uColorTex2 != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorTexId2);
		glUniform1i(uColorTex2, 0);
	}
	if (uEmiTex2 != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, emiTexId2);
		glUniform1i(uEmiTex2, 1);
	}

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3,
		GL_UNSIGNED_INT, (void*)0);


	glutSwapBuffers(); //Fuerza un redibujado
}

void resizeFunc(int width, int height) {
	glViewport(0, 0, width, height);
	float aspect = float(width) / float(height);
	proj = glm::perspective(glm::radians(60.0f), aspect, 1.0f, 50.0f);
	glutPostRedisplay();
}

void idleFunc() {
	
	angle = (angle > 3.141592f * 2.0f) ? 0 : angle + 0.01f;

	glutPostRedisplay(); //Fuerza un redibujado
}

void keyboardFunc(unsigned char key, int x, int y) {
	//CAMBIO DE LA POSICIÓN E INTENSIDAD DE LA LUZ
	if (key == '1') {
		lInt += glm::vec3(0.1);
		viewAlterationZ = 0.0f;
		viewAlterationX = 0.0f;
		viewAlterationRotation = 0.0f;
	}
	if (key == '0') {
		lInt += glm::vec3(-0.1);
		viewAlterationZ = 0.0f;
		viewAlterationX = 0.0f;
		viewAlterationRotation = 0.0f;
	}
	if (key == '8') {
		lPos += glm::vec3(0.0, 0.1, 0.0);
		viewAlterationZ = 0.0f;
		viewAlterationX = 0.0f;
		viewAlterationRotation = 0.0f;
	}
	if (key == '2') {
		lPos += glm::vec3(0.0, -0.1, 0.0);
		viewAlterationZ = 0.0f;
		viewAlterationX = 0.0f;
		viewAlterationRotation = 0.0f;
	}
	if (key == '4') {
		lPos += glm::vec3(-0.1, 0.0, 0.0);
		viewAlterationZ = 0.0f;
		viewAlterationX = 0.0f;
		viewAlterationRotation = 0.0f;
	}
	if (key == '6') {
		lPos += glm::vec3(0.1, 0.0, 0.0);
		viewAlterationZ = 0.0f;
		viewAlterationX = 0.0f;
		viewAlterationRotation = 0.0f;
	}
	if (key == '7') {
		lPos += glm::vec3(0.0, 0.0, -1.0);
		viewAlterationZ = 0.0f;
		viewAlterationX = 0.0f;
		viewAlterationRotation = 0.0f;
	}
	if (key == '9') {
		lPos += glm::vec3(0.0, 0.0, 1.0);
		viewAlterationZ = 0.0f;
		viewAlterationX = 0.0f;
		viewAlterationRotation = 0.0f;
	}

	//CAMBIO DE LA POSICIÓN DE LA CÁMARA
	if (key == 'w' || key == 'W')
	{
		viewAlterationZ = 0.1f;
		viewAlterationX = 0.0f;
		viewAlterationRotation = 0.0f;
	}
	else if (key == 's' || key == 'S')
	{
		viewAlterationZ = -0.1f;
		viewAlterationX = 0.0f;
		viewAlterationRotation = 0.0f;
	}
	else if (key == 'a' || key == 'A')
	{
		viewAlterationX = 0.1f;
		viewAlterationZ = 0.0f;
		viewAlterationRotation = 0.0f;
	}
	else if (key == 'd' || key == 'D')
	{
		viewAlterationX = -0.1f;
		viewAlterationZ = 0.0f;
		viewAlterationRotation = 0.0f;
	}
	else if (key == 'q' || key == 'Q')
	{
		viewAlterationRotation = -0.1f;
		viewAlterationX = 0.0f;
		viewAlterationZ = 0.0f;
	}
	else if (key == 'e' || key == 'E')
	{
		viewAlterationRotation = 0.1f;
		viewAlterationX = 0.0f;
		viewAlterationZ = 0.0f;
	}

	view[3].z += viewAlterationZ;
	view[3].x += viewAlterationX;

	glm::mat4 rotation =
	{
		glm::vec4(cos(viewAlterationRotation), 0, -sin(viewAlterationRotation), 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(sin(viewAlterationRotation), 0, cos(viewAlterationRotation), 0),
		glm::vec4(0, 0, 0, 1)
	};
	view = rotation * view;
}

void mouseFunc(int button, int state, int x, int y) {}











#include "BOX.h"
#include <IGL/IGlib.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


//Idenficadores de los objetos de la escena
int objId = -1;
int objId2 = 0;
int objId3 = 1;

glm::mat4 view = glm::mat4(1.0);
float viewAlterationZ = 0.0f;
float viewAlterationX = 0.0f;
float viewAlterationRotation = 0.0f;

int xAnt = 0;
int yAnt = 0;


//Declaración de CB (Call Back, función de llamada cada vez que se va a realizar un evento)
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMotionFunc(int x, int y);


int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// Para poder poner los acentos
	if (!IGlib::init("../shaders_P1/shader.obligatorio2.vert", "../shaders_P1/shader.obligatorio2.frag")) //Directorio relativo. .. = "paso" para atrás. / = "paso" para alante.
		return -1;
   
	//Se ajusta la cámara
	view[3].z = -6.0f;

	glm::mat4 proj = glm::mat4(1.0);
	
	float f = 1.0f / tan(3.141592 / 6.0);
	float far = 20.0;
	float near = 0.1;

	proj[0].x = f;
	proj[1].y = f;
	proj[2].z = (far + near) / (near - far);
	proj[2].w = -1.0f;
	proj[3].z = (2.0 * far * near) / (near - far);
	proj[3].w = 0.0;
	IGlib::setProjMat(proj);
	IGlib::setViewMat(view);

	
	//Si no se da valor se cogen valores por defecto
	//IGlib::setProjMat(const glm::mat4 &projMat);
	//IGlib::setViewMat(const glm::mat4 &viewMat);

	//Creamos el objeto que vamos a visualizar
	objId = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex, 
			cubeVertexPos, cubeVertexColor, cubeVertexNormal,cubeVertexTexCoord, cubeVertexTangent);
	objId2 = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex,
		cubeVertexPos, cubeVertexColor, cubeVertexNormal, cubeVertexTexCoord, cubeVertexTangent);

	objId3 = IGlib::createObj(piramideNTriangleIndex, piramideNVertex, piramideTriangleIndex,
		piramideVertexPos, piramideVertexColor, piramideVertexNormal, piramideVertexTexCoord, piramideVertexTangent);
		
	glm::mat4 modelMat = glm::mat4(1.0f);
	IGlib::setModelMat(objId, modelMat);
	IGlib::setModelMat(objId2, modelMat);
	IGlib::setModelMat(objId3, modelMat);
	//Incluir texturas aquí.
	IGlib::addColorTex(objId, "../img/color.png");
	IGlib::addColorTex(objId2, "../img/color.png");
	IGlib::addColorTex(objId3, "../img/color.png");


	//CBs
	IGlib::setResizeCB(resizeFunc);
	IGlib::setIdleCB(idleFunc);
	IGlib::setKeyboardCB(keyboardFunc);
	IGlib::setMouseCB(mouseFunc);
	IGlib::setMouseMoveCB(mouseMotionFunc);

	
	//Mainloop
	IGlib::mainLoop();
	IGlib::destroy();
	return 0;
}

void resizeFunc(int width, int height)
{
	glm::mat4 proj = glm::mat4(0.0f);
	float ratio = static_cast<float>(width) / static_cast<float>(height);
	float near = 1.0f;
	float far = 20.0f;
	float right = ratio;
	float left = -ratio;
	float top = 1.0f;
	float bottom = -1.0f;
	proj[0].x = (2 * near) / (right - left); 
	proj[0].z = (right + left) / (right - left);
	proj[1].y = (2 * near) / (top - bottom); 
	proj[1].z = (top + bottom) / (top - bottom);
	proj[2].z = (-(far + near)) / (far - near);
	proj[2].w = -2 * (far * near) / (far - near);
	proj[3].z = -1;
	IGlib::setProjMat(proj);
}

void idleFunc()
{
	static float angle = 0.0f; //El valor de las statics se acumula, no se borra.
	angle = (angle < 3.141599f * 2.0f) ? angle + 0.1f : 0.0f;

	glm::mat4 modelMat = glm::mat4(1.0f);
	//modelMat = glm::translate(modelMat, glm::vec3(2.0f, 0.0f, 0.0f));
	modelMat = glm::rotate(modelMat, angle, glm::vec3(1.0f, 0.0f, 0.0f));//Rotación eje x
	modelMat = glm::rotate(modelMat, angle, glm::vec3(0.0f, 1.0f, 0.0f));//Rotación eje y
	//modelMat = glm::translate(modelMat, glm::vec3(2.0f, 0.0f, 0.0f));
	IGlib::setModelMat(objId, modelMat);
	

	static float angle2 = 0.0f; //El valor de las statics se acumula, no se borra.
	angle2 = (angle2 < 3.141599f * 2.0f) ? angle2 + 0.01f : 0.0f;

	glm::mat4 modelMat2 = glm::mat4(1.0f);

	modelMat2 = glm::rotate(modelMat2, angle2, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMat2 = glm::translate(modelMat2, glm::vec3(5.0f, 0.0f, 0.0f));
	modelMat2 = glm::rotate(modelMat2, angle2, glm::vec3(0.0f, 1.0f, 0.0f));
	
	
	IGlib::setModelMat(objId2, modelMat2);


	glm::mat4 modelMat3 = glm::mat4(1.0f);
	modelMat3 = glm::translate(modelMat3, glm::vec3(0.0, 4.0, 0.0));
	modelMat3 = glm::rotate(modelMat3, angle2, glm::vec3(0.0f, 1.0f, 0.0f));

	IGlib::setModelMat(objId3, modelMat3);
}

void keyboardFunc(unsigned char key, int x, int y)
{
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
	
	
	IGlib::setViewMat(view);
}

void mouseFunc(int button, int state, int x, int y)
{
	if (state==0)
		std::cout << "Se ha pulsado el botón ";
	else
		std::cout << "Se ha soltado el botón ";
	
	if (button == 0) std::cout << "de la izquierda del ratón " << std::endl;
	if (button == 1) std::cout << "central del ratón " << std::endl;
	if (button == 2) std::cout << "de la derecha del ratón " << std::endl;

	std::cout << "en la posición " << x << " " << y << std::endl << std::endl;
}

void mouseMotionFunc(int x, int y)
{
	float rotationX;
	float rotationY;
	
	
	if (x > xAnt) 
	{
		rotationX = 0.1f;
	}
	else
	{
		rotationX = -0.1f;
	}

	if (y > yAnt)
	{
		rotationY = 0.1f;
	} 
	else
	{
		rotationY = -0.1f;
	}

	xAnt = x;
	yAnt = y;

	view = glm::rotate(view, rotationX, glm::vec3(0.0, 1.0, 0.0));
	view = glm::rotate(view, rotationY, glm::vec3(1.0, 0.0, 0.0));

	IGlib::setViewMat(view);
}

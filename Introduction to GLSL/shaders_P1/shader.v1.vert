#version 330 core

in vec3 inPos;
#define M_PI 3.141519653

//HAY QUE DEFINIR LA MATRIZ DE PROYECCIÓN. 
//De esta manera, se convierten las coordenadas del mundo a coordenadas de la cámara.

void main()
{
	mat4 modelView = mat4(1.0);
	modelView[3].z = -12;
	mat4 proj = mat4(1.0);
	float f = 1.0/tan(M_PI/6); //f es la focal, la apertura que tiene la cámara (60º convertidos a radianes)
	float far = 15.0;
	float near = 0.1;

	proj[0].x = f;
	proj[1].y = f;
	proj[2].z = (far + near)/(near-far);
	proj[2].w = -1.0;
	proj[3].z = (2.0*far*near)/(near-far);
	proj[3].w = 0.0;
	
	gl_Position = proj*modelView*vec4(inPos,1); //El 1 de vec4 se pone para que sean coordenadas homogéneas
}

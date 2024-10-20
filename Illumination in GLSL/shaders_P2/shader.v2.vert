#version 330 core

uniform mat4 normal;
uniform mat4 modelView;
uniform mat4 modelViewProj;

in vec3 inPos;
in vec3 inNormal;
in vec3 inColor;

out vec3 color;
out vec3 pos;
out vec3 norm;

void main()
{
	color = inColor;
	norm = (normal*vec4(inNormal, 0.0)).xyz; //Dentro de la matriz normal est� lo necesario para transformal coordenadas del objeto a las de la c�mara
	pos = (modelView*vec4(inPos, 1.0)).xyz; //El .xyz es porque la multiplicaci�n da un vec4, pero s�lo nos interesa lo que conformar�a el vec3

	gl_Position =  modelViewProj * vec4 (inPos,1.0);
}


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
	norm = (normal*vec4(inNormal, 0.0)).xyz; //Dentro de la matriz normal está lo necesario para transformal coordenadas del objeto a las de la cámara
	pos = (modelView*vec4(inPos, 1.0)).xyz; //El .xyz es porque la multiplicación da un vec4, pero sólo nos interesa lo que conformaría el vec3

	gl_Position =  modelViewProj * vec4 (inPos,1.0);
}


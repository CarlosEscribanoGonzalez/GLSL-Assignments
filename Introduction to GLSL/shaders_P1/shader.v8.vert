#version 330 core

in vec3 inPos;
in vec3 inNormal;
uniform mat4 modelViewProj; //La librería las ha multiplicado ya. Vienen premultiplicadas. Más eficiente.


out vec3 norm;


void main()
{
	norm = inNormal;	
	gl_Position = modelViewProj * vec4 (inPos,1.0);
}

#version 330 core

in vec3 inPos;
in vec3 inColor;
uniform mat4 modelViewProj; //La librería las ha multiplicado ya. Vienen premultiplicadas. Más eficiente.


out vec3 color;


void main()
{
	color = inColor;	
	gl_Position = modelViewProj*vec4(inPos,1);
}

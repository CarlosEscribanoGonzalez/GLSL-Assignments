#version 330 core

in vec3 inPos;
in vec3 inColor;
uniform mat4 modelViewProj; //La librer�a las ha multiplicado ya. Vienen premultiplicadas. M�s eficiente.


out vec3 color;


void main()
{
	color = inColor;	
	gl_Position = modelViewProj*vec4(inPos,1);
}

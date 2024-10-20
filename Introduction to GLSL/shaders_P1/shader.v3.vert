#version 330 core

in vec3 inPos;
uniform mat4 modelViewProj; //La librer�a las ha multiplicado ya. Vienen premultiplicadas. M�s eficiente.


void main()
{
	gl_Position = modelViewProj*vec4(inPos,1);
}

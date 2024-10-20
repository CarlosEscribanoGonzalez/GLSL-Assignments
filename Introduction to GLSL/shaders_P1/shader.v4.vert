#version 330 core

in vec3 inPos;
uniform mat4 modelViewProj; //La librería las ha multiplicado ya. Vienen premultiplicadas. Más eficiente.


void main()
{
	gl_Position = modelViewProj*vec4(inPos,1);
}

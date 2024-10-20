#version 330 core

in vec3 inPos;
in vec2 inTexCoord;
uniform mat4 modelViewProj; //La librería las ha multiplicado ya. Vienen premultiplicadas. Más eficiente.


out vec2 texCoord;


void main()
{
	texCoord = inTexCoord;	
	gl_Position = modelViewProj * vec4 (inPos,1);
}

#version 330 core

in vec3 inPos;
uniform mat4 modelViewProj; 
in vec2 inTexCoord;


out vec2 texCoord;


void main()
{
	texCoord = inTexCoord;	
	gl_Position = modelViewProj*vec4(inPos,1);
}

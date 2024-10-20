#version 330 core

in vec3 inPos;
uniform mat4 modelViewProj; 
uniform mat4 modelView;
in vec2 inTexCoord;
in vec3 inNormal;

out vec2 texCoord;
out vec3 norm;

void main()
{
	texCoord = inTexCoord;	
	norm = (transpose(inverse(modelView)) * vec4(inNormal,0)).xyz;	
	gl_Position = modelViewProj*vec4(inPos,1);
}

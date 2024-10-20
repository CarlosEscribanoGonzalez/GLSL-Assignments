#version 330 core

in vec3 norm;
in vec2 texCoord;

out vec4 outColor;
uniform sampler2D colorTex;


void main()
{

	if(mod(gl_PrimitiveID, 2.0) == 0.0)
	{ 
		outColor = vec4(norm,1.0);  
	} 
	else 
	{
		outColor = vec4(texCoord, 0.0, 1.0);
	} 
}

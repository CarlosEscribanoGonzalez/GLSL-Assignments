#version 330 core

in vec3 norm;
out vec4 outColor;


void main()
{

	outColor = vec4(norm, 1.0);
	 
}

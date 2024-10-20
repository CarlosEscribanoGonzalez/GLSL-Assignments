#version 330 core


in vec2 texCoord;

out vec4 outColor;
uniform sampler2D colorTex;


void main()
{

	if(pow(texCoord.x - 0.5, 2) + pow(texCoord.y - 0.5, 2) < 0.25)
	{ 
		outColor = texture(colorTex, texCoord);  
	} 
	else 
	{
		discard;
	} 
}

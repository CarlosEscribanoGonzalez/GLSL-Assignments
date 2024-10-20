#version 330 core


in vec2 texCoord;

out vec4 outColor;
uniform sampler2D colorTex;


void main()
{
	if(texCoord.x < 0.5 && texCoord.y > 0.5+texCoord.x || texCoord.x < 0.5 && texCoord.y <= 0.5-texCoord.x || 
		texCoord.x > 0.5 && texCoord.y >= 1-texCoord.x+0.5 || texCoord.x > 0.5 && texCoord.y <= texCoord.x-0.5)
	{
		outColor = vec4(0.5, 0.0, 1.0, 1.0);
	}
	else 
	{
		outColor = texture(colorTex, texCoord);  
	} 
}

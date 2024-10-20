#version 330 core

out vec4 outColor;

in vec2 texCoord; //Atributo de UVs en vértices de objeto
in vec3 pos;
in vec3 norm; //normal en espacio de cámara/vista

uniform sampler2D colorTex;
uniform sampler2D emiTex;
uniform sampler2D specularTex;

//propiedades de la luz
vec3 Ia = vec3(0.5); //luz ambiente

//Primera luz: direccional
vec3 Id1 = vec3(0.9); //luz difusa
vec3 Is1 = vec3(0.9); //luz especular
vec3 lpos1 = vec3(0.0, 3.0, 3.0); 
//La luz direccional no tiene atenuación

//Segunda luz: focal
vec3 Id2 = vec3(25.0);
vec3 Is2 = vec3(25.0);
vec3 lpos2 = vec3(-1, 0.0, -3.0);
float d2 = distance(lpos2, pos);
float fdist2 = pow(1/d2, 2);

//propiedades del material-objeto
vec3 Ka; //componente ambiente
vec3 Kd; //componente difusa
vec3 Ks; //componente especular
vec3 Ke; //componente emisiva
vec3 N; //normal en el espacio de vista
float alpha = 3.0;

vec3 shade();

void main()
{
	Ka = texture(colorTex, texCoord).rgb;
	Kd = texture(colorTex, texCoord).rgb;
	Ke = texture(emiTex, texCoord).rgb;
	Ks = texture(specularTex, texCoord).rgb; 

	N = normalize(norm);
	outColor = vec4(shade(),1.0);
}

vec3 shade()
{
	//Luz ambiental:
	vec3 c = vec3(0.0); //color negro por defecto
	c += Ia*Ka; 
	
	//Primera luz: direccional
	vec3 L1 = normalize(lpos1); 
	vec3 diffuse1 = Id1*Kd*dot(L1,N); 
	c += clamp(diffuse1, 0.0, 1.0); 

	vec3 V1 = normalize(-pos); 
	vec3 R1 = normalize(reflect(-L1,N)); 
	float factor1 = clamp(dot(R1,V1),0.0,1.0); 
	vec3 specular1 = Is1*Ks*pow(factor1,alpha); 
	c += clamp(specular1, 0.0, 1.0);

	//Segunda luz: focal
	vec3 L2 = normalize(lpos2-pos); 
	vec3 D = normalize(lpos2); //Dirección en la que mira la luz
	float fdir = pow(max(0, (dot(-L2, D) - cos(30))/(1-cos(30))), 10);
	vec3 diffuse2 = fdir*fdist2*Id2*Kd*dot(L2,N); 
	c += clamp(diffuse2, 0.0, 1.0); 

	vec3 V2 = normalize(-pos); 
	vec3 R2 = normalize(reflect(-L2,N));
	float factor2 = clamp(dot(R2,V2),0.0,1.0); 
	vec3 specular2 = fdir*fdist2*Is2*Ks*pow(factor2,alpha);
	c += clamp(specular2, 0.0, 1.0);

	//Luz emisiva:
	c += Ke;
	
	return c;
}
#version 330 core

out vec4 outColor;

in vec2 texCoord; //Atributo de UVs en v�rtices de objeto
in vec3 pos;
in vec3 norm; //normal en espacio de c�mara/vista

uniform sampler2D colorTex;
uniform sampler2D emiTex;
uniform sampler2D specularTex;

//propiedades de la luz
vec3 Ia = vec3(0.5); //luz ambiente

//Primera luz: 
vec3 Id1 = vec3(30.0); //luz difusa
vec3 Is1 = vec3(30.0); //luz especular
vec3 lpos1 = vec3(3.0, 0.0, 0.0); 
float d1 = distance(lpos1, pos);
float fdist1 = pow(1/d1, 2); //Atenuaci�n dependiendo de la distancia

//Segunda luz:
vec3 Id2 = vec3(30.0);
vec3 Is2 = vec3(30.0);
vec3 lpos2 = vec3(-3.0, 0.0, 0.0);
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
	Ks = texture(specularTex, texCoord).rgb; //Todos usan las mismas UVs, por lo que tienen las mismas coordenadas

	N = normalize(norm);
	outColor = vec4(shade(),1.0);
}

vec3 shade()
{
	//Luz ambiental:
	vec3 c = vec3(0.0); //color negro por defecto
	c += Ia*Ka; //El color depende solamente del color ambiente (Ka) y del valor de la luz ambiente (Ia)
	
	//Primera luz:
	vec3 L1 = normalize(lpos1-pos); //direcci�n de la luz, desde el punto hacia la luz
	vec3 diffuse1 = fdist1*Id1*Kd*dot(L1,N); //Esto da la componente difusa 
	c += clamp(diffuse1, 0.0, 1.0); //Componente ambiental + el valor difuso acotado entre 0 y 1 (para que no se queme a blanco o los valores no sean negativos)

	vec3 V1 = normalize(-pos); //Estamos en coordenadas de vista, desde la c�mara, por lo que se pone 0-pos = -pos
	vec3 R1 = normalize(reflect(-L1,N)); // R es el reflejo de la luz respecto a la normal del objeto (reflexi�n total)
	//Ahora nos fijamos en el �ngulo formado entre la V y la R, cuanto m�s cercana sea la V a la R m�s reflejo se ver�
	float factor1 = clamp(dot(R1,V1),0.0,1.0); //Valdr� 1 cuando est� alineado perfectamente y 0 cuando est�n a 90 grados
	vec3 specular1 = fdist1*Is1*Ks*pow(factor1,alpha); //pow es ^ en GLSL
	c += clamp(specular1, 0.0, 1.0);

	//Segunda luz:
	vec3 L2 = normalize(lpos2-pos); 
	vec3 diffuse2 = fdist2*Id2*Kd*dot(L2,N); 
	c += clamp(diffuse2, 0.0, 1.0); 

	vec3 V2 = normalize(-pos); 
	vec3 R2 = normalize(reflect(-L2,N)); 
	float factor2 = clamp(dot(R2,V2),0.0,1.0); 
	vec3 specular2 = fdist2*Is2*Ks*pow(factor2,alpha); 
	c += clamp(specular2, 0.0, 1.0);

	//Luz emisiva:
	c += Ke; //Como es emisiva s�lo se suma su energ�a.
	
	return c;
}
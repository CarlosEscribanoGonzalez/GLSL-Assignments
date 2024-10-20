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
vec3 Id = vec3(1.0); //luz difusa
vec3 Is = vec3(1.0); //luz especular
vec3 lpos = vec3(3.0); //posici�n de la luz (en este caso en el 3, 3, 3) en el espacio de vista (no var�an)

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
	vec3 c = vec3(0.0); //color negro por defecto
	c = Ia*Ka; //El color depende solamente del color ambiente (Ka) y del valor de la luz ambiente (Ia)

	vec3 L = normalize(lpos-pos); //direcci�n de la luz, desde el punto hacia la luz
	vec3 diffuse = Id*Kd*dot(L,N); //Esto da la componente difusa 
	c += clamp(diffuse, 0.0, 1.0); //Componente ambiental + el valor difuso acotado entre 0 y 1 (para que no se queme a blanco o los valores no sean negativos)

	vec3 V = normalize(-pos); //Estamos en coordenadas de vista, desde la c�mara, por lo que se pone 0-pos = -pos
	vec3 R = normalize(reflect(-L,N)); // R es el reflejo de la luz respecto a la normal del objeto (reflexi�n total)
	//Ahora nos fijamos en el �ngulo formado entre la V y la R, cuanto m�s cercana sea la V a la R m�s reflejo se ver�
	float factor = clamp(dot(R,V),0.0,1.0); //Valdr� 1 cuando est� alineado perfectamente y 0 cuando est�n a 90 grados
	vec3 specular = Is*Ks*pow(factor,alpha); //pow es ^ en GLSL
	c += clamp(specular, 0.0, 1.0);

	c += Ke; //Como es emisiva s�lo se suma su energ�a.
	
	return c;
}
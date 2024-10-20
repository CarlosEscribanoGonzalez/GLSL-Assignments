#version 330 core

in vec3 inPos;
uniform mat4 modelViewProj; //La librería las ha multiplicado ya. Vienen premultiplicadas. Más eficiente.
out vec3 color;


void main()
{
	if(mod(gl_VertexID, 4.0) == 0.0){ //Hace el módulo entre el ID de la primitiva y 4. El resulado puede ser 0, 1, 2 o 3.
		color = vec3(1.0,0.0,0.0);  
	} else if(mod(gl_VertexID, 4.0) == 1.0){
		color = vec3(0.0,1.0,0.0);
	} else if(mod(gl_VertexID, 4.0) == 2.0){
		color = vec3(0.0,0.0,1.0);
	} else {
		color = vec3(0.5,0.0,0.5); //Morado
	}
	
	gl_Position = modelViewProj*vec4(inPos,1);
}

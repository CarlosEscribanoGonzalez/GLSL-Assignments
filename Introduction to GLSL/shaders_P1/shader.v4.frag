#version 330 core

out vec4 outColor;


void main()
{

	if(mod(gl_PrimitiveID, 4.0) == 0.0){ //Hace el módulo entre el ID de la primitiva y 4. El resulado puede ser 0, 1, 2 o 3.
		outColor = vec4(1.0,0.0,0.0,1.0);  
	} else if(mod(gl_PrimitiveID, 4.0) == 1.0){
		outColor = vec4(0.0,1.0,0.0,1.0);
	} else if(mod(gl_PrimitiveID, 4.0) == 2.0){
		outColor = vec4(0.0,0.0,1.0,1.0);
	} else {
		outColor = vec4(0.5,0.0,0.5,1.0); //Morado
	}
	 
}

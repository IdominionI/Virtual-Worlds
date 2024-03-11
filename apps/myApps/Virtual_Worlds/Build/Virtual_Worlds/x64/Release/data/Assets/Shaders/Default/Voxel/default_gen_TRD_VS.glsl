//#version 450
//#version 450 core
#version 330 core

// -------------- Shader Reserved Uniforms -------------------

//#include "shader_basis_code/Universal_reserved_uniforms.glsl"

#include "shader_basis_code/vs_reserved_uniforms.glsl"


// -------------- User Defined Uniforms ----------------------

out vec3 positionFromVertex;

uniform int display_as_points;
vec4 raw_color;

// -------------Application dynamicly defined uniorms---------
// Do not delete next line with DDU as application defined uniforms are placed here
// Must exist in every glsl code unless user wishes to manually enter uniforms that
// the application generates.
// #DD#

// -------------- Shader Reserved functions ----------------

// -------------- User Defined Functions -------------------

void main(){
	//value = int(position.w);              // required as a min
	
	// following required as a min
	if(display_as_points == 0){ // do not display as points
		gl_Position = position; // required as a min
	}else{// diaplay as points
		gl_Position = modelViewProjectionMatrix*position; // required as a min
	}
	
	raw_color = vec4(1.0,1.0,0.0,1.0);

	vs_out.varyingColor = raw_color;

}

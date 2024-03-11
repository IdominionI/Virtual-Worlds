#version 450 core

// -------------- Shader Reserved Uniforms -------------------

//#include "shader_basis_code/universal_reserved_uniforms.glsl"

#include "shader_basis_code/vs_reserved_uniforms.glsl"

// -------------- User Defined Uniforms ----------------------

uniform int display_as_points;
vec4 raw_color;

// -------------Application dynamicly defined uniorms---------
// Do not delete next line with DDU as applicatioin defined uniforms are placed here
// Must exist in every glsl code unless user whishes to manually enter uniforms that
// the application generates.
// #DD#

// -------------- Shader Reserved funtions ----------------

// -------------- User Defined Functions -------------------


void main(){

	//gl_Position = vec4(vertex.x,vertex.y,vertex.z,1.0) ; // required as a min
	
		// following required as a min
	if(display_as_points == 0){ // do not display as points
		gl_Position = position; // required as a min
		//gl_Position = vec4(position.x,position.y,position.z,1.0); // required as a min
		//vs_out.varyingColor = vec4(1.0,0.0,0.0,1.0);
	}else{// diaplay as points
		gl_Position = modelViewProjectionMatrix*position; // required as a min
		//vs_out.varyingColor = vec4(0.0,1.0,0.0,1.0);
	}
	
	//gl_Position = modelViewProjectionMatrix*position;

	raw_color = vec4(1.0,1.0,1.0,1.0); // required as a min

	vs_out.varyingColor = raw_color;
}
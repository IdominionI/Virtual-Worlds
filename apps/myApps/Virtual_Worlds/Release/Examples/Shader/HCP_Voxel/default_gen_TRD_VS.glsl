//#version 450
//#version 450 core
#version 450 core

// -------------- Shader Reserved Uniforms -------------------

//#include "shader_basis_code/Universal_reserved_uniforms.glsl"

#include "shader_basis_code/vs_reserved_uniforms.glsl"


// -------------- User Defined Uniforms ----------------------

out vec3 positionFromVertex;

// -------------Application dynamicly defined uniorms---------
// Do not delete next line with DDU as application defined uniforms are placed here
// Must exist in every glsl code unless user wishes to manually enter uniforms that
// the application generates.
// #DD#

// -------------- Shader Reserved functions ----------------

// -------------- User Defined Functions -------------------

void main(){
	positionFromVertex = position.xyz;
	
	gl_Position = position;        // required as a min

	//value = int(position.w);              // required as a min

	vs_out.varyingColor = vec4(1.0,1.0,0.0,1.0);

}

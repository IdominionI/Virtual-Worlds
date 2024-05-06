//#version 450
//#version 450 core
#version 450 core

// -------------- Shader Reserved Uniforms -------------------

//#include "shader_basis_code/fs_reserved_uniforms.glsl"
out vec4 FragColor; // output color

// Cannot use same variable name for in and out variables
// use Input and output interface blocks to solve this
// The following input block is to allow  varyingColor to be
// either a vertex or geometry shader

in VertexData
{
  vec4 varyingColor;
} fs_in;

// -------------- User Defined Uniforms ----------------------

// -------------Application dynamicly defined uniorms---------
// Do not delete next line with DDU as applicatioin defined uniforms are placed here
// Must exist in every glsl code unless user whishes to manually enter uniforms that
// the application generates.
// #DD#

// -------------- Shader Reserved funtions ----------------

// -------------- User Defined Functions -------------------

void main(){
   FragColor = fs_in.varyingColor;
   //FragColor = vec4(1.0f,1.0f,.5f,1.0f);
}

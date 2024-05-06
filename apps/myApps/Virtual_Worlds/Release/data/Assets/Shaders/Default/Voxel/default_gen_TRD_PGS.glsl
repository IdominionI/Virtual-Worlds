//#version 450
//#version 450 core
#version 330 core

// -------------- Shader Reserved Uniforms -------------------
layout(points) in;
layout(points, max_vertices = 1) out;    // This required to display a voxel center of 1 point

uniform mat4 modelViewProjectionMatrix;

in VertexData
{
  vec4 varyingColor;
} gs_in[]; // Must have as an array otherwise will not work as one for each input vertex,

out VertexData
{
  vec4 varyingColor;
} gs_out;

// -------------- User Defined Uniforms ----------------------

// -------------Application dynamicly defined uniorms---------
// Do not delete next line with DDU as applicatioin defined uniforms are placed here
// Must exist in every glsl code unless user whishes to manually enter uniforms that
// the application generates.
// #DD#

// -------------- Shader Reserved funtions ----------------

// -------------- User Defined Functions -------------------

void main(){
	gs_out.varyingColor = gs_in[0].varyingColor;
	//gs_out.varyingColor = vec4(0.0,1.0,1.0,1.0);

    //gl_Position = modelViewProjectionMatrix*vec4(center,1.0); 
    gl_Position =  modelViewProjectionMatrix*gl_in[0].gl_Position;

    EmitVertex();
    //EndPrimitive();

}

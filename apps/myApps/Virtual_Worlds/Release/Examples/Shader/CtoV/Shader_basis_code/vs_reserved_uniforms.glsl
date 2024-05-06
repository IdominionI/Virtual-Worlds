uniform mat4 modelViewProjectionMatrix;  // ++++++

in vec4 position; // ++++++


// //in vec4 vertex;        //vertex that the shader is processing
// //in vec3 normal;        //vertex that the shader is processing

// layout(location = 0) in vec3 vertex; //******
// layout(location = 1) in vec3 normal; //******

// #include "universal_reserved_uniforms.glsl"

// // hex surface parameter data
// uniform vec2  grid_origin;

// //in float hex_size;
// //flat out float hexSize; 
// uniform float hexSize; 

// //hex surface display data
// uniform float hex_min_surface_display_value;
// uniform float hex_max_surface_display_value;

// vec4 raw_color;

// Cannot use same variable name for in and out variables 
// use Input and output interface blocks to solve this
// The following output block is to allow  varyingColor to be passed to either 
// a geometry shader to be psssed on to the fragment shader, or to the fragemt alone
out VertexData
{
//  float  value;
  vec4 varyingColor;
} vs_out;

/*
// ******* BEGIN TESTING PARAMETER TRANSFER
in int shader_parameters_frame;

//uniform vec2  origin;
uniform vec2  resolution;

uniform int display_as_points;
 //color of vertex that shader is processing
uniform vec4  color;			       
uniform int  display_native_vertex_colors;
*/
// ************************************************************
/*
layout(location = 0) in vec4 vertex;

out int value;

uniform mat4 mvpMatrix; //model-view-projection matrix

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;


// Camera data
uniform vec3  camera_loc;         // Camera location

uniform vec3  camera_front_vector;
uniform vec3  camera_up_vector;
uniform vec3  camera_right_vector;


// reserved light variables
uniform vec4  light_color;        // ****
uniform vec3  lighting_direction; // ****
uniform float ambience;           //****  change shaders ambient_light to ambience
uniform float specular_strength;  //****
uniform float lighting_intensity;    //****


// reserved camera light variables
uniform int   camera_lighting_type;   //**** camera_lighting_type - done
uniform int   use_camera_lighting;     //**** chnage to use_camera_lighting -done
uniform vec3  lighting_camera_offset; //**** lighting_camera_offset -done

uniform int   t_frame;

// NEW ONES TO BE ADDED
//in float voxel_size;	
//flat out float voxSize;
// change  above to 
 
uniform float voxSize;

// reserved Voxel parameter data
uniform ivec3 voxel_matrix_dimension;
uniform vec3  voxel_origin;
uniform float voxel_hcp_z_increment;

//voxel surface display data
uniform float voxel_min_surface_display_value;
uniform float voxel_max_surface_display_value;

in int vEnabledFaces;
flat out int gEnabledFaces;


#define TRUE  0
#define FALSE 1

// Cannot use same variable name for in and out variables 
// use Input and output interface blocks to solve this
// The following output block is to allow  varyingColor to be passed to either 
// a geometry shader to be psssed on to the fragment shader, or to the fragemt alone
out VertexData
{
 vec4 varyingColor;
} vs_out;

//out vec4 fvaryingColor;

vec4 raw_color;
*/
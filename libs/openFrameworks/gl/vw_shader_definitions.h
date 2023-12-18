#pragma once

//#include <gl/ofShader.h>
//#include <types/ofColor.h>

enum class shader_uniform_data_type_enum {
	undefined,
	udt_int, udt_uint, udt_float, udt_double,		//single valued 
	udt_int2, udt_int3, udt_int4,						// 2,3,4 parameter integer
	udt_float2, udt_float3, udt_float4,				// 2,3,4 parameter float
	udt_vec2f, udt_vec3f, udt_vec4f,					// vector floats
	udt_vec2i, udt_vec3i, udt_vec4i,					// vector integers
	udt_array1i, udt_array2i, udt_array3i, udt_array4i, // 1,2,3,4 array of integers 
	udt_array1f, udt_array2f, udt_array3f, udt_array4f
};// 1,2,3,4 array of floats

enum class shader_attribute_data_type_enum {
	undefined,
	adt_int1, adt_int2, adt_int3, adt_int4,					//1, 2,3,4 parameter integer
	adt_float1, adt_float2, adt_float3, adt_float4,		//1, 2,3,4 parameter float
	adt_double1, adt_double2, adt_double3, adt_double4,	//1, 2,3,4 parameter double
	adt_array1f, adt_array2f, adt_array3f, adt_array4f
};	// 1,2,3,4 array of floats

enum class shader_type_enum { vertex, geometry, fragment, compute };
enum class shader_assignment_enum { all, vertex, geometry, fragment, vertex_geometry, vertex_fragment, geometry_fragment };

enum class basic_shader_function_type_enum { color, no_texture, texture_2D, texture_rect, texture_2D_color, texture_rect_color, custom };

enum class geometry_shader_input_type_enum { gl_points, gl_lines, gl_lines_adjacency_ext, gl_triangles, gl_triangles_adjacency_ext };// these are for openframeworks
enum class geometry_shader_output_type_enum { gl_points, gl_line_strip, gl_triangle_strip };

//enum class geometry_shader_input_type_enum { gl_points, 
//											   gl_lines, gl_lines_adjacency, gl_line_strip_adjacency, 
//											   gl_triangles,gl_triangle_strip, gl_triangle_fan,
//											   gl_triangles_adjacency, gl_triangle_strip_adjacency};// more thorough opengl


/*
static const string default_basic_vertex_PBR_defines = R"(

)";

static const string default_basic_vertex_inputs = R"(
in vec4 position;
in vec4 color;
in vec4 normal;
in vec2 texcoord;)";

static const string default_basic_vertex_outputs = R"(
out vec2 v_texcoord; // phong pass the texCoord if needed
out vec3 v_transformedNormal;
out vec3 v_normal;
out vec3 v_eyePosition;
out vec3 v_worldPosition;
out vec3 v_worldNormal;
#if HAS_COLOR
out vec4 v_color;
#endif)";

static const string default_basic_vertex_uniforms = R"(
// these are passed in from OF programmable renderer
uniform mat4 modelViewMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat4 normalMatrix;

uniform vec2 mat_texcoord_scale;)";
*/
//static const string default_basic_vertex_uniforms2 = R"(
//uniform int number_lights;
//uniform int HAS_TEXTURE;
//uniform int HAS_COLOR;)";
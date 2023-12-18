#pragma once

#include <app/ofAppBaseWindow.h>
#include <gl/ofShader.h>
#include <gl/ofMaterial.h>

#include <gl/ofGLProgrammableRenderer.h>

#include <FrameWork/VW_framework/Types/vw_type_definitions.h>

struct shader_default_variable_struct_type {

// ********** of Shader names ****************
//Camera matrix4f  uniform names modelview
	std::string model_matrix_uniform                = "uniform mat4 modelmatrix;\n";
	std::string view_matrix_uniform                 = "uniform mat4 viewmatrix;\n";
	std::string modelview_matrix_uniform            = "uniform mat4 modelviewmatrix\n";
//camera matrix4f  uniform names modelview+prjection
	std::string projection_matrix_uniform           = "uniform mat4 projectionmatrix;\n";
	std::string modelview_projection_matrix_uniform = "uniform mat4 modelviewprojectionmatrix;\n";

// texture matrix4f matrix uniform
	std::string texture_matrix_uniform  = "uniform mat4 texturematrix;\n";
// color 4f uniform 
	std::string color_uniform           = "uniform mat4 globalcolor;\n";

// boolean 1f unforms for shader if statements
	std::string use_texture_uniform    = "uniform float usingtexture;\n";
	std::string use_colors_uniform     = "uniform float usingcolors;\n";
	std::string bitmap_string_uniform  = "uniform float bitmaptext;\n";

	// vertex 
	std::string vertex_position_data = "IN vec4 position;\n";
	std::string vertex_color_data    = "IN vec4 color;\n";
	std::string vertex_normal_data   = "IN vec4 normal;\n";

	// image texture coordinate data
	std::string texture_coord_data = "IN vec2 texcoord;\n";

	/* fragment shader vars
	uniform sampler2DRect src_tex_unit0;
	uniform sampler2DRect src_tex_unit1;

	IN float depth;
	IN vec4 colorVarying;
	IN vec2 texCoordVarying;
	*/

	// ofMaterial 
	// Not sure thses are used
	std::string of_material_texture_diffuse						  =  "tex0";
	std::string of_material_texture_specular					  =  "tex_specular";
	std::string of_material_texture_ambient						  =  "tex_ambient";
	std::string of_material_texture_emissive					  =  "tex_emissive";
	std::string of_material_texture_normal						  =  "tex_normal";
	std::string of_material_texture_occlusion                     =  "tex_occlusion";
	std::string of_material_texture_ao_roughness_metallic         =  "tex_ao_roughness_metallic";
	std::string of_material_texture_roughness_metallic            =  "tex_roughness_metallic";
	std::string of_material_texture_roughness                     =  "tex_roughness";
	std::string of_material_texture_metallic                      =  "tex_metallic";
	std::string of_material_texture_displacement                  =  "tex_displacement";
	std::string of_material_texture_clearcoat                     =  "tex_clearcoat_intensity";
	std::string of_material_texture_clearcoat_roughness           =  "tex_clearcoat_roughness";
	std::string of_material_texture_clearcoat_intensity_roughness =  "tex_clearcoat_intensity_roughness";
	std::string of_material_texture_clearcoat_normal			  =  "tex_clearcoat_normal";

	// material parameters
	std::string CurrentEyePosition						  = "uniform vec3 uCameraPos;\n";

	std::string of_material_diffuse						  = "uniform vec4  mat_diffuse;\n";
	std::string of_material_emissive					  = "uniform vec4  mat_emissive;\n";
	std::string of_material_ambient						  = "uniform vec4  mat_ambient;\n";
	std::string of_material_specular					  = "uniform vec4  mat_specular;\n";
	std::string of_material_shininess					  = "uniform float mat_shininess;\n";

	std::string of_material_roughness					  = "uniform float mat_roughness;\n";
	std::string of_material_metallic					  = "uniform float mat_metallic;\n";
	std::string of_material_reflectance					  = "uniform float mat_reflectance;\n";
	std::string of_material_displacement_strength		  = "uniform float mat_displacement_strength;\n";
	std::string of_material_displacement_normals_strength = "uniform float mat_displacement_normals_strength;\n";
	std::string normalGeomToNormalMapMix				  = "uniform float mat_normal_mix;\n";

	std::string of_material_ibl_exposure				  = "uniform float mat_ibl_exposure;\n";
	std::string of_material_clearcoat					  = "uniform vec2  mat_clearcoat;\n";
	std::string of_material_texCoordScale				  = "uniform vec2  mat_texcoord_scale\n";

	std::string of_material_uCubeMapEnabled				  = "uniform float uCubeMapEnabled;\n";
	std::string of_material_maxMipLevels				  = "uniform float uEnvMapMaxMips;\n";

};

struct geometry_shader_definition_struct_type {

	geometry_shader_input_type_enum  input_type  = geometry_shader_input_type_enum::gl_points;  // type: GL_POINTS, GL_LINES, GL_LINES_ADJACENCY_EXT, GL_TRIANGLES, GL_TRIANGLES_ADJACENCY_EXT
	geometry_shader_output_type_enum output_type = geometry_shader_output_type_enum::gl_points; // type: GL_POINTS, GL_LINE_STRIP or GL_TRIANGLE_STRIP
	size_t                           number_output_vertices = 1;                                // set number of output vertices

	GLenum get_opengl_geometry_shader_input_type() {
		switch (input_type) {
			case geometry_shader_input_type_enum::gl_points :                 return GL_POINTS; break;
			case geometry_shader_input_type_enum::gl_lines:                   return GL_LINES; break;
			case geometry_shader_input_type_enum::gl_lines_adjacency_ext:     return GL_LINES_ADJACENCY_EXT; break;
			case geometry_shader_input_type_enum::gl_triangles:               return GL_TRIANGLES; break;
			case geometry_shader_input_type_enum::gl_triangles_adjacency_ext: return GL_TRIANGLES_ADJACENCY_EXT; break;
			default : return GL_INVALID_ENUM;
		}
		return GL_INVALID_ENUM;
	}

	GLenum get_opengl_geometry_shader_output_type() {
		switch (output_type) {
			case geometry_shader_output_type_enum::gl_points :        return GL_POINTS; break;
			case geometry_shader_output_type_enum::gl_line_strip:     return GL_LINE_STRIP; break;
			case geometry_shader_output_type_enum::gl_triangle_strip: return GL_TRIANGLE_STRIP; break;
			default: return GL_INVALID_ENUM;
		}
		return GL_INVALID_ENUM;
	}

	std::string get_geometry_shader_input_type_string() {
		switch (input_type) {
			case geometry_shader_input_type_enum::gl_points :                 return "layout(points) in;\n"; break;
			case geometry_shader_input_type_enum::gl_lines:                   return "layout(lines) in;\n"; break;
			case geometry_shader_input_type_enum::gl_lines_adjacency_ext:     return "layout(lines_adjacency) in;\n"; break;
			case geometry_shader_input_type_enum::gl_triangles:               return "layout(triangles) in;\n"; break;
			case geometry_shader_input_type_enum::gl_triangles_adjacency_ext: return "layout(triangles_adjacency) in;\n"; break;
			default: return "";
		}
		return "";
	}

	std::string get_geometry_shader_output_type_string() {
		switch (output_type) {
		case geometry_shader_output_type_enum::gl_points:         return "layout(points, max_vertices = " + std::to_string(number_output_vertices) + ") out;\n"; break;
		case geometry_shader_output_type_enum::gl_line_strip:     return "layout(line_strip, max_vertices = " + std::to_string(number_output_vertices) + ") out;\n"; break;
		case geometry_shader_output_type_enum::gl_triangle_strip: return "layout(triangle_strip, max_vertices = " + std::to_string(number_output_vertices) + ") out;\n"; break;
		default: return "";
		}
		return "";
	}


};


class shader_contruction_class : public ofShader {
//class shader_contruction_class : public ofMaterial {
public:

	shader_contruction_class() { }
	~shader_contruction_class() {}

	string_type shader_name        = "";
	string_type shader_description = "";

	std::string vertex_shader_source_code   = "";
	std::string geometry_shader_source_code = "";
	std::string fragment_shader_source_code = "";


	bool is_PBR_shader_material() {	return material_shader.isPBR();	}

	bool define_basic_default_shader(basic_shader_function_type_enum basic_shader_function,ofGLProgrammableRenderer *renderer, bool set_as_pbr_shader = false) {
		if (renderer == NULL) {
			// error message
			return false;
		}

		if (renderer->getGLVersionMajor() < 4) {
			// error message
			return false;
		}
		
		material_shader.currentRenderShader = this;
		material_shader.setPBR(set_as_pbr_shader);

		//if( material_shader.define_basic_shader(basic_shader_function,window, this)){
		if( material_shader.define_basic_shader(basic_shader_function, renderer, this)){
			// message creation success;
cout << "define_basic_default_shader true : \n";
//cout << std::endl << getShaderSource(GL_VERTEX_SHADER) << std::endl;
//cout << "Fragment SHADER  : " << std::endl << "-----------------------" << std::endl << getShaderSource(GL_FRAGMENT_SHADER) << std::endl;
//vertex_shader_source_code   = getShaderSource(GL_VERTEX_SHADER);
//fragment_shader_source_code = getShaderSource(GL_FRAGMENT_SHADER);
			return true;
		} else{
			// message creation failed;
cout << "define_basic_default_shader false : "  << std::endl;
			return false;
		}
	}

	
	//
	bool define_basic_vertex_source_code(basic_shader_function_type_enum basic_shader_function, ofGLProgrammableRenderer *renderer, bool set_as_pbr_shader = false) {
		if (ofGLPRenderer == NULL) {
			//error message
cout << "define_basic_vertex_source_code : ofGLPRenderer == NULL" << std::endl;
			return false;
		}

		if (renderer->getGLVersionMajor() < 4) {
			// error message
			return false;
		}

cout << "define_basic_vertex_source_code : " << renderer->getGLVersionMajor() << " : " << renderer->getGLVersionMajor() << std::endl;

		vertex_shader_source_code = material_shader.basic_shader_source_code(shader_type_enum::vertex, basic_shader_function, renderer, set_as_pbr_shader);

cout << "define_basic_vertex_source_code source :\n" << vertex_shader_source_code;

		return true;
	}
	
	
	bool define_basic_geometry_source_code(geometry_shader_definition_struct_type geometry_shader_definition, ofGLProgrammableRenderer *renderer) {
		if (ofGLPRenderer == NULL) {
			//error message
			cout << "define_basic_geometry_source_code : ofGLPRenderer == NULL" << std::endl;
			return false;
		}

		int major = renderer->getGLVersionMajor();
		int minor = renderer->getGLVersionMinor();

		if (renderer->getGLVersionMajor() < 4) {
			// error message
			return false;
		}

		std::string source = "";

		source += "#version " + std::to_string(major) + std::to_string(minor) + "0 core\n";
		source += "\n";
		source += geometry_shader_definition.get_geometry_shader_input_type_string();
		source += geometry_shader_definition.get_geometry_shader_output_type_string();
		source += "\n";
		source += "//%CUSTOM DEFINES\n";
		source += "\n";
		source += "//%CUSTOM ATTRIBUTES\n";
		source += "\n";
		source += "uniform mat4    modelViewProjectionMatrix;\n";// not sure this should be madatory for all geometry shaders
		source += "\n";
		source += "//%CUSTOM UNIFORMS\n";
		source += "\n";
		source += "//%CUSTOM FUNCTIONS\n";
		source += "\n";
		source += "void main() {\n";
		source += "//gl_Position = modelViewProjectionMatrix * gl_in[].gl_Position;\n";// uncomment ????
		source += "//EmitVertex();\n";												   // uncomment ????
		source += "\n";
		source += "//%CUSTOM MAIN SOURCE CODE%\n";
		source += "\n";
		source += "}\n";

		std::string geometry_shader_source_code = source;

cout << "define_basic_fragment_source_code source :\n" << geometry_shader_source_code;
		return true;
	}
	
	bool define_basic_fragment_source_code(basic_shader_function_type_enum basic_shader_function, ofGLProgrammableRenderer* renderer, bool set_as_pbr_shader = false) {
		if (ofGLPRenderer == NULL) {
			//error message
			cout << "define_basic_fragment_source_code : ofGLPRenderer == NULL" << std::endl;
			return false;
		}

		if (renderer->getGLVersionMajor() < 4) {
			// error message
			return false;
		}

cout << "define_basic_fragment_source_code : " << renderer->getGLVersionMajor() << " : " << renderer->getGLVersionMajor() << std::endl;

		fragment_shader_source_code = material_shader.basic_shader_source_code(shader_type_enum::fragment, basic_shader_function, renderer, set_as_pbr_shader);

cout << "define_basic_fragment_source_code source :\n" << fragment_shader_source_code;

		return true;
	}

	bool add_custom_uniform(shader_assignment_enum shader_assignment, std::string uniform_name, shader_uniform_data_type_enum uniform_data_type) { return false; }
	bool add_custom_attribute(shader_assignment_enum shader_assignment, std::string attribute_name, shader_attribute_data_type_enum attribute_data_type) { return false; }

	bool add_custom_source_code_functions(shader_type_enum shader_type, std::string source_code) { return false; }
	bool add_custom_main_source_code_fragment(shader_type_enum shader_type, std::string source_code) { return false; }

	std::string get_resultant_source_code(shader_type_enum shader_type) { return ""; };

	//material_shader_class material_shader;
	ofMaterial material_shader;

	ofGLProgrammableRenderer *ofGLPRenderer;

	//ofShader *shader;

protected:

private:


};
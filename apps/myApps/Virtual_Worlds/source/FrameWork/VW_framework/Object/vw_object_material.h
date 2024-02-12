#pragma once

#include <glm/glm.hpp>

#include "../Shader/shader.h"

// No textures are to be used as virtual world objects are proceduaraly
// genereated or involve celular automata not suitable for texture mapping
// Othewise would have used ofMaterial 

struct vw_object_material_struct_type {

	glm::vec4 mat_color        = {0.5f,0.5f,0.5f,1.0};
	//float     mat_transparency = 0.0f;  // the alpha channel of the mat_color

	// Naming convention kept same as used in
	// ofFrameworks for consistancy of any
	// shader code or addons to be integrated
	// These names are also the glsl uniform 
	// naming convention

	// Color vectors 
	glm::vec4 mat_diffuse_color  = {0.5f,0.5f,0.5f,1.0};
	glm::vec4 mat_ambient_color  = {0.5f,0.5f,0.5f,1.0};
	glm::vec4 mat_specular_color = {0.5f,0.5f,0.5f,1.0};
	glm::vec4 mat_emissive_color = {0.5f,0.5f,0.5f,1.0};
	
	// surface properties
	float mat_shinines    = 0.5f; // specular ??
	float mat_metallic    = 0.5f;
	float mat_roughness   = 0.5f; // difuse ??
	float mat_reflectance = 0.5f;
	
	//float diffuse  = 0.5f;
	//float specular = 0.5f;


	// not sure need these
	//glm::vec2 mat_clearcoat_strength;
	//glm::vec2 mat_clearcoat_roughness;

	//float	  mat_displacement_strength;
	//float	  mat_displacement_normals_strength;
	//float	  mat_normal_mix;

	void set_color(glm::vec4 color) {
		mat_color = color;
	}

	void set_transparency(float transparency) {
		mat_color.w = 1.0f-transparency;
	}

	// add others if necessary

	void update_shader_uniforms(GLint shader_id) {
		//std::cout << "vw_camera_base::update_shader_uniforms : shader_id 0000 " << shader_id << std::endl;

		if (shader_id < 0) {
			std::cout << "vw_camera_base::update_shader_uniforms : shader_id < 0 " << shader_id << std::endl;
			return;
		}

		shader_class shader;

		shader.set_vec4(shader_id, mat_color,         "mat_color");
		shader.set_vec4(shader_id, mat_diffuse_color, "mat_diffuse");
		shader.set_vec4(shader_id, mat_ambient_color, "mat_ambient");
		shader.set_vec4(shader_id, mat_specular_color,"mat_specular");
		shader.set_vec4(shader_id, mat_emissive_color,"mat_emissive");

		shader.set_f1(shader_id, mat_shinines,   "mat_shinines");
		shader.set_f1(shader_id, mat_metallic,   "mat_metallic");
		shader.set_f1(shader_id, mat_roughness,  "mat_roughness");
		shader.set_f1(shader_id, mat_reflectance,"mat_reflectance");

		// If need other camera paramters to be used by shader glsl code add below 

	//std::cout << "vw_camera_base::update_shader_uniforms 1111 : " << renderer->camera_forward_dir.x << " : " << renderer->camera_forward_dir.y << " : " << renderer->camera_forward_dir.z << " : " << std::endl;

	}

	void display_object_material() {

	}

	void save_object_material() {

	}

	void import_object_material() {

	}
};

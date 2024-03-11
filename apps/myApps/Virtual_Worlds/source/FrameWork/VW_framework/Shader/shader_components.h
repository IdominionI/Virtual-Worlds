#pragma once

#include "shader_format.h"
#include "shader_manager.h"

struct material_base_struct_type : public shader_format_class {
	shader_db_manager_class shader_manager; // Need to substitute this with shader_class !!!!!!!!!!!

	virtual void define_shader_uniforms() {};
	virtual bool update_shader_variables_value_increment(int frame_increment) { return false; };
	virtual bool update_shader_variables() { return false; };

};

struct material_struct_type : public material_base_struct_type {
	~material_struct_type() { //*****
		variables.clear();
		variables.shrink_to_fit();
		int_variables.clear();
		int_variables.shrink_to_fit();
		bool_variables.clear();
		bool_variables.shrink_to_fit();
	}

	shader_display_parameters_struct_type shader_display_parameters;

	std::vector<shader_parameter_variable_struct_type>      variables;
	std::vector<shader_parameter_int_variable_struct_type>  int_variables;
	std::vector<shader_parameter_bool_variable_struct_type> bool_variables;
	//other shader uniform datatypes to be defined here

	void define_shader_uniforms() {
		clear_uniforms();

		for (shader_parameter_variable_struct_type float_variable : variables) {
			if (!uniform_exists(float_variable.variable_name) && float_variable.active_variable) add_uniform(float_variable.variable_name, application_default_shader_variable_type_enum::Float1);
		}

		for (shader_parameter_int_variable_struct_type int_variable : int_variables) {
			if (!uniform_exists(int_variable.variable_name) && int_variable.active_variable) add_uniform(int_variable.variable_name, application_default_shader_variable_type_enum::Int1);
		}

		for (shader_parameter_bool_variable_struct_type bool_variable : bool_variables) {
			if (!uniform_exists(bool_variable.variable_name) && bool_variable.active_variable) add_uniform(bool_variable.variable_name, application_default_shader_variable_type_enum::Bool);
		}

		//other shader uniform datatypes to be added here
	}

	bool update_shader_variables_value_increment(int frame_increment) {
		if (!shader_program_id) return false;
//printf("material_struct_type : update_shader_variables 1111 : \n");
		for (shader_parameter_variable_struct_type variable : variables) {
			if (variable.active_variable && variable.active_variable_step) {
				variable.value += variable.variable_step * float(frame_increment);
			}
		}

		for (shader_parameter_int_variable_struct_type int_variable : int_variables) {
			if (int_variable.active_variable && int_variable.active_variable_step) {
				int_variable.value += int_variable.variable_step * frame_increment;
			}
		}

		return true;
	}

	bool update_shader_variables() {
//printf("material_struct_type : update_shader_variables 0000 : %i \n", shader_program_id);

		if (!shader_program_id) return false;
//printf("material_struct_type : update_shader_variables 1111 : \n");
		for (shader_parameter_variable_struct_type variable : variables) {
			shader_manager.shader.set_f1(shader_program_id, variable.value, variable.variable_name);
		}

		for (shader_parameter_int_variable_struct_type int_variable : int_variables) {
			shader_manager.shader.set_i1(shader_program_id, int_variable.value, int_variable.variable_name);
		}

		for (shader_parameter_bool_variable_struct_type bool_variable : bool_variables) {
			shader_manager.shader.set_b1(shader_program_id, bool_variable.value, bool_variable.variable_name);
		}


		// This method of storing shader uniform variables as an array has been  problematic and is not used :
		// potential for future use ???????
/*		for (application_default_shader_uniform_variables_struct_type uniform_variable : default_shader_uniform_variables) {
if(uniform_variable.type == application_default_shader_variable_type_enum::Float1) printf("update_shader_variables :: uniform_variable type Float1 \n");
printf("update_shader_variables :: uniform_variable %s : %f : \n", uniform_variable.name.c_str(), *(float*)uniform_variable.value0);
			update_shader_variable(uniform_variable);
		}
*/
		return true;
	}
};
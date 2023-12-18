#pragma once

#include <VW_framework/Scene/scene_object_categories.h>

#include <VW/Editor/Shader/shader_import_export.h>

#include "../../Object/voxel_hcp_object.h"
#include "../../Kernal/voxel_function_import_export.h"
#include "../../Automata/automata_byte_import_export.h"

class voxel_hcp_scene_objects_class : public vw_scene_objects_category_class{

	voxel_function_import_export_class      voxel_function_import_export;
	voxel_texture_import_export_class       voxel_texture_import_export;
	import_export_byte_automata_rules_class import_export_byte_automata_rules;

public:



	// ++++++++++++++++++++++++ Voxel generation data ++++++++++++++++++++++++++++++++++++++
	//								Assign Data values
	bool define_hcp_parameter_values(id_type object_id, voxel_generator_parameters_struct_type data) {
		int object_index = get_object_index(object_id);
		if (object_index < 0) return false;

		//if (data_list.size() != 15) return false;

		//voxel_hcp_object_class* voxel_hcp_object = at(object_index);
		voxel_hcp_object_class *voxel_hcp_object = dynamic_cast <voxel_hcp_object_class*>(category_objects.at(object_index));

		voxel_hcp_object->voxel_object_data.voxel_generator_parameters.expression_file_pathname = data.expression_file_pathname;
		voxel_hcp_object->voxel_object_data.voxel_generator_parameters.expression_file_name = data.expression_file_name;

		voxel_hcp_object->voxel_object_data.voxel_generator_parameters.x_start = data.x_start;
		voxel_hcp_object->voxel_object_data.voxel_generator_parameters.x_end = data.x_end;
		voxel_hcp_object->voxel_object_data.voxel_generator_parameters.y_start = data.y_start;
		voxel_hcp_object->voxel_object_data.voxel_generator_parameters.y_end = data.y_end;
		voxel_hcp_object->voxel_object_data.voxel_generator_parameters.z_start = data.z_start;
		voxel_hcp_object->voxel_object_data.voxel_generator_parameters.z_end = data.z_end;

		voxel_hcp_object->voxel_object_data.voxel_generator_parameters.invocation = data.invocation;
		voxel_hcp_object->voxel_object_data.voxel_generator_parameters.min_surface_value = data.min_surface_value;
		voxel_hcp_object->voxel_object_data.voxel_generator_parameters.max_surface_value = data.max_surface_value;
		voxel_hcp_object->voxel_object_data.voxel_generator_parameters.resolution_step = data.resolution_step;
		voxel_hcp_object->voxel_object_data.voxel_generator_parameters.generation_threshold = data.generation_threshold;

		voxel_hcp_object->voxel_object_data.display_points = data.generator_display_data.display_as_points;
		voxel_hcp_object->voxel_object_data.voxel_size = data.generator_display_data.voxel_scale;

		return true;
	}

	bool define_hcp_real_var_values(id_type object_id, std::vector<voxel_generator_parameter_variable_struct_type> data) {
		int object_index = get_object_index(object_id);
		if (object_index < 0) return false;

		voxel_hcp_object_class* voxel_hcp_object = dynamic_cast <voxel_hcp_object_class*>(category_objects.at(object_index));

		if (voxel_hcp_object == NULL) return false;

		voxel_hcp_object->voxel_object_data.voxel_generator_parameters.variables.clear();

		for (voxel_generator_parameter_variable_struct_type real_variable : data) {
			voxel_hcp_object->voxel_object_data.voxel_generator_parameters.variables.push_back(real_variable);
		}

		return true;
	}

	bool define_hcp_int_var_values(id_type object_id, std::vector<voxel_generator_parameter_int_variable_struct_type>  data) {
		int object_index = get_object_index(object_id);
		if (object_index < 0) return false;

		voxel_hcp_object_class* voxel_hcp_object = dynamic_cast <voxel_hcp_object_class*>(category_objects.at(object_index));

		if (voxel_hcp_object == NULL) return false;

		voxel_hcp_object->voxel_object_data.voxel_generator_parameters.int_variables.clear();

		for (voxel_generator_parameter_int_variable_struct_type int_variable : data) {
			voxel_hcp_object->voxel_object_data.voxel_generator_parameters.int_variables.push_back(int_variable);
		}
		return true;
	}

	bool define_hcp_bool_var_values(id_type object_id, std::vector<voxel_generator_parameter_bool_variable_struct_type>  data) {
		int object_index = get_object_index(object_id);
		if (object_index < 0) return false;

		voxel_hcp_object_class* voxel_hcp_object = dynamic_cast <voxel_hcp_object_class*>(category_objects.at(object_index));

		if (voxel_hcp_object == NULL) return false;

		voxel_hcp_object->voxel_object_data.voxel_generator_parameters.bool_variables.clear();

		for (voxel_generator_parameter_bool_variable_struct_type bool_variable : data) {
			voxel_hcp_object->voxel_object_data.voxel_generator_parameters.bool_variables.push_back(bool_variable);
		}

		return true;
	}

	//	++++++++++++++++++++++++ retrieve Data values ++++++++++++++++++++++++++


private:
};
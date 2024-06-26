#pragma once

#include <vector>

//#include <Source/Editor/Common/definitions.h>
//#include <Source/Editor/Shader/shader_import_export.h>

#include <VW_framework/Scene/scene_object_categories.h>

#include <VW/Editor/Shader/shader_import_export.h>

#include "../../Object/hex_surface_object.h"

/*
				Hex surface scene objects class

	This class defines and manages the storage update and retrieval
	of the 2D hexagonal surface data objects that are to be used by
	the Virtual Worlds application.

	The data storage model utilised is a simple C++ standard vector
	array of data type hex_surface_object_class that the class 
	inherits.
*/
// ********
//#include <Source/Editor/Scene/objects_category_basis.h>

//class hex_surface_scene_objects_class  : public vw_scene_objects_category_class, public std::vector<hex_surface_object_class*> {
class hex_surface_scene_objects_class  : public vw_scene_objects_category_class {
	//voxel_function_import_export_class      voxel_function_import_export;
	//voxel_texture_import_export_class       voxel_texture_import_export;
	//import_export_byte_automata_rules_class import_export_byte_automata_rules;

public:
/*	vw_object_base_class* get_entity_object(id_type object_id) {
		int index = get_object_index(object_id);
		//printf("voxel_hcp_scene_objects_class :: get_voxel_hcp_object %i\n", index);
		if (index > -1) return at(index);

		return NULL;
	}

	hex_surface_object_class* get_hex_surface_object(id_type object_id) {
		int index = get_object_index(object_id);
		//printf("voxel_hcp_scene_objects_class :: get_voxel_hcp_object %i\n", index);
		if (index > -1) return at(index);

		return NULL;
	}

	bool get_shader_parameters(id_type object_id, material_struct_type& shader_parameters) {
		int object_index = get_object_index(object_id);
		if (object_index < 0) return false;

		hex_surface_object_class* hex_surface_object = at(object_index);

		shader_parameters = hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters;

		return true;
	}

	bool save_generation_data(id_type object_id, string_type file_pathname) {
		int object_index = get_object_index(object_id);
		if (object_index < 0) return false;

		hex_surface_object_class* hex_surface_object = at(object_index);

	}

	int get_object_index(id_type object_id) {
		//printf("voxel_hcp_scene_objects_class :: get_object_index %i \n", size());
		for (int i = 0; i < size(); i++) {
			//printf("voxel_hcp_scene_objects_class :: get_object_index %i : %i\n", i, at(i)->object_id);
			if (at(i)->id == object_id) return i;
		}

		return -1;
	}

	bool have_object_of_id(id_type object_id) {
		for (int i = 0; i < size(); i++) {
			if (at(i)->id == object_id) return true;
		}

		return false;
	}

	bool add_new_object(id_type object_id) {
		hex_surface_object_class* new_object = new (hex_surface_object_class);
		if (!new_object) return false;

		new_object->id = object_id;
		//new_hcp_object.object_category_id = ; // to be determined
		push_back(new_object);
		return true;
	}

	bool delete_object(id_type object_id) {
		int delete_index = get_object_index(object_id);
		if (delete_index > -1 && delete_index < size()) {

			hex_surface_object_class *hex_surface_object_to_delete = at(delete_index);
			delete hex_surface_object_to_delete;

			erase(begin() + delete_index);
			shrink_to_fit();
			return true;
		}

		return false;
	}

	void delete_objects() {
		for (int i = 0; i < size(); i++) {
			hex_surface_object_class *hex_surface_object_to_delete = at(i);
			delete hex_surface_object_to_delete;
		}
		clear();
	}

	bool rename_object_id(id_type old_object_id, id_type new_object_id) {
		int object_index = get_object_index(old_object_id);
		if (object_index > -1) {
			at(object_index)->id = new_object_id;
			return true;
		}
		return false;
	}
*/
	// ++++++++++++++++++++++++ Voxel generation data ++++++++++++++++++++++++++++++++++++++
	//								Assign Data values
	bool define_hcp_parameter_values(id_type object_id, hex_surface_generator_parameters_struct_type data) {
		int object_index = get_object_index(object_id);
		if (object_index < 0) return false;

		//if (data_list.size() != 15) return false;

		//hex_surface_object_class* hex_surface_object = at(object_index);
		hex_surface_object_class *hex_surface_object = dynamic_cast <hex_surface_object_class*>(category_objects.at(object_index));

		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.expression_file_pathname = data.expression_file_pathname;
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.expression_file_name     = data.expression_file_name;

		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.x_start = data.x_start;
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.x_end   = data.x_end;
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.y_start = data.y_start;
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.y_end   = data.y_end;

		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.invocation = data.invocation;
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.min_surface_value = data.min_surface_value;
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.max_surface_value = data.max_surface_value;
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.resolution_step   = data.resolution_step;

		return true;
	}

	bool define_hcp_real_var_values(id_type object_id, std::vector<hex_surface_generator_parameter_variable_struct_type> data) {
		int object_index = get_object_index(object_id);
		if (object_index < 0) return false;

		//hex_surface_object_class* hex_surface_object = at(object_index);
		hex_surface_object_class *hex_surface_object = dynamic_cast <hex_surface_object_class*>(category_objects.at(object_index));

		if (hex_surface_object == NULL) return false;

		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.variables.clear();

		for (hex_surface_generator_parameter_variable_struct_type real_variable : data) {
			hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.variables.push_back(real_variable);
		}

		return true;
	}

	bool define_hcp_int_var_values(id_type object_id, std::vector<hex_surface_generator_parameter_int_variable_struct_type>  data) {
		int object_index = get_object_index(object_id);
		if (object_index < 0) return false;


		//hex_surface_object_class* hex_surface_object = at(object_index);
		hex_surface_object_class *hex_surface_object = dynamic_cast <hex_surface_object_class*>(category_objects.at(object_index));

		if (hex_surface_object == NULL) return false;

		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.int_variables.clear();

		for (hex_surface_generator_parameter_int_variable_struct_type int_variable : data) {
			hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.int_variables.push_back(int_variable);
		}
		return true;
	}

	bool define_hcp_bool_var_values(id_type object_id, std::vector<hex_surface_generator_parameter_bool_variable_struct_type>  data) {
		int object_index = get_object_index(object_id);
		if (object_index < 0) return false;

		//hex_surface_object_class* hex_surface_object = at(object_index);
		hex_surface_object_class *hex_surface_object = dynamic_cast <hex_surface_object_class*>(category_objects.at(object_index));

		if (hex_surface_object == NULL) return false;

		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.bool_variables.clear();

		for (hex_surface_generator_parameter_bool_variable_struct_type bool_variable : data) {
			hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters.bool_variables.push_back(bool_variable);
		}

		return true;
	}

	//	++++++++++++++++++++++++ retrieve Data values ++++++++++++++++++++++++++


private:
};

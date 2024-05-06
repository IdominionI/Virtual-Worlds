#pragma once

#include <FrameWork/Universal_FW/Animation/animation.h>

#include "../Object/voxel_hcp_object.h"

#include "../Editor/Widgets/hcp_voxel_shaders_widget.h"

#define TIMELINE_OBJECT_DATA_TYPE_ID_VOXEL_SHADER 1003

// Following not yet implemented and may not be required just yet
/*
struct hcp_keyframe_data_struct_type : public animation_keyframe_basis_class {


};

class hcp_voxel_animation_keyframe_class  {
public:
	hcp_keyframe_data_struct_type hcp_keyframe_data;

private:
};
*/

class hcp_animation_shader_class : public animation_object_basis_class {
public:
	log_panel_class* log_panel = nullptr;

	// List of pointers to the hcp voxel data objects in the Virtual Worlds scene data model that are to be rendered in the scene
	voxel_hcp_object_class* voxel_hcp_object = nullptr;

	// Initial hcp voxel data object parameter data that are to be rendered in the scene
	material_struct_type					vw_initial_voxel_shader_generator_variables, current_voxel_shader_generator_variables;

	bool setup_animation_data(animation_data_struct_type animation_settings) {
		//std::cout << "hcp_animation_object_class:setup_animation_data : in setup_animation_data\n";
		if (!voxel_hcp_object) {
std::cout << "hcp_animation_object_class:setup_animation_data : !voxel_hcp_object\n";
			return false;
		}

		animation_data = animation_settings;

		vw_initial_voxel_shader_generator_variables = voxel_hcp_object->voxel_object_data.shader_parameters;// In future this may be a seperate animation entity linked to the hcp voxel generation 

		current_voxel_shader_generator_variables = vw_initial_voxel_shader_generator_variables;

		// initialise for hcp voxel shader 
		voxel_shader.voxel_hcp_object_to_execute = voxel_hcp_object;
		voxel_shader.current_selected_entity_id = voxel_hcp_object->id;
		voxel_shader.voxel_shader_parameters = &voxel_hcp_object->voxel_object_data.shader_parameters;

		return true;
	}

	bool update_animation_data(animation_data_struct_type animation_settings) {
std::cout << "hcp_animation_object_class:update_animation_data : in update_animation_data " << animation_settings.frame_step_interval() << std::endl;

		animation_data = animation_settings;

		current_voxel_shader_generator_variables.frame = animation_settings.current_frame;

		current_voxel_shader_generator_variables.update_shader_variables_value_increment(animation_settings.frame_step_interval());

		// If other parameter variables are influenced by the current frame or frame step interval place updates here

		return true;
	}

	void perform_animation_for_frame(int frame) {
std::cout << "hcp_animation_object_class:perform_animation_for_frame : in perform_animation_for_frame\n";
		// need to update shader program with hcp voxel shader parameters variables here 

		switch (animation_data.animation_action) {
		case animation_action_enum::play         : voxel_shader.perform_increment_variables(animation_data.frame_step_interval()); break;
		case animation_action_enum::step_forward : voxel_shader.perform_increment_variables(animation_data.frame_step_interval()); break;
		case animation_action_enum::reverse_play : voxel_shader.perform_decrement_variables(animation_data.frame_step_interval()); break;
		case animation_action_enum::step_back    : voxel_shader.perform_decrement_variables(animation_data.frame_step_interval()); break;
		}
	}

	void perform_animation_for_time(float time) {
		// not impemented for hcp voxel
	}

	void restore_initial_frame() {
		voxel_hcp_object->voxel_object_data.shader_parameters = vw_initial_voxel_shader_generator_variables;// In future this may be a seperate animation entity linked to the hcp voxel generation 
	} // A function that is defined in the derived animation object class using its unique data


private:
	voxel_shaders_widget_class        voxel_shader;
	animation_data_struct_type        animation_data;


};

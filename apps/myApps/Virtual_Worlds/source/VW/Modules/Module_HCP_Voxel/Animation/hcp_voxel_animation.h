#pragma once

#include <FrameWork/Universal_FW/Animation/animation.h>

#include "../Object/voxel_hcp_object.h"

#include "../Editor/Widgets/hcp_voxel_generator_widget.h"

#define TIMELINE_OBJECT_DATA_TYPE_ID_VOXEL 1000

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

class hcp_animation_object_class : public animation_object_basis_class {
public:
	log_panel_class *log_panel = nullptr;

	// List of pointers to the hcp voxel data objects in the Virtual Worlds scene data model that are to be rendered in the scene
	voxel_hcp_object_class *voxel_hcp_object=nullptr;

	// Initial hcp voxel data object parameter data that are to be rendered in the scene
	voxel_generator_parameters_struct_type  vw_initial_voxel_generator_variables, current_voxel_generator_variables;
	material_struct_type					vw_initial_voxel_shader_generator_variables, current_voxel_shader_generator_variables;
	
	// hcp voxel data object compute generator classes that generate the hcp point cloud data the are to be rendered in the scene
	voxel_compute_generator_class *voxel_generator;

	bool setup_animation_data(animation_data_struct_type animation_settings) {
//std::cout << "hcp_animation_object_class:setup_animation_data : in setup_animation_data\n";
		if (!voxel_hcp_object) {
std::cout << "hcp_animation_object_class:setup_animation_data : !voxel_hcp_object\n";
			return false;
		}

		animation_data = animation_settings;

		vw_initial_voxel_generator_variables        = voxel_hcp_object->voxel_object_data.voxel_generator_parameters;
		vw_initial_voxel_shader_generator_variables = voxel_hcp_object->voxel_object_data.shader_parameters;// In future this may be a seperate animation entity linked to the hcp voxel generation 

		current_voxel_generator_variables        = vw_initial_voxel_generator_variables;
		current_voxel_shader_generator_variables = vw_initial_voxel_shader_generator_variables;

		voxel_hcp_generation.voxel_hcp_object_to_execute = voxel_hcp_object;
		voxel_hcp_generation.current_selected_entity_id  = voxel_hcp_object->id;

		// ******************* Testing only delete/comment out when testing complete
		//load_test_data();

		// *******************************

		return true; 
	}

	bool update_animation_data(animation_data_struct_type animation_settings) {
std::cout << "hcp_animation_object_class:update_animation_data : in update_animation_data " << animation_settings.frame_step_interval() << std::endl;

		animation_data = animation_settings;

		current_voxel_generator_variables.frame        = animation_settings.current_frame;
		current_voxel_shader_generator_variables.frame = animation_settings.current_frame;

		current_voxel_generator_variables.update_variables_value_increment(animation_settings.frame_step_interval());
		current_voxel_shader_generator_variables.update_shader_variables_value_increment(animation_settings.frame_step_interval());

		// If other parameter variables are influenced by the current frame or frame step interval place updates here

		return true; 
	}

	void perform_animation_for_frame(int frame) {
std::cout << "hcp_animation_object_class:perform_animation_for_frame : in perform_animation_for_frame\n";
		// need to update shader program with hcp voxel shader parameters variables here 
		
		switch (animation_data.animation_action){
			case animation_action_enum::play         : voxel_hcp_generation.perform_increment_variables(animation_data.frame_step_interval()); break;
			case animation_action_enum::step_forward : voxel_hcp_generation.perform_increment_variables(animation_data.frame_step_interval()); break;
			case animation_action_enum::reverse_play : voxel_hcp_generation.perform_decrement_variables(-animation_data.frame_step_interval()); break;
			case animation_action_enum::step_back    : voxel_hcp_generation.perform_decrement_variables(-animation_data.frame_step_interval()); break;
		}
	}

	void perform_animation_for_time(float time) {
		// not impemented for hcp voxel
	}

	void restore_initial_frame() {
		voxel_hcp_object->voxel_object_data.voxel_generator_parameters = vw_initial_voxel_generator_variables;
		voxel_hcp_object->voxel_object_data.shader_parameters          = vw_initial_voxel_shader_generator_variables;// In future this may be a seperate animation entity linked to the hcp voxel generation 
	} // A function that is defined in the derived animation object class using its unique data


private:
	voxel_hcp_generation_widget_class voxel_hcp_generation;
	animation_data_struct_type        animation_data;

	// ******************* Testing only delete/comment out when testing complete
	//void load_test_data() {
	//	voxel_hcp_generation.load_generation_parameters();
	//}

	// *******************************
};
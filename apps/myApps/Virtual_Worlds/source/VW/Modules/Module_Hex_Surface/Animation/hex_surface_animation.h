#pragma once

#include <FrameWork/Universal_FW/Animation/animation.h>

#include "../Object/hex_surface_object.h"

#include "../Editor/Widgets/hex_generator_widget.h"
#include "../Editor/Widgets/hex_shaders_widget.h"

#define TIMELINE_OBJECT_DATA_TYPE_ID_HEX_SURFACE 1200

// Following not yet implemented and may not be required just yet
/*
struct keyframe_data_struct_type : public animation_keyframe_basis_class {


};

class hex_surface_animation_keyframe_class  {
public:
	keyframe_data_struct_type keyframe_data;

private:
};
*/

class hex_surface_animation_object_class : public animation_object_basis_class {
public:
	log_panel_class *log_panel = nullptr;

	// List of pointers to the  hex_surface data objects in the Virtual Worlds scene data model that are to be rendered in the scene
	hex_surface_object_class *hex_surface_object=nullptr;

	// Initial  hex_surface data object parameter data that are to be rendered in the scene
	hex_surface_generator_parameters_struct_type  vw_initial_hex_surface_generator_variables, current_hex_surface_generator_variables;
	material_struct_type					      vw_initial_hex_surface_shader_generator_variables, current_hex_surface_shader_generator_variables;
	
	//  hex_surface data object compute generator classes that generate the  point cloud data the are to be rendered in the scene
	hex_surface_compute_generator_class *hex_surface_generator;

	unsigned int hex_surface_interval_track_id        = UINT_MAX;
	unsigned int hex_surface_shader_interval_track_id = UINT_MAX;

	bool setup_animation_data(animation_data_struct_type animation_settings) {
//std::cout << "animation_object_class:setup_animation_data : in setup_animation_data\n";
		if (!hex_surface_object) {
//std::cout << "animation_object_class:setup_animation_data : !hex_surface_object\n";
			return false;
		}

		animation_data = animation_settings;

		vw_initial_hex_surface_generator_variables        = hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters;
		vw_initial_hex_surface_shader_generator_variables = hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters;// In future this may be a seperate animation entity linked to the  hex_surface generation 

		current_hex_surface_generator_variables        = vw_initial_hex_surface_generator_variables;
		current_hex_surface_shader_generator_variables = vw_initial_hex_surface_shader_generator_variables;

		// initilise for  hex_surface generation
		hex_surface_generation.hex_surface_object_to_execute = hex_surface_object;
		hex_surface_generation.current_selected_entity_id  = hex_surface_object->id;

		// initialise for  hex_surface shader 
		hex_surface_shader.hex_surface_object_to_execute     = hex_surface_object;
		hex_surface_shader.current_selected_entity_id        = hex_surface_object->id;
		hex_surface_shader.hex_surface_shader_parameters     = &hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters;

		return true; 
	}

	bool update_animation_data(animation_data_struct_type animation_settings) {
//std::cout << "animation_object_class:update_animation_data : in update_animation_data " << animation_settings.frame_step_interval() << std::endl;

		animation_data = animation_settings;

		current_hex_surface_generator_variables.frame        = animation_settings.current_frame;
		current_hex_surface_shader_generator_variables.frame = animation_settings.current_frame;

		current_hex_surface_generator_variables.update_variables_value_increment(animation_settings.frame_step_interval());
		current_hex_surface_shader_generator_variables.update_shader_variables_value_increment(animation_settings.frame_step_interval());

		// If other parameter variables are influenced by the current frame or frame step interval place updates here

		return true; 
	}

	void perform_animation_for_frame(int frame , int track) {
//std::cout << "animation_object_class:perform_animation_for_frame : in perform_animation_for_frame\n";
		// need to update shader program with  hex_surface shader parameters variables here 
		
		if(track == hex_surface_interval_track_id){
			switch (animation_data.animation_action){
				case animation_action_enum::play         : {hex_surface_generation.perform_increment_variables(animation_data.frame_step_interval()); 
															break;}
				case animation_action_enum::step_forward : {hex_surface_generation.perform_increment_variables(animation_data.frame_step_interval()); 
															break;}
				case animation_action_enum::reverse_play : {hex_surface_generation.perform_decrement_variables(-animation_data.frame_step_interval());
															break;}
				case animation_action_enum::step_back    : {hex_surface_generation.perform_decrement_variables(-animation_data.frame_step_interval());
															break;}
			}
		}

		if(track == hex_surface_shader_interval_track_id){
			switch (animation_data.animation_action){
				case animation_action_enum::play         : {hex_surface_shader.perform_increment_variables(animation_data.frame_step_interval());
															break;}
				case animation_action_enum::step_forward : {hex_surface_shader.perform_increment_variables(animation_data.frame_step_interval());
															break;}
				case animation_action_enum::reverse_play : {hex_surface_shader.perform_decrement_variables(-animation_data.frame_step_interval());
															break;}
				case animation_action_enum::step_back    : {hex_surface_shader.perform_decrement_variables(-animation_data.frame_step_interval());
															break;}
			}
		}
	}

	void perform_animation_for_time(float time) {
		// not impemented for  hex_surface
	}

	void restore_initial_frame() {
		hex_surface_object->hex_surface_object_data.hex_surface_generator_parameters = vw_initial_hex_surface_generator_variables;
		hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters    = vw_initial_hex_surface_shader_generator_variables;// In future this may be a seperate animation entity linked to the  hex_surface generation 
	} // A function that is defined in the derived animation object class using its unique data


private:
	hex_surface_generation_widget_class hex_surface_generation;
	hex_surface_shaders_widget_class    hex_surface_shader;
	animation_data_struct_type          animation_data;


};
#pragma once

#include <FrameWork/Universal_FW/Animation/animation.h>

#include "../Object/voxel_hcp_object.h"

#include "../Editor/Widgets/hcp_voxel_generator_widget.h"
#include "../Editor/Widgets/hcp_voxel_automata_widget.h"

#define TIMELINE_OBJECT_DATA_TYPE_ID_VOXEL_AUTOMATA 1002

class hcp_voxel_automata_animation_object_class : public animation_object_basis_class {
public:
	log_panel_class* log_panel = nullptr;

	// It may be that in future with some altered code this using a pointer to the master voxel_hcp_automata_widget class
	// does not work and this pointer becomes a local variable that needs initialisation 
	voxel_hcp_automata_widget_class *voxel_hcp_automata_widget = nullptr;

	bool setup_animation_data(animation_data_struct_type animation_settings) {
//std::cout << "hcp_animation_object_class:setup_animation_data : in setup_animation_data\n";
		//if (!voxel_hcp_object || !voxel_hcp_automata_byte_rules) {
		if (!voxel_hcp_automata_widget) {
std::cout << "hcp_voxel_automata_animation_object_class:setup_animation_data : !voxel_hcp_object\n";
			return false;
		}

		animation_data = animation_settings;

		//voxel_hcp_automata_widget.voxel_hcp_object_to_execute = voxel_hcp_object;
		//voxel_hcp_automata_widget.entity_id  = voxel_hcp_object->id;

		return true;
	}

	bool update_animation_data(animation_data_struct_type animation_settings) {
//std::cout << "hcp_voxel_automata_animation_object_class:update_animation_data : in update_animation_data " << animation_settings.frame_step_interval() << std::endl;

		animation_data = animation_settings;

		return true; 
	}


	void perform_animation_for_frame(int frame) {
std::cout << "hcp_voxel_automata_animation_object_class:perform_animation_for_frame : in perform_animation_for_frame hcp_voxel_automata_animation_object_class\n";

		if(!voxel_hcp_automata_widget) return ;

		switch (animation_data.animation_action){
			case animation_action_enum::play        : {std::cout << "play :\n";voxel_hcp_automata_widget->next_step_automata_execution(); break;}
			case animation_action_enum::step_forward: {std::cout << "step_forward :\n"; voxel_hcp_automata_widget->next_step_automata_execution();  break;}
			// Automata cannot be animated in reverse motion as next step is a function of the current voxel state 
		}
	}

private:
	//voxel_hcp_automata_widget_class voxel_hcp_automata_widget; 
	animation_data_struct_type      animation_data;
};

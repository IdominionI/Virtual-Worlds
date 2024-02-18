#pragma once

#include <FrameWork/Universal_FW/Animation/animation.h>

#include "../interaction_node.h"

#define TIMELINE_OBJECT_DATA_TYPE_ID_HEX_SURFACE_INTERACTION 10200

class hex_surface_interaction_animation_class : public animation_object_basis_class {
public:
	log_panel_class* log_panel = nullptr;

	hex_surface_interaction_node_class *hex_surface_interaction_node =nullptr;

	bool setup_animation_data(animation_data_struct_type animation_settings) {
//std::cout << "hcp_animation_object_class:setup_animation_data : in setup_animation_data\n";
		if (!hex_surface_interaction_node) {
//std::cout << "hex_surface_automata_animation_object_class:setup_animation_data : !voxel_hcp_object\n";
			return false;
		}

		animation_data = animation_settings;

		return true;
	}

	bool update_animation_data(animation_data_struct_type animation_settings) {
//std::cout << "hex_surface_automata_animation_object_class:update_animation_data : in update_animation_data " << animation_settings.frame_step_interval() << std::endl;

		animation_data = animation_settings;

		return true; 
	}


	void perform_animation_for_frame(int frame, int track) { // Not yet tested
//std::cout << "hex_surface_automata_animation_object_class:perform_animation_for_frame : in perform_animation_for_frame hex_surface_automata_animation_object_class\n";

		if(!hex_surface_interaction_node) return ;

		switch (animation_data.animation_action){
			case animation_action_enum::play        : {hex_surface_interaction_node->action(); break;}
			case animation_action_enum::step_forward: {hex_surface_interaction_node->action();  break;}
			case animation_action_enum::reverse_play: {hex_surface_interaction_node->action();  break;}
			case animation_action_enum::step_back   : {hex_surface_interaction_node->action();  break;}
		}
	}

private:
	animation_data_struct_type      animation_data;
};
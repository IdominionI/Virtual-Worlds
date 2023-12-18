#pragma once

#include <VW_framework/Types/vw_type_definitions.h>

#include <VW/Editor/Main_Window/Panels/log_panel.h>

class voxel_hcp_object_class;
class hcp_voxel_generation_basis_widget_class {
public:
	log_panel_class* log_panel = NULL;

	id_type                 current_selected_entity_id  = INVALID_ID;  // entity id of the selected entity to display/modify
	voxel_hcp_object_class *voxel_hcp_object_to_execute = nullptr; // Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model

	void virtual display() {}
	void virtual change_voxels_display() {}
	void virtual update_voxel_size(){}

	// Add more here as required

private:
	//void virtual display_voxel_generation_widget(voxel_hcp_object_class* voxel_hcp_object_to_execute, id_type current_selected_entity_id) {}

	// Add more here as required

};
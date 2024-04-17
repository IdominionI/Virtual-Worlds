#include "hex_surface_editor_parameters_widget.h"

#include "../../Object/hex_surface_object.h"

// Including any of the following generates linking errors regarding "imgui_custom_definitions.h" unkown why
//#include "generation_selection_widget.h"
//#include "shaders_widget.h"
//#include "automata_widget.h"

void hex_surface_parameters_editor_widget_class::display_parameters_widget() {
//printf("parameter_panel_class :: display_voxel_hcp_generation_widget 11\n");

	if (current_selected_object_id < 0) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Invalid selected ID number given for the current selected hcp voxel object. No object selected\n");
		return;
	}

	hex_surface_object_class* hex_surface_object_to_execute = dynamic_cast<hex_surface_object_class*>(object_to_execute);	// Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model

	if (hex_surface_object_to_execute == NULL) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hcp voxel object defined to perform tasks on.\n");
		return;
	}

	hex_analysis_shader->current_selected_entity_id  = current_selected_object_id;// ++++
	hex_analysis_shader->hex_surface_object_to_execute = hex_surface_object_to_execute;// ++++


	if (ImGui::BeginTabBar("##hcp_voxel_editor_tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Generation##hex_gen_tab")) {
			display_hex_surface_generation_widget(hex_surface_object_to_execute, current_selected_object_id);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Editor##hex_ed_tab")) {
			display_hex_surface_editing_widget(hex_surface_object_to_execute, current_selected_object_id);
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
}

void hex_surface_parameters_editor_widget_class::display_hex_surface_generation_widget(hex_surface_object_class* hex_surface_object_to_execute, id_type current_selected_object_id) {
	hex_surface_generation_widget.hex_surface_object_to_execute = hex_surface_object_to_execute;

	if (hex_surface_generation_widget.hex_surface_object_to_execute == NULL) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hcp voxel object defined to perform voxel generation procedure on.\n");
		return;
	}

	hex_surface_generation_widget.hex_surface_generation_widget.hex_analysis_shader = hex_analysis_shader;
	hex_surface_generation_widget.log_panel = log_panel;
	hex_surface_generation_widget.current_selected_entity_id = current_selected_object_id;
	hex_surface_generation_widget.display();
}


void hex_surface_parameters_editor_widget_class::display_hex_surface_editing_widget(hex_surface_object_class* hex_surface_object_to_execute, id_type current_selected_object_id) {
	hex_surface_editor_editing_widget.hex_surface_object_to_execute = hex_surface_object_to_execute;

	if (hex_surface_editor_editing_widget.hex_surface_object_to_execute == NULL) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hcp voxel object defined for voxel shader widget to perform actions on.\n");
		return;
	}

	hex_surface_editor_editing_widget.hex_analysis_shader = hex_analysis_shader;
	hex_surface_editor_editing_widget.log_panel = log_panel;
	hex_surface_editor_editing_widget.current_selected_entity_id = current_selected_object_id;

	hex_surface_editor_editing_widget.display();
}

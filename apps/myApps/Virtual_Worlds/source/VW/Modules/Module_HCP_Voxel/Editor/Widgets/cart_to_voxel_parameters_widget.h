#pragma once
#include <iostream>

#include <imgui-docking/imgui.h>

#include <VW_framework/Widgets/parameter_widget_base.h>

// Including any of the following generates linking errors unkown why
#include "cart_to_voxel_generation_widget.h"
#include "hcp_voxel_shaders_widget.h"

#include "../../Object/voxel_hcp_object.h"
/*
				HCP voxel volume paramters widget

	This widget class is a parent widget that manages the display
	of HCP voxel generation, shader variable and cellula automata child
	widgets via selection of ImGui tabs.

	This widget class is thus the widget class that is called by
	the main application editor to display hex surface parameter
	data to be changed or generate data.
*/

class cart_to_parameters_widget_class : public parameter_widget_base_class {
public:
	void display_parameters_widget(){

		if (current_selected_object_id < 0) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Invalid selected ID number given for the current selected hcp voxel object. No object selected\n");
			return;
		}

		voxel_hcp_object_class* voxel_hcp_object_to_execute = dynamic_cast<voxel_hcp_object_class*>(object_to_execute);	// Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model

		if (voxel_hcp_object_to_execute == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hcp voxel object defined to perform tasks on.\n");
			return;
		}

		if (ImGui::BeginTabBar("##cart_to_hcp_voxel_tabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Cart_to_vaxel_Generation##hcp_cart_to_voxel_tab")) {
				display_cart_to_voxel_widget(voxel_hcp_object_to_execute, current_selected_object_id);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Shader##cart_to_voxel_shader_tab")) {
				display_voxel_shader_widget(voxel_hcp_object_to_execute, current_selected_object_id);
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}

	voxel_shaders_widget_class             cart_to_voxel_shaders_widget;// Need to modify voxel_shaders_widget_class to use a template for cart_to_voxel_shaders_widget_class and voxel_shaders_widget
	cart_to_voxel_generation_widget_class  cart_to_voxel_widget;

private:
	//voxel_shaders_widget_class             cart_to_voxel_shaders_widget;// Need to modify voxel_shaders_widget_class to use a template for cart_to_voxel_shaders_widget_class and voxel_shaders_widget
	//cart_to_voxel_generation_widget_class  cart_to_voxel_widget;

	void display_cart_to_voxel_widget(voxel_hcp_object_class* voxel_hcp_object_to_execute, id_type current_selected_object_id) {
		cart_to_voxel_widget.voxel_hcp_object_to_execute = voxel_hcp_object_to_execute;

		if (cart_to_voxel_widget.voxel_hcp_object_to_execute == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hcp voxel object defined to perform cart to voxel generation procedure on.\n");
			return;
		}
//printf("voxel_hcp_generation_widget.voxel_hcp_object_to_execute != NULL ^^^^\n");
		cart_to_voxel_widget.log_panel = log_panel;
		cart_to_voxel_widget.current_selected_entity_id = current_selected_object_id;
//printf("parameter_panel_class :: display_voxel_hcp_generation_widget22\n");
		cart_to_voxel_widget.display();
	}

	void display_voxel_shader_widget(voxel_hcp_object_class* voxel_hcp_object_to_execute, id_type current_selected_object_id) {
		cart_to_voxel_shaders_widget.voxel_hcp_object_to_execute = voxel_hcp_object_to_execute;
		cart_to_voxel_shaders_widget.hcp_voxel_generation_widget = &cart_to_voxel_widget;

		if (cart_to_voxel_shaders_widget.voxel_hcp_object_to_execute == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hcp voxel object defined for voxel shader widget to perform actions on.\n");
			return;
		}

		if (cart_to_voxel_shaders_widget.hcp_voxel_generation_widget == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : The voxel generation widget has not been defined for the voxel shader widget to use\n");
			return;
		}

		cart_to_voxel_shaders_widget.log_panel = log_panel;
		cart_to_voxel_shaders_widget.current_selected_entity_id = current_selected_object_id;

		cart_to_voxel_shaders_widget.display();

	}
};
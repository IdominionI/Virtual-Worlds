#pragma once

#include <FrameWork/Kernels/imgui-docking/imgui.h>
#include <FrameWork/Kernels/ImGuiEx/imgui_widgets.h>

#include <VW_framework/Scene/scene_entities_manager.h>

#include "../Widgets/parameters_widget.h"

#include "../Widgets/global_light_widget.h"
#include "../Widgets/camera_widget.h"
#include "../Widgets/viewport_properties_widget.h"

/*
							PARAMETER PANEL

	Parent class to define the application parameter panel that is the parent
	"window widget" for the application to display the entity parameter data
	into and enable whatever functions. procedures etc top be performed for
	that entity
*/

class parameter_panel_class {
public:
	parameter_panel_class() {

	}

	~parameter_panel_class() {}

	scene_entities_manager_class        *scene_manager = NULL;

	global_light_parameters_widget_class global_light_parameters_widget;
	camera_poperties_widget_class        camera_poperties_widget;
	viewport_properties_widget_class     viewport_properties_widget;

	log_panel_class *log_panel  = NULL; // Define the application log panel to display application messages to
	log_panel_class *code_panel = NULL; // Define the application log panel to display application shder or other code error messages to

	void show() {
		ImGui::Begin("Parameters");
			if (ImGui::BeginTabBar("##app_parameter_data", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem("Object##object_tab")) {
					display_object_parameters();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Global Light##global_light_tab")) {
					display_global_light_parameters();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Camera##camera_tab")) {
					display_camera_parameters();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Viewport##viewport_tab")) {
					display_viewport_parameters();
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}

		ImGui::End();
	}

	void display_object_parameters() {

		switch (globalc::get_current_selected_data_context_id()) {

			case NODE_DATA_CONTEXT: {
				node_basis_class* current_selected_node = globalc::get_current_selected_node();
				if (current_selected_node) {
//std::cout << "current_selected_node != NULL :" << current_selected_node->node_id << ":" << current_selected_node->label << std::endl;
					current_selected_node->display_ui();
				}
				break;
			}

//std::cout << "parameter_panel_class : show : " << globalc::get_current_selected_entity_type_id() << ":" << globalc::get_current_selected_entity_category_type_id() << ":" << globalc::get_current_selected_entity_id() << std::endl;
			case ENTITY_DATA_CONTEXT:{
//std::cout << "ENTITY_DATA_CONTEXT :" << globalc::get_current_selected_entity_type_id() << ":" << globalc::get_current_selected_entity_category_type_id() << ":" << globalc::get_current_selected_entity_id() << std::endl;
				scene_manager->display_entity_parameters(globalc::get_current_selected_entity_type_id(), globalc::get_current_selected_entity_category_type_id(), globalc::get_current_selected_entity_id()); break;
			}
//std::cout << "parameter_panel_class : show : " << current_selected_entity_type_id << ":" << current_selected_entity_category_type_id << ":" << current_selected_entity_id << std::endl;
		}
	}

	void display_global_light_parameters() {
		title("Global Lighting");
		global_light_parameters_widget.display();
	}

	void display_camera_parameters() {
		title("Camera");
		camera_poperties_widget.display();
	}

	void display_viewport_parameters() {
		title("Viewport");
		viewport_properties_widget.display();
	}
};
#pragma once

//#include <VW_framework/Scene/scene_entities_manager.h>
#include <VW_framework/Scene/vw_scene.h>

#include <FrameWork/Kernels/ImGuiEx/imgui_widgets.h>

#include "log_panel.h"

#include "../Widgets/scene_node_editor_widget.h"

class node_editor_panel_class {
public:
	vw_scene_class  *vw_scene  = NULL;
	log_panel_class *log_panel = NULL; // Define the application log panel to display application messages to

	ImNodesContext   *nodes_context = NULL;
	node_basis_class *selected_node = NULL;

	void show() {
		auto flags = ImGuiWindowFlags_MenuBar;

		// The node editor window
//std::cout << "node_editor_panel_class : show 00 : " << scene_node_editors.size();
if(!log_panel) std::cout << "node_editor_panel_class : show 00 : !log_panel \n";
if(!vw_scene) std::cout << "node_editor_panel_class : show 00 : !vw_scene \n";

		for (scene_node_editor_class *scene_node_editor : scene_node_editors) {
std::string ids = "Scene Node Editor " + std::to_string(scene_node_editor->editor_id);
			if (scene_node_editor->display_on) {
				ImGui::Begin(ids.c_str(), NULL, flags);
					//scene_node_editor->ed
					scene_node_editor->log_panel = log_panel;
					scene_node_editor->vw_scene = vw_scene;
					scene_node_editor->display();
				ImGui::End();
			}
		}
	}


};





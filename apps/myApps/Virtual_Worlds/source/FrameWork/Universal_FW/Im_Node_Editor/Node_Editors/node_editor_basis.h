#pragma once

#include "node_editor_definitions.h"
#include <Universal_FW/ImNodes/Kernal/node.h>
#include <Universal_FW/ImNodes/Kernal/graph.h>

// delete forward declaration when not needed
//class scene_manager_class;
//class log_panel_class;
class editor_node_basis {
public:
	node_type_struct editor_node_definition;

	//virtual void create_editor_node(node_type_struct ui_node_type, graph_class* graph, scene_manager_class* scene_manager, log_panel_class* log_panel, ImVec2 click_pos) {}
	virtual void create_editor_node(node_type_struct ui_node_type, graph_class* graph,  ImVec2 click_pos) {}

};

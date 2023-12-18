#pragma once
#include <VW/Editor/Main_Window/Panels/log_panel.h>
#include <VW_framework/Scene/vw_scene.h>

// A data structure to pass additional data to be used in constructing nodes in the node
// editor and in particular defining otherwise difficult to pass particular parameter data
// Intwended to be passed as a void pointer.

struct node_misc_import_struct_type {
	log_panel_class *log_panel = nullptr; // Define the application log panel to display application messages to
	vw_scene_class  *vw_scene = nullptr; // Defines the virtual world application scene class that is required 

	// add additional data here
};

enum class node_status_enum {none, unselected, selected, active}; // Add more status designations as required.

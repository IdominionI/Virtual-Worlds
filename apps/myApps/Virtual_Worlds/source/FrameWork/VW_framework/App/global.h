#pragma once

#include <FrameWork/VW_framework/Types/vw_type_definitions.h>

#include <Universal_FW/Kernal/id_key_manager.h>

#include <Universal_FW/ImNodes/Kernal/node.h>

// Singleton class to store global variables and functions
// to be used through out the application

//class scene_manager_class;
//class log_panel_class;
//class node_basis_class;

//inline id_type current_selected_entity_id               = INVALID_ID;
//inline id_type current_selected_entity_type_id          = INVALID_ID;
//inline id_type current_selected_entity_category_type_id = INVALID_ID;

//#define NODE_DATA_CONTEXT                   100
//#define NODE_GROUP_DATA_CONTEXT             101
//#define NODE_GROUP_INPUT_LINK_DATA_CONTEXT  102
//#define NODE_GROUP_OUTPUT_LINK_DATA_CONTEXT 103
//#define ENTITY_DATA_CONTEXT                 110


class globalc {
public:
	globalc(const globalc&) = delete;

	static globalc& get() {
		static globalc instance;
		return instance;
	}

	static id_type get_current_selected_data_context_id() { return get().current_selected_data_context_id; }
	static void    set_current_selected_data_context_id(id_type n) { get().current_selected_data_context_id = n; }

	static id_type get_current_selected_entity_id() { return get().current_selected_entity_id; }
	static void    set_current_selected_entity_id(id_type n) { get().current_selected_entity_id = n; }

	static id_type get_current_selected_entity_type_id() { return get().current_selected_entity_type_id; }
	static void    set_current_selected_entity_type_id(id_type n) { get().current_selected_entity_type_id = n; }

	static id_type get_current_selected_entity_category_type_id() { return get().current_selected_entity_category_type_id; }
	static void    set_current_selected_entity_category_type_id(id_type n) { get().current_selected_entity_category_type_id = n; }

	//The get and set outliner selection global variables are so as to set the above gloabal entity and entity type
	//variables correctly between the outliner and node editor selection of scene entities. Without this the user
	//would need to deselect the node in the node editor before selecting an entity in the outliner widget, otherwise
	//no entity other than the selected node entity will be designated as the current selected entity. 
	static bool get_outliner_selection() { return get().outliner_selection; }
	static void set_outliner_selection(bool n) { get().outliner_selection = n; }

	static node_basis_class *get_current_selected_node() { return get().current_selected_node; }
	static void				 set_current_selected_node(node_basis_class *n) { get().current_selected_node = n; }


	static id_type get_available_entity_id() { return get().entity_id_key.get_available_id_key(); }
	static void     assign_free_entity_id(id_type entity_id) { get().entity_id_key.assign_free_id_key(entity_id); };
	static void     reinitialise() { get().entity_id_key.reinitialise(); };

private:
	globalc() {};

	id_type current_selected_data_context_id         = INVALID_ID;

	id_type current_selected_entity_id               = INVALID_ID;
	id_type current_selected_entity_type_id          = INVALID_ID;
	id_type current_selected_entity_category_type_id = INVALID_ID;

	bool outliner_selection = false;

	node_basis_class *current_selected_node = nullptr;

	// Not sure the following pointers is best here
	// to be reviewed and decided :: 
	// log panel to also be singleton class ?????
	//scene_manager_class *scene_manager = NULL;
	//log_panel_class     *log_panel     = NULL;
	//node_basis_class    *selected_node = NULL;

	id_key_manager_class<id_type> entity_id_key;

};

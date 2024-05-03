#pragma once
// This Class is called exclusively from the scene_node_editor.cpp file
// and used in the scene_node_editor.cpp code thus avoiding circular 
// referencing and need to have functions defined in scene_node_editor.h

#include <fstream>
#include <string>

#define OVER_WRITE_FILE 100
#define READ_FILE       101

class node_editor_import_export_class {
public:
	std::fstream stream;
	std::string  input_line;

	std::vector<std::string> lines;
	std::string line;
	int line_number = 0;

	std::vector<std::pair< node_id_type, node_id_type>>  old_new_node_ids;
	std::vector<std::pair< node_id_type, node_id_type>>  old_new_group_ids;
	std::vector<std::pair< node_id_type, node_id_type>>  old_new_entity_ids;// Probably not needed
	std::vector<std::pair< int, int>>                    old_new_pin_ids;

	int find_new_pin_id_from_old(int old_pin_id) {
		for (std::pair< int, int> old_new_pin_id : old_new_pin_ids) {
			if (old_new_pin_id.first == old_pin_id)
				return old_new_pin_id.second;
		}
		return INVALID_ID;
	}

	void display_import_data(node_group_class *new_node_group,scene_node_editor_class *node_group_editor) {//for testing only: delete or comment out when finisdhed
		std::cout << "node_editor_import_export_class:: display_import_data :: " << std::endl;
		std::cout << "Basic Group Node data " << std::endl;
		std::cout << "visible " << new_node_group->visible << std::endl;
		std::cout << "lable " << new_node_group->label << std::endl;
		std::cout << "category " << new_node_group->ui_node_type.category << std::endl;
		std::cout << "node type " << new_node_group->ui_node_type.node_type << std::endl;

		std::cout << "node_id " << new_node_group->node_id << std::endl;
		std::cout << "node group_id " << new_node_group->node_group_id << std::endl;

		// These are for the editor to use to display node entity parameter UI
		std::cout <<  "node entity id " <<new_node_group->node_entity_id << std::endl;
		std::cout <<  "node cat id " <<new_node_group->node_entity_category_id << std::endl;
		std::cout <<  "entity type id " <<new_node_group->node_entity_type_id << std::endl;

		std::cout << "input pin ids";
		for (int input_pin_id : new_node_group->inputs) {
			std::cout << input_pin_id << ":";
		}
		std::cout << std::endl;

		std::cout << "output pin ids";
		for (int output_pin_id : new_node_group->outputs) {
			std::cout << output_pin_id << ":";
		}
		std::cout << std::endl;

		ImVec2 node_grid_space_pos = new_node_group->graph->node_editor->GetNodeGridSpacePos(new_node_group->node_id);
		std::cout << "grid pos " << node_grid_space_pos.x << ":" << node_grid_space_pos.y << std::endl;
		std::cout << "--------------------------" << std::endl;
		
		//diplay_old_new_data();
		node_group_editor->display_graph_status(node_group_editor);
	}

	void diplay_old_new_data() {
		std::cout << "Old node id : new node id" << std::endl;
		for (std::pair< node_id_type, node_id_type> node_id : old_new_node_ids) {
			std::cout << node_id.first << " : " << node_id.second << std::endl;
		}
		std::cout << "Old group id : new goup id" << std::endl;
		for (std::pair< node_id_type, node_id_type> node_id : old_new_group_ids) {
			std::cout << node_id.first << " : " << node_id.second << std::endl;
		}
		std::cout << "Old entity id : new entity id" << std::endl;
		for (std::pair< node_id_type, node_id_type> node_id : old_new_entity_ids) {
			std::cout << node_id.first << " : " << node_id.second << std::endl;
		}
		std::cout << "Old pin id : new pin id" << std::endl;
		for (std::pair< node_id_type, node_id_type> node_id : old_new_pin_ids) {
			std::cout << node_id.first << " : " << node_id.second << std::endl;
		}
		std::cout << "Old pin node id : new pin node id" << std::endl;
		for (std::pair< node_id_type, node_id_type> node_id : old_new_pin_ids) {
			std::cout << node_id.first << " : " << node_id.second << std::endl;
		}
	}

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// This function is placed here and needs to be modified and maintained when any new node is added/delete to the 
	// node editor.
	node_basis_class *import_new_node_into_editor(node_basis_class _node,scene_node_editor_class *node_group_editor) {
		switch (_node.ui_node_type.node_type) {
			case NODE_TYPE_ID_GROUP_INPUT  : 
			case NODE_TYPE_ID_GROUP_OUTPUT : return import_link_node(node_group_editor,_node.ui_node_type.node_type, _node.node_location); break;
			
			// ADD NEW NODES HERE
			case NODE_TYPE_ID_HCP_VOXEL            : return import_node<hcp_voxel_node_class>(node_group_editor, _node.node_location); break;
			case NODE_TYPE_ID_CART_TO_HCP_VOXEL    : return import_node<cart_to_hcp_voxel_node_class>(node_group_editor, _node.node_location); break;
			case NODE_TYPE_ID_HCP_VOXEL_EDITOR     : return import_node<hcp_voxel_editor_node_class>(node_group_editor, _node.node_location); break;
			case NODE_TYPE_ID_HCP_VOXEL_AUTOMATA   : return import_node<hcp_voxel_automata_node_class>(node_group_editor, _node.node_location); break;
			case NODE_TYPE_ID_HCP_VOXEL_TRANSLATION: return import_node<hcp_voxel_translation_node_class>(node_group_editor, _node.node_location); break;
			case NODE_TYPE_ID_HCP_VOXEL_INTERACTION: return import_node<hcp_voxel_interaction_node_class>(node_group_editor, _node.node_location); break;
			
			
			case NODE_TYPE_ID_HEX_SURFACE             : return import_node<hex_surface_node_class>(node_group_editor, _node.node_location); break;
			case NODE_TYPE_ID_HEX_SURFACE_EDITOR      : return import_node<hex_surface_editor_node_class>(node_group_editor, _node.node_location); break;
			case NODE_TYPE_ID_HEX_SURFACE_AUTOMATA    : return import_node<hex_surface_automata_node_class>(node_group_editor, _node.node_location); break;
			case NODE_TYPE_ID_HEX_SURFACE_TRANSLATION : return import_node<hex_surface_translation_node_class>(node_group_editor, _node.node_location); break;
			case NODE_TYPE_ID_HEX_SURFACE_INTERACTION : return import_node<hex_surface_interaction_node_class>(node_group_editor, _node.node_location); break;
			default : return nullptr;
		}

		return nullptr;
	}
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	int  open_file_stream(std::string filename, int write_mode) {
		if (write_mode == OVER_WRITE_FILE) {
			stream.open(filename, std::ios::out | std::ios::trunc);// open for output (std::ios::out) and overwrite (std::ios::trunc)

			if (!stream) return false;// Must have QIODevice::Text or ENDL newline not written !!
		}
		else
			if (!stream.is_open()) return false;

		return true;
	}

	void close_file_stream() {
		stream.close();
	}

	// ############## Export group node data ##################
	bool export_node_group(std::string file_pathname, node_group_class *node_group, scene_node_editor_class *node_group_editor) {
////QMessageBox::information(NULL, "", "in save_working_generated_function", //QMessageBox::Ok);

		if (file_pathname.size() == 0) {
			vwDialogs::display_error_message("Export Node", "ERROR : No file name defined to save data to \n Save Static Generated Function aborted\n");	
			return false;
		}

		if (!open_file_stream(file_pathname, OVER_WRITE_FILE)) { 
			vwDialogs::display_error_message("Export Node group", "ERROR : Could not open file stream to export node group data\n");	
			return false; 
		}

		export_node_group_data(node_group, node_group_editor);

		return true;
	}

	bool export_node_group_data( node_group_class *node_group, scene_node_editor_class *node_group_editor) {
		//output group node start flag
		stream << GRAPH_NODE_GROUP_START_FLAG << ENDL;

		node_group->export_node(stream);

		for (scene_node_editor_class* child_group_editor : node_group_editor->child_groups) {
			if (!child_group_editor) continue;
			//find node for this child group
			node_basis_class *group_node = node_group_editor->graph.nodes.get_node(child_group_editor->editor_id);
			node_group_class *child_group_node = dynamic_cast<node_group_class*>(group_node);

			if(group_node && child_group_node) export_node_group_data(child_group_node, child_group_editor);
		}

		export_node_group_node_data(node_group_editor->graph.nodes);
		export_node_group_link_data(node_group_editor->graph.links);
		export_node_group_node_link_data(node_group_editor);

		//output group node end flag
		stream << GRAPH_NODE_GROUP_END_FLAG << ENDL;

		return true;
	}

	bool export_node_group_node_data(nodes_class group_graph_nodes) {
		// output node data start flag
		stream << GRAPH_NODE_START_FLAG << ENDL;
		for (node_basis_class* node : group_graph_nodes.nodes) {
			if ((node->ui_node_type.category == NODE_CATEGORY_ID_GROUP) && (node->ui_node_type.node_type == NODE_TYPE_ID_GROUP))
				continue;
			stream << GRAPH_NODE_START_DATA_FLAG << ENDL;
			node->export_node(stream);
			stream << GRAPH_NODE_END_DATA_FLAG << ENDL;
		}
		// output node data end flag
		stream << GRAPH_NODE_END_FLAG << ENDL;

		return true;
	}

	//  export_node_group_pin_data not needed as pin data is saved with node data

	bool export_node_group_link_data(links_class group_graph_links) {
		// output link data start flag
		stream << GRAPH_LINK_START_FLAG << ENDL;
			
		// ouput_link data
		for (link_struct_type link : group_graph_links.links) {
			link.export_link_data(stream);
		}

		// output link data end flag
		stream << GRAPH_LINK_END_FLAG << ENDL;

		return true;
	}

	void export_node_group_node_link_data(scene_node_editor_class *node_group_editor) {
		stream << GRAPH_LINK_NODE_INPUT_START_FLAG << std::endl;
		node_group_editor->input_link_node->export_node_input_link_data(stream);
		stream << GRAPH_LINK_NODE_INPUT_END_FLAG << std::endl;
		
		stream << GRAPH_LINK_NODE_OUTPUT_START_FLAG << std::endl;
		node_group_editor->output_link_node->export_node_output_link_data(stream);
		stream << GRAPH_LINK_NODE_OUTPUT_END_FLAG << std::endl;
	}
	// ######################################################

	// !!!!!!!!!!!!!!!!! Import group node data !!!!!!!!!!!!!!!!!!!!!!!!!!

	bool import_node_group(std::string file_pathname,scene_node_editor_class *node_group_editor, ImVec2 click_pos) {
		if (!node_group_editor) { 
			vwDialogs::display_error_message("Import Node Group", "ERROR : No editor defined to import node group into\n");		
			return false; 
		}

		if (file_pathname.size() == 0) {
			vwDialogs::display_error_message("Import Node Group", "ERROR : No file name defined to save data to \n Save Static Generated Function aborted\n");	
			return false;
		}

		std::fstream import_file(file_pathname, std::ios::in);

		if (!import_file) {
			std::string str = "FILE ERROR\nCould not read node group data file \n" + (std::string)file_pathname;
			vwDialogs::display_error_message("Import Node Group", str.c_str());
			return false;
		}

		std::string object_string = FW::filetools::read_all(import_file);

		//log_panel = log_panel;
		lines.clear(); lines.shrink_to_fit();
		lines = FW::stringtools::split(object_string, '\n');// Create a list of strings for each line in the expression code

		line_number = 0;

		return import_node_group_data(file_pathname, node_group_editor, click_pos);
	}

	bool import_node_group_data(std::string file_pathname, scene_node_editor_class *node_group_editor, ImVec2 click_pos) {
		old_new_node_ids.clear();
		old_new_group_ids.clear();
		old_new_entity_ids.clear();// Probably not needed
		old_new_pin_ids.clear();
		error_code = 0;

		while (!(FW::stringtools::contains(lines[line_number], GRAPH_NODE_GROUP_START_FLAG)))
			line_number++;

		// If have not found a defined expression return an error
		if (line_number >= lines.size()) {
			//if (log_panel != NULL) log_panel->application_log.AddLog("ERROR :Could not find object start block to import parameter data.\n");
			//std::cout << "ERROR :Could not find node group start block to import node group data.\n";
			vwDialogs::display_error_message("Import Node Group data","ERROR ::\n Could not find node group start block to import node group data.\n");
			return false;
		}

		if (lines.size() < line_number + 57) {
			//std::cout << "ERROR :: import_node_group_data has corrupted format. \n";
			vwDialogs::display_error_message("Import Node Group data","ERROR ::\n Import node group data has corrupted format\nat begining of file.");
			return false;
		}

		return read_node_group(node_group_editor, click_pos, true);

	}

	bool read_node_group(scene_node_editor_class *node_group_editor, ImVec2 click_pos,bool base_group = false) {
		if (!node_group_editor) {
			//std::cout << "node_editor_import_export_class: No node editor defined to read node group data into!\n";
			vwDialogs::display_error_message("Read node group", "node_editor_import_export_class: No node editor defined to read node group data into!\n");
			return false;
		}

		node_group_class *new_node_group = node_group_editor->create_group_node(click_pos, true);
		if (!new_node_group) {
			vwDialogs::display_error_message("Read Node Group", "ERROR :: Could not create a new group node\n");
			return false;
		}
std::cout << "node_editor_import_export_class :: read_node_group RRR :" << new_node_group->node_id << ":" << new_node_group->label << std::endl;

		if(!import_node_group_basic_attribute_data(new_node_group,node_group_editor, click_pos, base_group)){
			std::string error_message = "ERROR :: Have corrupted or out of sequence entry at line " + std::to_string(line_number) + "\n with entry  \n" + lines[line_number] +"\n";
			vwDialogs::display_error_message("Read Node Group : Attribute data", error_message,error_code);
			return false;
		}

		if (!import_node_pin_data(new_node_group, node_group_editor)) {
			std::string error_message = "ERROR :: Have corrupted or out of sequence entry at line " + std::to_string(line_number) + "\n with entry  \n" + lines[line_number] +"\n";
			vwDialogs::display_error_message("Read Node Group : pin data", error_message,error_code);
			node_group_editor->delete_group_node(new_node_group);
			return false;
		}

		scene_node_editor_class *new_group_scene_node_editor = node_group_editor->get_node_editor(new_node_group->node_id);
		if (!new_group_scene_node_editor) {
			vwDialogs::display_error_message("Read Node Group","ERROR :: Could not get the node group editor to allocate group nodes to\n");
			node_group_editor->delete_group_node(new_node_group);
			return false;
		}

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		if (FW::stringtools::contains(lines[line_number], GRAPH_NODE_GROUP_START_FLAG)) {
			read_node_group(new_group_scene_node_editor, { 0.0,0.0 });

			line_number++;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
std::cout << "node_editor_import_export_class : read_node_group ---- 000 : " << line_number << ":" << lines[line_number]  << std::endl;
			if (!FW::stringtools::contains(lines[line_number], GRAPH_NODE_GROUP_END_FLAG)) {
				//std::cout << "Import voxel import node group node data ERROR : \n Could not find end of node group data block to import node data\n";
				std::string error_message = "ERROR :: Import voxel import node group node data\nCould not find end of node group data block to import node data\n";
				error_message += std::to_string(line_number) + "\n with entry  \n" + lines[line_number] + "\n";
				vwDialogs::display_error_message("Read Node Group",error_message);

				node_group_editor->delete_group_node(new_node_group);
				return false;
			}

		} else
			line_number--;

		if(!import_node_group_nodes(new_group_scene_node_editor)) {
			//std::cout << "node_editor_import_export_class: !import_node_group_nodes(new_group_scene_node_editor)" << line_number << ":" << lines[line_number] << std::endl;
			//node_group_editor->delete_group_node(new_node_group);// Cannot use here as have random crashes of app even though it seems this function works correctly
			std::string error_message = " Have corrupted or out of sequence entry at line " + std::to_string(line_number) + "\n with entry  \n" + lines[line_number] +"\n";
			error_message+="The importation of the group node(s) may be incomplete.\n";
			vwDialogs::display_error_message("Read Node Group : Import node grtoup nodes",error_message);
			return false;
		}

		if (!import_group_node_links_data(new_group_scene_node_editor)) {
			std::cout << "node_editor_import_export_class: !import_group_node_links_data(new_group_scene_node_editor)" << line_number << ":" << lines[line_number] << std::endl;
			//node_group_editor->delete_group_node(new_node_group);// Cannot use here as have random crashes of app even though it seems this function works correctly
			std::string error_message =  " Have corrupted or out of sequence entry at line " + std::to_string(line_number) + "\n with entry  \n" + lines[line_number] +"\n";
			error_message+="The importation of the group node(s) may be incomplete.\n";
			vwDialogs::display_error_message("Read Node Group : import node group links ",error_message);
			return false;
		}

		if (!import_node_group_link_node_data(new_group_scene_node_editor)) {
			//std::cout << "node_editor_import_export_class: !import_node_group_link_node_data " << line_number << ":" << lines[line_number] << std::endl;
			//node_group_editor->delete_group_node(new_node_group);// Cannot use here as have random crashes of app even though it seems this function works correctly
			std::string error_message =  " Have corrupted or out of sequence entry at line " + std::to_string(line_number) + "\n with entry  \n" + lines[line_number] +"\n";
			error_message+="The importation of the group node(s) may be incomplete.\n";
			vwDialogs::display_error_message("Read Node Group : import node group link node ",error_message);
			return false;
		}

display_import_data(new_node_group, node_group_editor);
		return true;
	}

	bool import_node_group_nodes(scene_node_editor_class *node_group_editor) {
		graph_class graph = node_group_editor->graph;
		std::pair< node_id_type, node_id_type> old_new_id;
		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "node_editor_import_export_class: import_node_group_nodes 000 : " << line_number << ":" << lines[line_number]  << std::endl;
		if (!FW::stringtools::contains(lines[line_number], GRAPH_NODE_START_FLAG)) {
			//std::cout << "Import voxel import node group node data ERROR : \n Could not find start of node data block to import node data\n";
			vwDialogs::display_error_message("Import node group nodes : import node group links ","Could not find start of graph node data block to import node data\n");
			return false;
		}

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "node_editor_import_export_class: import_node_group_nodes 111 : " << line_number << ":" << lines[line_number]  << std::endl;
		while (!FW::stringtools::contains(lines[line_number], GRAPH_NODE_END_FLAG)) {// Have not reached end of node pin data block
			if (!FW::stringtools::contains(lines[line_number], GRAPH_NODE_START_DATA_FLAG)) { // There msut allways be at least two nodes. The input and output link nodes
				vwDialogs::display_error_message("Import node group nodes : import node group links ","Could not find graph node start of data flag to import node data\n");
				return false;
			}
//std::cout << "node_editor_import_export_class: import_node_group_nodes 222 : " << line_number << ":" << lines[line_number]  << std::endl;
			node_basis_class _node;
			if (!get_import_node_basic_attribute_data(_node)) {
				std::string error_message = "Unable to import a completed node group : Error in file entry at line "  + std::to_string(line_number) + "\n with entry  \n" + lines[line_number] +"\n";
				error_message += "The importation of the group node(s) may be incomplete.\n";
				vwDialogs::display_error_message("Import node group nodes : get import node basic attribute data",error_message);
				return false;
			}
			//node_basis_class _node = get_import_node_basic_attribute_data();
//std::cout << "node_editor_import_export_class: import_node_group_nodes 333 : " << _node.ui_node_type.category << ":"<< _node.ui_node_type.node_type <<":" << line_number << ":" << lines[line_number]  << std::endl;
			node_basis_class *new_node = import_new_node_into_editor(_node, node_group_editor);
			if (!new_node) {
				std::string error_message = "Unable to import a completed node group : Error in file entry at line "  + std::to_string(line_number) + "\n with entry  \n" + lines[line_number] +"\n";
				error_message += "The importation of the group node(s) may be incomplete.\n";
				vwDialogs::display_error_message("Import node group nodes : import new node into editor",error_message);
				return false;
			}

//std::cout << "node_editor_import_export_class: import_node_group_nodes AAA333AAAAA "  << line_number << ":" << lines[line_number] << std::endl;
			define_node_location(new_node,_node.node_location);

			// correlate old node ids to new created node ids
			std::pair< node_id_type, node_id_type> old_new_id;
			old_new_id = { _node.node_id ,new_node->node_id };
			old_new_node_ids.push_back(old_new_id);

			old_new_id = { _node.node_group_id ,new_node->node_group_id };
			old_new_group_ids.push_back(old_new_id);

			old_new_id = { _node.node_entity_id ,new_node->node_group_id };
			old_new_entity_ids.push_back(old_new_id);

//std::cout << "node_editor_import_export_class: import_node_group_nodes BBBB333BBBBB "  << line_number << ":" << lines[line_number] << std::endl;
			if (!import_node_pin_data(new_node,node_group_editor)) {
				//error message
				return false;
			}

			line_number++;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "node_editor_import_export_class: import_node_group_nodes 444 : " << line_number << ":" << lines[line_number]  << std::endl;
			if (!FW::stringtools::contains(lines[line_number], GRAPH_NODE_END_DATA_FLAG)) { // There msut allways be at least two nodes. The input and output link nodes
				vwDialogs::display_error_message("Import node group nodes","Could not find end of graph node data flag\n");
				return false;
			}

			line_number++;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		}
//std::cout << "node_editor_import_export_class: import_node_group_nodes 555 : " << line_number << ":" << lines[line_number]  << std::endl;
		if (FW::stringtools::contains(lines[line_number], GRAPH_NODE_END_FLAG))
			return true;
		else {
			vwDialogs::display_error_message("Import node group nodes","Could not find end of graph node data block\n");
			return false;
		}
	}

	bool import_group_node_links_data(scene_node_editor_class *node_group_editor) {
		graph_class graph = node_group_editor->graph;

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "node_editor_import_export_class: import_group_node_links_data 000 : " << line_number << ":" << lines[line_number] << std::endl;
		if (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_START_FLAG)) {
			//std::cout << "Import voxel import node group node data ERROR : \n Could not find start of nodes links data block to import node data\n";
			vwDialogs::display_error_message("Import group node links data","Could not find start of nodes links data block to import node data\n");
			return false;
		}

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "node_editor_import_export_class: import_group_node_links_data 111 : " << line_number << ":" << lines[line_number] << std::endl;
		while (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_END_FLAG)) {// Have not reached end of node pin data block
			if (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_START_DATA_FLAG)) { // There msut allways be at least two nodes. The input and output link nodes
				//std::cout << "node_editor_import_export_class: !FW::stringtools::contains(lines[line_number], GRAPH_LINK_START_DATA_FLAG) : " << line_number << ":" << lines[line_number] << std::endl;
				vwDialogs::display_error_message("Import group node links data","Could not find start of links data block to import node data\n");
				return false;
			}

			links_class &links = node_group_editor->graph.links;
			link_struct_type link;

			line_number++;
			//int link_id = stoi(lines[line_number]); line_number++;// !!!! Probably not needed
			//int link_from_pin_id    = stoi(lines[line_number]); line_number++;// !!!! This is the output pin id of the saved link that is no longer is valid 
			//int link_to_pin_id      = stoi(lines[line_number]); // !!!! This is the input pin id of the saved link that is no longer is valid

			int link_id ,link_from_pin_id,link_to_pin_id;
			if(!FW::stringtools::string_to_int(lines[line_number],&link_id,error_code)) return false;line_number++;// !!!! Probably not needed
			if(!FW::stringtools::string_to_int(lines[line_number],&link_from_pin_id,error_code)) return false;line_number++;// !!!! This is the output pin id of the saved link that is no longer is valid 
			if(!FW::stringtools::string_to_int(lines[line_number],&link_to_pin_id,error_code)) return false;// !!!! This is the input pin id of the saved link that is no longer is valid


			line_number++; line = FW::stringtools::truncate(line, line.size());
			if (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_END_DATA_FLAG)) { // There msut allways be at least two nodes. The input and output link nodes
				//std::cout << "node_editor_import_export_class: !FW::stringtools::contains(lines[line_number], GRAPH_LINK_END_DATA_FLAG) : " << line_number << ":" << lines[line_number] << std::endl;
				vwDialogs::display_error_message("Import group node links data","Could not find end of links data block to import node data\n");
				return false;
			}

			// Find old from pin id in the old_new_pin_id vector list
			int new_pin_from = find_new_pin_id_from_old(link_from_pin_id);
			if (graph.pins.find_pin(new_pin_from).pin_id == INVALID_ID) {
				//std::cout << "node_editor_import_export_class: graph.pins.find_pin(new_pin_from).pin_id == INVALID_ID : " << line_number << ":" << lines[line_number] << std::endl;
				vwDialogs::display_error_message("Import group node links data","Could not find output pin to connect node link to\n");
				return false;
			}
			
			// Find old to pin id in the old_new_pin_id vector list
			int new_pin_to = find_new_pin_id_from_old(link_to_pin_id);
			if (graph.pins.find_pin(new_pin_to).pin_id == INVALID_ID) {
				//std::cout << "node_editor_import_export_class: graph.pins.find_pin(new_pin_to).pin_id == INVALID_ID : " << line_number << ":" << lines[line_number] << std::endl;
				vwDialogs::display_error_message("Import group node links data","Could not find input pin to connect node link to\n");
				return false;
			}

//std::cout << "node_editor_import_export_class: import_group_node_links_data 222 !!!!! : " << line_number << ":" << lines[line_number] << ":" << new_pin_from << ":" << new_pin_to << ":" << node_group_editor->editor_id << std::endl;
			 // Create a new link with the new from and to pin ids in the editor graph
			node_group_editor->graph.links.insert(new_pin_from, new_pin_to);
//std::cout << "node_editor_import_export_class: import_group_node_links_data 333 !!!!! : "<< new_pin_from << ":" << new_pin_to << ":" << node_group_editor->graph.links.links.size() << std::endl;

			line_number++;
			line = lines[line_number];line = FW::stringtools::truncate(line, line.size());
//std::cout << "node_editor_import_export_class: import_group_node_links_data 444 !!!!! : " << line_number << ":" << lines[line_number] << std::endl;
		}

//std::cout << "node_editor_import_export_class: import_group_node_links_data 555 !!!!! : " << line_number << ":" << lines[line_number] << std::endl;
		if (FW::stringtools::contains(lines[line_number], GRAPH_LINK_END_FLAG))
			return true;
		else {
			vwDialogs::display_error_message("Import group node links data","Could not find node link end data block flag\n");
			return false;
		}
	}

	bool import_node_group_link_node_data(scene_node_editor_class *node_group_editor) {
//std::cout << "node_editor_import_export_class: import_node_group_link_node_data 000ABB : " << line_number << ":" << lines[line_number] << std::endl;
		node_basis_class *input_link_node  = node_group_editor->graph.nodes.find_node_of_category_and_type(NODE_CATEGORY_ID_GROUP, NODE_TYPE_ID_GROUP_INPUT);
		node_basis_class *output_link_node = node_group_editor->graph.nodes.find_node_of_category_and_type(NODE_CATEGORY_ID_GROUP, NODE_TYPE_ID_GROUP_OUTPUT);

		if (input_link_node == nullptr || output_link_node == nullptr) {
//std::cout << "node_editor_import_export_class: input_link_node == nullptr || output_link_node == nullptr 000ACC : " << line_number << ":" << lines[line_number] << std::endl;
			vwDialogs::display_error_message("Import node group link node data","Could not find node input and output link node data\n");
			return false;
		}

		if (node_group_editor->parent_group == NULL) {
//std::cout << "node_editor_import_export_class: node_group_editor->parent_group == NULL 000ADDD : " << line_number << ":" << lines[line_number] << std::endl;
			vwDialogs::display_error_message("Import node group link node data","Group node editor does not have a parent group\n");
			return false;
		}

		node_basis_class *group_basis_node = node_group_editor->parent_group->graph.nodes.get_node(node_group_editor->editor_id);
		if (group_basis_node == nullptr) {
//std::cout << "node_editor_import_export_class: group_basis_node == nullptr  000AEEEE : " << line_number << ":" << lines[line_number] << std::endl;
			vwDialogs::display_error_message("Import node group link node data","No group basis node exists.\n");
			return false;
		}
//std::cout << "Scene node node_editor_import_export_class : import_node_group_link_node_data 000AFFFF 3333 : " << std::endl;
		node_group_class *group_node = dynamic_cast<node_group_class*>(group_basis_node);
		if (group_node == nullptr) {
//std::cout << "node_editor_import_export_class: group_node == nullptr  000AGGGG : " << line_number << ":" << lines[line_number] << std::endl;
			vwDialogs::display_error_message("Import node group link node data","Cannot alloacate a group node to linkk to.\n");
			return false;
		}

//std::cout << "node_editor_import_export_class: import_node_group_link_node_data 000HHHH : " << input_link_node->node_id << ":" << output_link_node->node_id << ":" << group_node->group_input_links.size() << std::endl;

		if (node_group_editor->input_link_node == nullptr) {
			node_group_editor->input_link_node = dynamic_cast<group_link_node_class*>(input_link_node);
		}

		if (node_group_editor->output_link_node == nullptr) {
			node_group_editor->output_link_node = dynamic_cast<group_link_node_class*>(output_link_node);
		}

		if (node_group_editor->input_link_node == nullptr || node_group_editor->output_link_node == nullptr) {
//std::cout << "node_editor_import_export_class: input_link_node == nullptr || output_link_node == nullptr 000ACC : " << line_number << ":" << lines[line_number] << std::endl;
			vwDialogs::display_error_message("Import node group link node data","No group input or export links tpo parent editor could not be defined.\n");
			return false;
		}

//std::cout << "node_editor_import_export_class: import_node_group_link_node_data 000DDD : " << line_number << ":" << lines[line_number] << std::endl;
		// Read Input node link data

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "node_editor_import_export_class: import_node_group_link_node_data 000A : " << line_number << ":" << lines[line_number] << std::endl;
		if (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_NODE_INPUT_START_FLAG)) {
			//std::cout << "Import voxel import node group node data ERROR : \n Could not find start of nodes links data block to import node data\n";
			vwDialogs::display_error_message("Import node group link node data","No start of group input link node data block could not be found.\n");
			return false;
		}

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "node_editor_import_export_class: import_node_group_link_node_data 111A : " << line_number << ":" << lines[line_number] << std::endl;
		if (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_NODE_INPUT_END_FLAG)) {// Have not reached end of node pin data block
			if (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_NODE_START_DATA_FLAG)) { // There msut allways be at least two nodes. The input and output link nodes
				vwDialogs::display_error_message("Import node group link node data","No input link node data block could not be found.\n");
				return false;
			}

			if (node_group_editor->parent_group == nullptr) {
//std::cout << "node_editor_import_export_class: node_group_editor->parent_group == nullptr 444A : " << std::endl;
				vwDialogs::display_error_message("Import node group link node data","No group parent node editor could not be found.\n");
				return false;
			}

			line_number++;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "node_editor_import_export_class: import_node_group_link_node_data 222A : " << line_number << ":" << lines[line_number] << std::endl;
			while (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_NODE_END_DATA_FLAG)) { // There msut allways be at least two nodes. The input and output link nodes
				// read input node links data and allocate to input_link_node
				if (line.size() > 0) {// Node has output pins
					line = FW::stringtools::truncate(line, line.size());
					if (line.size() > 0) {// just to be sure
						std::vector<std::string> link_node_link_data = FW::stringtools::split(line, ':');// Create a list of strings for x:y loction coordinate
						//int input_link_input_pin_id  = stoi(link_node_link_data[0]);// Link input pin id of parent editor graph input pins
						//int input_link_output_pin_id = stoi(link_node_link_data[1]);// Link output pin id of new_group_scene_node_editor editor graph output pins

						int input_link_input_pin_id,input_link_output_pin_id;// Link output pin id of new_group_scene_node_editor editor graph output pins
						if(!FW::stringtools::string_to_int(link_node_link_data[0],&input_link_input_pin_id,error_code)) return false;// Link input pin id of parent editor graph input pins
						if(!FW::stringtools::string_to_int(link_node_link_data[1],&input_link_output_pin_id,error_code)) return false;// Link output pin id of new_group_scene_node_editor editor graph output pins

						// Find old from pin id in the old_new_pin_id vector list
						int parent_editor_input_link_input_pin_id = find_new_pin_id_from_old(input_link_input_pin_id);
						if (node_group_editor->parent_group->graph.pins.get_input_pin_index(parent_editor_input_link_input_pin_id) == INVALID_ID) {
							vwDialogs::display_error_message("Import node group link node data","Group parent editor input node input pin could not be found.\n");
							return false;
						}

						// Find old to pin id in the old_new_pin_id vector list
						int node_editor_input_link_output_pin_id = find_new_pin_id_from_old(input_link_output_pin_id);
						if (node_group_editor->graph.pins.get_output_pin_index(node_editor_input_link_output_pin_id) == INVALID_ID) {
							vwDialogs::display_error_message("Import node group link node data","Group parent editor input node output pin could not be found.\n");
							return false;
						}
//std::cout << "node_editor_import_export_class: import_node_group_link_node_data 333A : " << input_link_input_pin_id << ":" << parent_editor_input_link_input_pin_id << "::"<< input_link_output_pin_id <<":" << node_editor_input_link_output_pin_id << std::endl;
						// create a new input node link with the new input pin and new output pin ids in the editor graph
						std::pair<int, int> input_link_pin_ids = { parent_editor_input_link_input_pin_id ,node_editor_input_link_output_pin_id };
//if(node_group_editor->input_link_node == nullptr) std::cout << "node_editor_import_export_class: node_group_editor->input_link_node == nullptr 444A : " << std::endl;
						node_group_editor->input_link_node->input_links.push_back(input_link_pin_ids);

						group_node->group_input_links = node_group_editor->input_link_node->input_links;
					}
				}

				line_number++;
				line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
			}
//std::cout << "node_editor_import_export_class: import_node_group_link_node_data 555A : " << line_number << ":" << lines[line_number] << std::endl;
			if (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_NODE_END_DATA_FLAG)) { // There msut allways be at least two nodes. The input and output link nodes
				vwDialogs::display_error_message("Import node group link node data","Group link node end of data flag could not be found.\n");
				return false;
			}

			line_number++;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		}

		if (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_NODE_INPUT_END_FLAG)) {
			vwDialogs::display_error_message("Import node group link node data","Group link node end of input link data block flag could not be found.\n");
			return false;
		}

		
		// Read output node link data : Not yet tested but should work as the input does

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "node_editor_import_export_class: import_node_group_link_node_data 666A : " << line_number << ":" << lines[line_number] << std::endl;
		if (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_NODE_OUTPUT_START_FLAG)) {
			//std::cout << "Import voxel import node group node data ERROR : \n Could not find start of nodes links data block to import node data\n";
			vwDialogs::display_error_message("Import node group link node data","Could not find start of nodes links data block to import node data.\n");
			return false;
		}

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "node_editor_import_export_class: import_node_group_link_node_data 777A : " << line_number << ":" << lines[line_number] << std::endl;
		if (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_NODE_OUTPUT_END_FLAG)) {// Have not reached end of node pin data block
			if (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_NODE_START_DATA_FLAG)) { // There msut allways be at least two nodes. The input and output link nodes
				vwDialogs::display_error_message("Import node group link node data","Could not find start of nodes output link data block flag.\n");
				return false;
			}

			line_number++;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "node_editor_import_export_class: import_node_group_link_node_data 888A : " << line_number << ":" << lines[line_number] << std::endl;
			while (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_NODE_END_DATA_FLAG)) { // There msut allways be at least two nodes. The input and output link nodes

				// read output node links data and allocate to input_link_node
				if (line.size() > 0) {// Node has output pins
					line = FW::stringtools::truncate(line, line.size());
					if (line.size() > 0) {// just to be sure
						std::vector<std::string> link_node_link_data = FW::stringtools::split(line, ':');// Create a list of strings for x:y loction coordinate
						//int output_link_input_pin_id  = stoi(link_node_link_data[0]);// Link input pin id of parent editor graph input pins
						//int output_link_output_pin_id = stoi(link_node_link_data[1]);// Link output pin id of new_group_scene_node_editor editor graph output pins

						int output_link_input_pin_id,output_link_output_pin_id,error_code;// Link output pin id of new_group_scene_node_editor editor graph output pins
						if(!FW::stringtools::string_to_int(link_node_link_data[0],&output_link_input_pin_id,error_code)) return false;// Link input pin id of parent editor graph input pins
						if(!FW::stringtools::string_to_int(link_node_link_data[1],&output_link_output_pin_id,error_code)) return false;// Link output pin id of new_group_scene_node_editor editor graph output pins

						// Find old from pin id in the old_new_pin_id vector list
						int parent_editor_output_link_input_pin_id = find_new_pin_id_from_old(output_link_input_pin_id);
						if (node_group_editor->graph.pins.get_input_pin_index(parent_editor_output_link_input_pin_id) == INVALID_ID) {
							vwDialogs::display_error_message("Import node group link node data","Group parent editor output node input pin could be found.\n");
							return false;
						}

						// Find old to pin id in the old_new_pin_id vector list
						int node_editor_output_link_output_pin_id = find_new_pin_id_from_old(output_link_output_pin_id);
						if (node_group_editor->parent_group->graph.pins.get_output_pin_index(node_editor_output_link_output_pin_id) == INVALID_ID) {
							vwDialogs::display_error_message("Import node group link node data","Group parent editor output node output pin could be found.\n");
							return false;
						}
//std::cout << "node_editor_import_export_class: import_node_group_link_node_data 999A : " << output_link_input_pin_id << ":" << parent_editor_output_link_input_pin_id << "::"<< output_link_output_pin_id <<":" << node_editor_output_link_output_pin_id << std::endl;
						// create a new input node link with the new input pin and new output pin ids in the editor graph
						std::pair<int, int> output_link_pin_ids = { parent_editor_output_link_input_pin_id ,node_editor_output_link_output_pin_id };
//if(node_group_editor->output_link_node == nullptr) std::cout << "node_editor_import_export_class: node_group_editor->output_link_node == nullptr 1000A : " << std::endl;
						node_group_editor->output_link_node->output_links.push_back(output_link_pin_ids);

						group_node->group_output_links = node_group_editor->output_link_node->output_links;// !!!!!!!!!! This needs to be tested but should work !!!!!!!!!!!!
					}
				}

				line_number++;
				line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
			}

			if (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_NODE_END_DATA_FLAG)) { // There msut allways be at least two nodes. The input and output link nodes
				vwDialogs::display_error_message("Import node group link node data","Group link node end of data flag could not be found.\n");
				return false;
			}

			line_number++;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		}		

//std::cout << "node_editor_import_export_class: import_node_group_link_node_data 2000A : " << line_number << ":" << lines[line_number] << std::endl;
		if (FW::stringtools::contains(lines[line_number], GRAPH_LINK_NODE_OUTPUT_END_FLAG))
			return true;
		else {
			vwDialogs::display_error_message("Import node group link node data","Group link node end of output link data block flag could not be found.\n");
			return false;
		}

		//return false;
	}

	//node_basis_class get_import_node_basic_attribute_data() {
	bool get_import_node_basic_attribute_data(node_basis_class &new_node) {
		//node_basis_class new_node;
		std::pair< node_id_type, node_id_type> old_new_id;
		line_number++;
		// Node general data

		//new_node.ui_node_type.category = stoi(lines[line_number]); line_number++;
		//new_node.ui_node_type.node_type = stoi(lines[line_number]); line_number++;

		//new_node.visible = stoi(lines[line_number]); line_number++;

		if(!FW::stringtools::string_to_int(lines[line_number],&new_node.ui_node_type.category,error_code)) return false;line_number++;
		if(!FW::stringtools::string_to_int(lines[line_number],&new_node.ui_node_type.node_type,error_code)) return false;line_number++;
		if(!FW::stringtools::string_to_bool(lines[line_number],&new_node.visible,error_code)) return false;line_number++;

		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		new_node.label = line; line_number++;

		//new_node.node_id = stoi(lines[line_number]); line_number++;// !!!! This is the node id of the saved node that is no longer is valid
		if(!FW::stringtools::string_to_int(lines[line_number],&new_node.node_id,error_code)) return false;line_number++;// !!!! This is the node id of the saved node that is no longer is valid 

		//new_node.node_group_id = stoi(lines[line_number]); line_number++;// !!!! This is the node group id of the saved node that is no longer is valid 
		//new_node.node_entity_id = stoi(lines[line_number]); line_number++;// !!!! This is the node entity id of the saved node that is no longer is valid

		if(!FW::stringtools::string_to_int(lines[line_number],&new_node.node_group_id,error_code)) return false;line_number++;// !!!! This is the node group id of the saved node that is no longer is valid 
		if(!FW::stringtools::string_to_int(lines[line_number],&new_node.node_entity_id,error_code)) return false;line_number++;// !!!! This is the node entity id of the saved node that is no longer is valid
		
		//new_node.node_entity_category_id = stoi(lines[line_number]); line_number++;
		//new_node.node_entity_type_id = stoi(lines[line_number]); line_number++;
		if(!FW::stringtools::string_to_int(lines[line_number],&new_node.node_entity_category_id,error_code)) return false;line_number++;
		if(!FW::stringtools::string_to_int(lines[line_number],&new_node.node_entity_type_id,error_code)) return false;line_number++;

		// Node editor grid location
		line = lines[line_number];line = FW::stringtools::truncate(line, line.size());
		std::vector<std::string> editor_grid_coord_s = FW::stringtools::split(line, ':');// Create a list of strings for x:y loction coordinate
		//ImVec2 loc = { stof(editor_grid_coord_s[0]),stof(editor_grid_coord_s[1]) };
		ImVec2 loc;
		if(!FW::stringtools::string_to_float(editor_grid_coord_s[0],&loc.x,error_code)) return false;
		if(!FW::stringtools::string_to_float(editor_grid_coord_s[1],&loc.y,error_code)) return false;

		new_node.node_location = loc;

		return true;
	}

	void define_node_location(node_basis_class *new_node,  ImVec2 click_pos, bool base_node = false) {
		if (new_node->graph) {
			if (!base_node) {			
				new_node->graph->node_editor->SetNodeGridSpacePos(new_node->node_id, click_pos);
				new_node->node_location = click_pos;
			}
			else {
				new_node->graph->node_editor->SetNodeScreenSpacePos(new_node->node_id, click_pos);
				new_node->node_location = click_pos;
			}
		}
	}

	bool import_node_group_basic_attribute_data(node_basis_class *new_node,scene_node_editor_class *node_group_editor,ImVec2 click_pos, bool base_node = false) {
		std::pair< node_id_type, node_id_type> old_new_id;
		line_number++;
		// Node general data

		//new_node->ui_node_type.category = stoi(lines[line_number]); line_number++;
		//new_node->ui_node_type.node_type = stoi(lines[line_number]); line_number++;

		//new_node->visible = stoi(lines[line_number]); line_number++;

		if(!FW::stringtools::string_to_int(lines[line_number],&new_node->ui_node_type.category,error_code)) return false;line_number++;
		if(!FW::stringtools::string_to_int(lines[line_number],&new_node->ui_node_type.node_type,error_code)) return false;line_number++;
		if(!FW::stringtools::string_to_bool(lines[line_number],&new_node->visible,error_code)) return false;line_number++;

		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		new_node->label   = line; line_number++;

		node_id_type old_node_id, old_group_id, old_node_entity_id;
		//node_id_type old_node_id		  = stoi(lines[line_number]); line_number++;// !!!! This is the node id of the saved node that is no longer is valid 
		if(!FW::stringtools::string_to_int(lines[line_number],&old_node_id,error_code)) return false;line_number++;// !!!! This is the node id of the saved node that is no longer is valid 
		old_new_id = { old_node_id ,new_node->node_id };
		old_new_node_ids.push_back(old_new_id);
		//node_id_type old_group_id		  = stoi(lines[line_number]); line_number++;// !!!! This is the node group id of the saved node that is no longer is valid 
		if(!FW::stringtools::string_to_int(lines[line_number],&old_group_id,error_code)) return false;line_number++;// !!!! This is the node group id of the saved node that is no longer is valid 
		old_new_id = { old_group_id ,new_node->node_group_id };
		old_new_group_ids.push_back(old_new_id);
		new_node->node_group_id = old_group_id;// *****

		//node_id_type old_node_entity_id   = stoi(lines[line_number]); line_number++;// !!!! This is the node entity id of the saved node that is no longer is valid 
		if(!FW::stringtools::string_to_int(lines[line_number],&old_node_entity_id,error_code)) return false;line_number++;// !!!! This is the node entity id of the saved node that is no longer is valid 
		old_new_id = { old_node_entity_id ,new_node->node_group_id };
		old_new_entity_ids.push_back(old_new_id);
		new_node->node_entity_id = old_node_entity_id; // *****
		//new_node->node_entity_category_id = stoi(lines[line_number]); line_number++;
		//new_node->node_entity_type_id     = stoi(lines[line_number]); line_number++;

		if(!FW::stringtools::string_to_int(lines[line_number],&new_node->node_entity_category_id,error_code)) return false;line_number++;
		if(!FW::stringtools::string_to_int(lines[line_number],&new_node->node_entity_type_id,error_code)) return false;line_number++;

		// Node editor grid location
		line = lines[line_number];
		if (new_node->graph) {
			if (!base_node) {
				if (line.size() > 0) {// Node has output pins
					line = FW::stringtools::truncate(line, line.size());
					if (line.size() > 0) {// just to be sure
						std::vector<std::string> editor_grid_coord_s = FW::stringtools::split(line, ':');// Create a list of strings for x:y loction coordinate
						//ImVec2 loc = { stof(editor_grid_coord_s[0]),stof(editor_grid_coord_s[1]) };
						ImVec2 loc;
						if(!FW::stringtools::string_to_float(editor_grid_coord_s[0],&loc.x,error_code)) return false;
						if(!FW::stringtools::string_to_float(editor_grid_coord_s[1],&loc.y,error_code)) return false;
						new_node->graph->node_editor->SetNodeGridSpacePos(new_node->node_id, loc);
						new_node->node_location = loc;
					}
				}
			}
			else {
				new_node->graph->node_editor->SetNodeScreenSpacePos(new_node->node_id, click_pos);
				new_node->node_location = click_pos;
			}
		}

		return true;
	}

	bool import_node_pin_data(node_basis_class *new_node,scene_node_editor_class *node_group_editor) {
		if (!import_node_input_pin_data(new_node, node_group_editor)) return false;
		return import_node_output_pin_data(new_node, node_group_editor);
	}

	bool import_node_input_pin_data(node_basis_class *new_node, scene_node_editor_class* node_group_editor) {
		std::pair< node_id_type, node_id_type> old_new_id;

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "node_editor_import_export_class: import_node_input_pin_data 000 " << line_number << ":" << lines[line_number]  << std::endl;
		if (!FW::stringtools::contains(lines[line_number], GRAPH_INPUT_PIN_START_FLAG)) {
			//std::cout << "Import voxel import node group pin data ERROR : \n Could not find start of pin data block to import node pin data\n";
			std::string error_message = "ERROR :: Import node group pin data\nCould not find start of pin data block to import node pin data\n";
			vwDialogs::display_error_message("Import node input pin data", error_message,error_code);
			return false;
		}
//std::cout << "node_editor_import_export_class: import_node_input_pin_data 111 " << line_number << ":" << lines[line_number]  << std::endl;
		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		while (!FW::stringtools::contains(lines[line_number], GRAPH_INPUT_PIN_END_FLAG)) {// Have not reached end of node pin data block
//std::cout << "node_editor_import_export_class: import_node_input_pin_data 333 " << std::endl;
			if (!FW::stringtools::contains(lines[line_number], GRAPH_PIN_START_DATA_FLAG)) { //Must Have pin data to read 
				//std::cout << "Import voxel import node group pin data ERROR : \n Could not find start of pin data to import node pin data\n";
				std::string error_message = "ERROR :: Import node group pin data\nCould not find start of pin data block to import node pin data\n";
				vwDialogs::display_error_message("Import node input pin data", error_message,error_code);
				return false;
			}
//std::cout << "node_editor_import_export_class: import_node_input_pin_data 444 " << line_number << ":" << lines[line_number]  << std::endl;
			pins_class &pins = node_group_editor->graph.pins;
			pin_struct_type pin;

			line_number++;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
			pin.label = line; line_number++;
			//int pin_type = stoi(lines[line_number]); line_number++;
			int pin_type;
			if(!FW::stringtools::string_to_int(lines[line_number],&pin_type,error_code)) return false;line_number++;

			pin.pin_type = static_cast<pin_type_enum>(pin_type);

			//pin.pin_value_type.category   = stoi(lines[line_number]); line_number++;
			//pin.pin_value_type.value_type = stoi(lines[line_number]); line_number++;
			//pin.context                   = stoi(lines[line_number]); line_number++;
			//pin.data_type                 = stoi(lines[line_number]); line_number++;
			//int old_pin_id                = stoi(lines[line_number]); line_number++;
			//int old_node_id               = stoi(lines[line_number]); line_number++;

			int old_pin_id, old_node_id;

			if(!FW::stringtools::string_to_int(lines[line_number],&pin.pin_value_type.category,error_code)) return false;line_number++;
			if(!FW::stringtools::string_to_int(lines[line_number],&pin.pin_value_type.value_type,error_code)) return false;line_number++;
			if(!FW::stringtools::string_to_int(lines[line_number],&pin.context,error_code)) return false;line_number++;
			if(!FW::stringtools::string_to_int(lines[line_number],&pin.data_type,error_code)) return false;line_number++;
			if(!FW::stringtools::string_to_int(lines[line_number],&old_pin_id,error_code)) return false;line_number++;
			if(!FW::stringtools::string_to_int(lines[line_number],&old_node_id,error_code)) return false;line_number++;

			pin.pin_id  = old_pin_id;
			pin.node_id = old_node_id;

			// Pin Drawing attributes 
			std::vector<std::string> s_data;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size()); line_number++;
			s_data = FW::stringtools::split(line, ':');
			
			float r, g, b, a;
			if(!FW::stringtools::string_to_float(s_data[0],&r,error_code)) return false;
			if(!FW::stringtools::string_to_float(s_data[1],&g,error_code)) return false;
			if(!FW::stringtools::string_to_float(s_data[2],&b,error_code)) return false;
			if(!FW::stringtools::string_to_float(s_data[3],&a,error_code)) return false;
			pin.pin_color = { r,g,b,a};
			//pin.pin_color = { stof(s_data[0]),stof(s_data[1]), stof(s_data[2]), stof(s_data[3]) };

			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size()); line_number++;
			s_data = FW::stringtools::split(line, ':');
			if(!FW::stringtools::string_to_float(s_data[0],&r,error_code)) return false;
			if(!FW::stringtools::string_to_float(s_data[1],&g,error_code)) return false;
			if(!FW::stringtools::string_to_float(s_data[2],&b,error_code)) return false;
			if(!FW::stringtools::string_to_float(s_data[3],&a,error_code)) return false;
			pin.pin_hovered_color = { r,g,b,a};
			//pin.pin_hovered_color = { stof(s_data[0]),stof(s_data[1]), stof(s_data[2]), stof(s_data[3]) };

			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size()); line_number++;
			s_data = FW::stringtools::split(line, ':');
			if(!FW::stringtools::string_to_float(s_data[0],&r,error_code)) return false;
			if(!FW::stringtools::string_to_float(s_data[1],&g,error_code)) return false;
			if(!FW::stringtools::string_to_float(s_data[2],&b,error_code)) return false;
			if(!FW::stringtools::string_to_float(s_data[3],&a,error_code)) return false;
			pin.pin_selected_color = { r,g,b,a};
			//pin.pin_selected_color = { stof(s_data[0]),stof(s_data[1]), stof(s_data[2]), stof(s_data[3]) };

			//int pin_shape = stoi(lines[line_number]); line_number++;
			int pin_shape;
			if(!FW::stringtools::string_to_int(lines[line_number],&pin_shape,error_code)) return false;line_number++;
			
			pin.pin_shape = static_cast<ImNodesPinShape_>(pin_shape);

			//ImColor          pin_color = IM_COL32(120, 30, 30, 255);
			//ImColor          pin_hovered_color = IM_COL32(200, 85, 85, 255);
			//ImColor          pin_selected_color = IM_COL32(200, 120, 120, 255);
			//ImNodesPinShape_ pin_shape = ImNodesPinShape_::ImNodesPinShape_QuadFilled;

//std::cout << "node_editor_import_export_class: import_node_input_pin_data 555 " << line_number << ":" << lines[line_number]  << std::endl;
			int new_pin_id = new_node->add_node_input_pin(pin, pins);
//std::cout << "node_editor_import_export_class: import_node_input_pin_data 555AAAA " << old_node_id  << ":" << new_node->node_id << ":" <<  std::endl;
			if (new_pin_id == INVALID_ID) return false;
			old_new_id = { old_pin_id ,new_pin_id };
			old_new_pin_ids.push_back(old_new_id);

			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
			if (!FW::stringtools::contains(lines[line_number], GRAPH_PIN_END_DATA_FLAG)) {
				//std::cout << "Import voxel import node group pin data ERROR : \n Could not find end of pin data to import node pin data\n";
				std::string error_message = "ERROR :: Import node group pin data\nCould not find end of pin data block to import node pin data\n";
				vwDialogs::display_error_message("Import node input pin data", error_message,error_code);
				return false;
			}
//std::cout << "node_editor_import_export_class: import_node_input_pin_data 666 " << line_number << ":" << lines[line_number]  << std::endl;
			line_number++;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		}

		if (FW::stringtools::contains(lines[line_number], GRAPH_INPUT_PIN_END_FLAG))
			return true;
		else {
			std::string error_message = "ERROR :: Import node group pin data\nCould not find end of input pin data block to import node pin data\n";
			vwDialogs::display_error_message("Import node input pin data", error_message,error_code);			
			return false;
		}
	}

	bool import_node_output_pin_data(node_basis_class* new_node, scene_node_editor_class* node_group_editor) {
		std::pair< node_id_type, node_id_type> old_new_id;

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "node_editor_import_export_class: import_node_output_pin_data 000 " << line_number << ":" << lines[line_number]  << std::endl;
		if (!FW::stringtools::contains(lines[line_number], GRAPH_OUTPUT_PIN_START_FLAG)) {
			//std::cout << "Import voxel import node group pin data ERROR : \n Could not find start of pin data block to import node pin data\n";
			std::string error_message = "ERROR :: Import node group pin data\nCould not find start of pin data block to import node pin data\n";
			vwDialogs::display_error_message("Import node output pin data", error_message,error_code);
			return false;
		}
//std::cout << "node_editor_import_export_class: import_node_output_pin_data 111" << line_number << ":" << lines[line_number]  << std::endl;
		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		while (!FW::stringtools::contains(lines[line_number], GRAPH_OUTPUT_PIN_END_FLAG)) {// Have not reached end of node pin data block
//std::cout << "node_editor_import_export_class: import_node_output_pin_data 333" << std::endl;
			if (!FW::stringtools::contains(lines[line_number], GRAPH_PIN_START_DATA_FLAG)) { //Must Have pin data to read 
				//std::cout << "Import voxel import node group pin data ERROR : \n Could not find start of pin data to import node pin data\n";
				std::string error_message = "ERROR :: Import node group pin data\nCould not find start of pin data block to import node pin data\n";
				vwDialogs::display_error_message("Import node output pin data", error_message,error_code);
				return false;
			}
//std::cout << "node_editor_import_export_class: import_node_output_pin_data 444" << line_number << ":" << lines[line_number]  << std::endl;
			pins_class &pins = node_group_editor->graph.pins;
			pin_struct_type pin;

			line_number++;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
			pin.label = line; line_number++;
			//int pin_type = stoi(lines[line_number]); line_number++;
			int pin_type;
			if(!FW::stringtools::string_to_int(lines[line_number],&pin_type,error_code)) return false;line_number++;

			pin.pin_type = static_cast<pin_type_enum>(pin_type);

			//pin.pin_value_type.category   = stoi(lines[line_number]); line_number++;
			//pin.pin_value_type.value_type = stoi(lines[line_number]); line_number++;
			//pin.context                   = stoi(lines[line_number]); line_number++;
			//pin.data_type                 = stoi(lines[line_number]); line_number++;
			//int old_pin_id                = stoi(lines[line_number]); line_number++;
			//int old_node_id               = stoi(lines[line_number]); line_number++;

			int old_pin_id, old_node_id;

			if(!FW::stringtools::string_to_int(lines[line_number],&pin.pin_value_type.category,error_code)) return false;line_number++;
			if(!FW::stringtools::string_to_int(lines[line_number],&pin.pin_value_type.value_type,error_code)) return false;line_number++;
			if(!FW::stringtools::string_to_int(lines[line_number],&pin.context,error_code)) return false;line_number++;
			if(!FW::stringtools::string_to_int(lines[line_number],&pin.data_type,error_code)) return false;line_number++;
			if(!FW::stringtools::string_to_int(lines[line_number],&old_pin_id,error_code)) return false;line_number++;
			if(!FW::stringtools::string_to_int(lines[line_number],&old_node_id,error_code)) return false;line_number++;

			pin.pin_id  = old_pin_id;
			pin.node_id = old_node_id;

			// Pin Drawing attributes 
			std::vector<std::string> s_data;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size()); line_number++;
			s_data = FW::stringtools::split(line, ':');
			//pin.pin_color = { stof(s_data[0]),stof(s_data[1]), stof(s_data[2]), stof(s_data[3])};

			//line = lines[line_number]; line = FW::stringtools::truncate(line, line.size()); line_number++;
			//s_data = FW::stringtools::split(line, ':');
			//pin.pin_hovered_color = { stof(s_data[0]),stof(s_data[1]), stof(s_data[2]), stof(s_data[3]) };

			//line = lines[line_number]; line = FW::stringtools::truncate(line, line.size()); line_number++;
			//s_data = FW::stringtools::split(line, ':');
			//pin.pin_selected_color = { stof(s_data[0]),stof(s_data[1]), stof(s_data[2]), stof(s_data[3]) };

			float r, g, b, a;
			if(!FW::stringtools::string_to_float(s_data[0],&r,error_code)) return false;
			if(!FW::stringtools::string_to_float(s_data[1],&g,error_code)) return false;
			if(!FW::stringtools::string_to_float(s_data[2],&b,error_code)) return false;
			if(!FW::stringtools::string_to_float(s_data[3],&a,error_code)) return false;
			pin.pin_color = { r,g,b,a};
			//pin.pin_color = { stof(s_data[0]),stof(s_data[1]), stof(s_data[2]), stof(s_data[3]) };

			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size()); line_number++;
			s_data = FW::stringtools::split(line, ':');
			if(!FW::stringtools::string_to_float(s_data[0],&r,error_code)) return false;
			if(!FW::stringtools::string_to_float(s_data[1],&g,error_code)) return false;
			if(!FW::stringtools::string_to_float(s_data[2],&b,error_code)) return false;
			if(!FW::stringtools::string_to_float(s_data[3],&a,error_code)) return false;
			pin.pin_hovered_color = { r,g,b,a};
			//pin.pin_hovered_color = { stof(s_data[0]),stof(s_data[1]), stof(s_data[2]), stof(s_data[3]) };

			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size()); line_number++;
			s_data = FW::stringtools::split(line, ':');
			if(!FW::stringtools::string_to_float(s_data[0],&r,error_code)) return false;
			if(!FW::stringtools::string_to_float(s_data[1],&g,error_code)) return false;
			if(!FW::stringtools::string_to_float(s_data[2],&b,error_code)) return false;
			if(!FW::stringtools::string_to_float(s_data[3],&a,error_code)) return false;
			pin.pin_selected_color = { r,g,b,a};
			//pin.pin_selected_color = { stof(s_data[0]),stof(s_data[1]), stof(s_data[2]), stof(s_data[3]) };

			//int pin_shape = stoi(lines[line_number]); line_number++;
			int pin_shape;
			if(!FW::stringtools::string_to_int(lines[line_number],&pin_shape,error_code)) return false;line_number++;

			pin.pin_shape = static_cast<ImNodesPinShape_>(pin_shape);

//std::cout << "node_editor_import_export_class: import_node_output_pin_data 555 :" << line_number << ":" << lines[line_number]  << std::endl;
			int new_pin_id = new_node->add_node_output_pin(pin, pins);
			if (new_pin_id == INVALID_ID) return false;
			old_new_id = { old_pin_id ,new_pin_id };
			old_new_pin_ids.push_back(old_new_id);

			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
			if (!FW::stringtools::contains(lines[line_number], GRAPH_PIN_END_DATA_FLAG)) {
				//std::cout << "Import voxel import node group pin data ERROR : \n Could not find end of pin data to import node pin data\n";
				std::string error_message = "ERROR :: Import node group pin data\nCould not find end of pin data block to import node pin data\n";
				vwDialogs::display_error_message("Import node output pin data", error_message,error_code);
				return false;
			}
//std::cout << "node_editor_import_export_class: import_node_output_pin_data 666 :" << line_number << ":" << lines[line_number]  << std::endl;
			line_number++;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		}

		if (FW::stringtools::contains(lines[line_number], GRAPH_OUTPUT_PIN_END_FLAG))
			return true;
		else {
			std::string error_message = "ERROR :: Import node group pin data\nCould not find end of input pin data block to import node pin data\n";
			vwDialogs::display_error_message("Import node output pin data", error_message,error_code);	
			return false;
		}
	}

	group_link_node_class *import_link_node(scene_node_editor_class *node_group_editor, int node_type, ImVec2 click_pos) {
		group_link_node_class* group_link_node = new group_link_node_class;

		if (!group_link_node) {
			// error message
			return nullptr;
		}

		group_link_node->ui_node_type.node_type = node_type;
		group_link_node->ui_node_type.category  = NODE_CATEGORY_ID_GROUP;

		if (group_link_node->ui_node_type.node_type == NODE_TYPE_ID_GROUP_INPUT) {
			group_link_node->ui_node_type.node_data_context = NODE_GROUP_INPUT_LINK_DATA_CONTEXT;
		}
		else
			group_link_node->ui_node_type.node_data_context = NODE_GROUP_OUTPUT_LINK_DATA_CONTEXT;

		group_link_node->graph = &node_group_editor->graph;

		// !!!!!!!!!!!!! Make sure node_group_editor has been set up before proceeding !!!!!!!!!!!!!!!!!!
		node_misc_import_struct_type node_misc_import_data;
		node_misc_import_data.log_panel = node_group_editor->log_panel;
		node_misc_import_data.vw_scene  = node_group_editor->vw_scene;

		if (node_group_editor->log_panel == nullptr) {
			std::cout << "########### node_editor_import_export_class : import_link_node: node_group_editor->log_panel == nullptr.\n";
		}

		if (node_group_editor->vw_scene == nullptr) {
			std::cout << "########### node_editor_import_export_class : import_link_node: node_group_editor->vw_scene == nullptr.\n" << std::endl;
		}

//std::cout << "node_editor_import_export_class: import_link_node ABABABA !!!!! :" << line_number << ":" << lines[line_number] << ":" << group_link_node->ui_node_type.category << std::endl;
		if (node_group_editor->graph.import_node(group_link_node, click_pos, INVALID_ID, &node_misc_import_data) == INVALID_NODE_ID) {
//std::cout << "node_editor_import_export_class: import_link_node ADADDADDA !!!!! :" << std::endl;
			std::string error_message = "ERROR :: Import node group link node\nCould not import a node in the node group editor graph\n";
			vwDialogs::display_error_message("Import link node", error_message,error_code);	
			delete group_link_node;
			return nullptr;
		}// create node in editor that has no pin data

//std::cout << "node_editor_import_export_class: import_link_node ACAACACA !!!!! :" << line_number << ":" << lines[line_number] << std::endl;

		node_group_editor->display_graph_status(node_group_editor);// testing only
//std::cout << "scene_node_editor_class : import_link_node : LINE !!!! : " << line_number << ":" << lines[line_number] << std::endl;

		return group_link_node;
	}


	//import node made seperate from create node to cater for any 
	//future changes that may be needed whenimporting nodes 
	template <class T> // CRITICAL :: THIS CLASS TYPE MUST BE A DERIVATIVE NODE OF NODE_BASIS_CLASS
	T *import_node(scene_node_editor_class *node_group_editor,ImVec2 click_pos) {
		T* new_node = new T;

		if (!new_node) {
			vwDialogs::display_error_message("Import Node", "ERROR : Could not allocate memory to create node");
			return nullptr;
		}

		new_node->graph = &node_group_editor->graph;

		// !!!!!!!!!!!!! Make sure node_group_editor has been set up before proceeding !!!!!!!!!!!!!!!!!!
		node_misc_import_struct_type node_misc_import_data;
		node_misc_import_data.log_panel = node_group_editor->log_panel;
		node_misc_import_data.vw_scene  = node_group_editor->vw_scene;

//if (node_group_editor->log_panel == nullptr) {
//	std::cout << "########### node_editor_import_export_class : import_node: node_group_editor->log_panel == nullptr.\n";
//}
//
//if (node_group_editor->vw_scene == nullptr) {
//	std::cout << "########### node_editor_import_export_class : import_node: node_group_editor->vw_scene == nullptr.\n" << std::endl;
//}
// 

std::cout << "node_editor_import_export_class: import_node ABABABA !!!!! :" << line_number << ":" << lines[line_number] << ":" << new_node->ui_node_type.category << std::endl;
		if (node_group_editor->graph.import_node(new_node, click_pos, INVALID_ID, &node_misc_import_data) == INVALID_NODE_ID) {
std::cout << "node_editor_import_export_class: import_node ADADDADDA !!!!! :" << std::endl;
			vwDialogs::display_error_message("Import Node","ERROR : Failed to create an entry in the editor graph\nfor seleced node type to import\n Task aborted");
			delete new_node;
			return nullptr;
		}// create node in editor that has no pin data


		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// CRITICAL ::: IF THE IMPORT FILE IS CORRUPTED WITH MISSING LINES OR INCORECT STRING NUMBERS  THAT RENDER THE CONVERSION
		//				OF STRINGS INTO NUMBERS USING THE C++ sto* FUNCTIONS, THE APPLICATION CRASHES
		//              NEED TO HAVE IN THE IMPORTATION CODE A MEANS TO IMPORT NUMBERS SAFELY AND EXIT IF AN ERROR OCCURS
		//				A C++ IMPLEMENTATION PROBLEM USING sto* functions
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
std::cout << "node_editor_import_export_class: import_node BBBBBBB !!!!! :" << std::endl;
		if (!new_node->import_node(lines, line_number)) {
std::cout << "node_editor_import_export_class: import_node CCCCCCC !!!!! :" << std::endl;
			//delete new_node;
			
			std::string str = "Critical Failure to import node:\n Incorrect or corrupted entry \n" + lines[line_number];
			str = str + "\nat line\n" + std::to_string(line_number) + "\n";
			vwDialogs::display_error_message("Import Node", str);

			node_group_editor->graph.delete_node(new_node->node_id);
			return nullptr;
		}
			
//std::cout << "node_editor_import_export_class: import_node DDDDDDD !!!!! :" << line_number << ":" << lines[line_number] << std::endl;

node_group_editor->display_graph_status(node_group_editor);// testing only
//std::cout << "scene_node_editor_class : Total scene graph status : LINE !!!! : " << line_number << ":" << lines[line_number] << std::endl;

		return new_node;
	}

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

private:
	int error_code = 0;

};
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
		std::cout << new_node_group->visible << std::endl;
		std::cout << new_node_group->label << std::endl;
		std::cout << new_node_group->ui_node_type.category << std::endl;
		std::cout << new_node_group->ui_node_type.node_type << std::endl;

		std::cout << new_node_group->node_id << std::endl;
		std::cout << new_node_group->node_group_id << std::endl;

		// These are for the editor to use to display node entity parameter UI
		std::cout <<  new_node_group->node_entity_id << std::endl;
		std::cout <<  new_node_group->node_entity_category_id << std::endl;
		std::cout <<  new_node_group->node_entity_type_id << std::endl;

		for (int input_pin_id : new_node_group->inputs) {
			std::cout << input_pin_id << ":";
		}
		std::cout << std::endl;

		for (int output_pin_id : new_node_group->outputs) {
			std::cout << output_pin_id << ":";
		}
		std::cout << std::endl;

		ImVec2 node_grid_space_pos = new_node_group->graph->node_editor->GetNodeGridSpacePos(new_node_group->node_id);
		std::cout << node_grid_space_pos.x << ":" << node_grid_space_pos.y << std::endl;
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
			case NODE_TYPE_ID_HCP_VOXEL_AUTOMATA   : return import_node<hcp_voxel_automata_node_class>(node_group_editor, _node.node_location); break;
			case NODE_TYPE_ID_HCP_VOXEL_TRANSLATION: return import_node<hcp_voxel_translation_node_class>(node_group_editor, _node.node_location); break;
			case NODE_TYPE_ID_HCP_VOXEL_INTERACTION: return import_node<hcp_voxel_interaction_node_class>(node_group_editor, _node.node_location); break;
			
			
			case NODE_TYPE_ID_HEX_SURFACE             : return import_node<hex_surface_node_class>(node_group_editor, _node.node_location); break;
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
			//QMessageBox::information(NULL, "", "No file name defined to save data to \n Save Static Generated Function aborted", //QMessageBox::Ok);
			return false;
		}

		if (!open_file_stream(file_pathname, OVER_WRITE_FILE)) return false;

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
		if (!node_group_editor) return false;

		if (file_pathname.size() == 0) {
            //QMessageBox::information(NULL, "", "No file name defined to save data to \n Save Static Generated Function aborted", //QMessageBox::Ok);
			return false;
		}

		std::fstream import_file(file_pathname, std::ios::in);

		if (!import_file) {
			std::string str = " Could not read file \n" + (std::string)file_pathname;
			vwDialogs::message_box("Import voxel object :", str.c_str());
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

		while (!(FW::stringtools::contains(lines[line_number], GRAPH_NODE_GROUP_START_FLAG)))
			line_number++;

		// If have not found a defined expression return an error
		if (line_number >= lines.size()) {
			//if (log_panel != NULL) log_panel->application_log.AddLog("ERROR :Could not find object start block to import parameter data.\n");
			std::cout << "ERROR :Could not find node group start block to import node group data.\n";
			return false;
		}

		if (lines.size() < line_number + 57) {
			std::cout << "ERROR :: import_node_group_data has corrupted format. \n";
			return false;
		}

		return read_node_group(node_group_editor, click_pos, true);

	}

	bool read_node_group(scene_node_editor_class *node_group_editor, ImVec2 click_pos,bool base_group = false) {
		if (!node_group_editor) {
			std::cout << "node_editor_import_export_class: No node editor defined to read node group data into!\n";
			return false;
		}

		node_group_class *new_node_group = node_group_editor->create_group_node(click_pos, true);
		if (!new_node_group) {
			// error message
			return false;
		}

		import_node_group_basic_attribute_data(new_node_group,node_group_editor, click_pos, base_group);

		if (!import_node_pin_data(new_node_group, node_group_editor)) {
			// error message
			node_group_editor->delete_group_node(new_node_group);
			return false;
		}

		scene_node_editor_class *new_group_scene_node_editor = node_group_editor->get_node_editor(new_node_group->node_id);
		if (!new_group_scene_node_editor) {
			// error message
			node_group_editor->delete_group_node(new_node_group);
			return false;
		}

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		if (FW::stringtools::contains(lines[line_number], GRAPH_NODE_GROUP_START_FLAG)) {
			read_node_group(new_group_scene_node_editor, { 0.0,0.0 });

			line_number++;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
std::cout << "node_editor_import_export_class: read_node_group ---- 000 : " << line_number << ":" << lines[line_number]  << std::endl;
			if (!FW::stringtools::contains(lines[line_number], GRAPH_NODE_GROUP_END_FLAG)) {
				std::cout << "Import voxel import node group node data ERROR : \n Could not find end of node group data block to import node data\n";
				node_group_editor->delete_group_node(new_node_group);
				return false;
			}

		} else
			line_number--;

		if(!import_node_group_nodes(new_group_scene_node_editor)) {
			std::cout << "node_editor_import_export_class: !import_node_group_nodes(new_group_scene_node_editor)" << line_number << ":" << lines[line_number] << std::endl;
			node_group_editor->delete_group_node(new_node_group);
			return false;
		}

		if (!import_group_node_links_data(new_group_scene_node_editor)) {
			std::cout << "node_editor_import_export_class: !import_group_node_links_data(new_group_scene_node_editor)" << line_number << ":" << lines[line_number] << std::endl;
			node_group_editor->delete_group_node(new_node_group);
			return false;
		}

		if (!import_node_group_link_node_data(new_group_scene_node_editor)) {
			std::cout << "node_editor_import_export_class: !import_node_group_link_node_data " << line_number << ":" << lines[line_number] << std::endl;
			node_group_editor->delete_group_node(new_node_group);
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
			std::cout << "Import voxel import node group node data ERROR : \n Could not find start of node data block to import node data\n";
			return false;
		}

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "node_editor_import_export_class: import_node_group_nodes 111 : " << line_number << ":" << lines[line_number]  << std::endl;
		while (!FW::stringtools::contains(lines[line_number], GRAPH_NODE_END_FLAG)) {// Have not reached end of node pin data block
			if (!FW::stringtools::contains(lines[line_number], GRAPH_NODE_START_DATA_FLAG)) { // There msut allways be at least two nodes. The input and output link nodes
				//error message
				return false;
			}
//std::cout << "node_editor_import_export_class: import_node_group_nodes 222 : " << line_number << ":" << lines[line_number]  << std::endl;
			node_basis_class _node = get_import_node_basic_attribute_data();
//std::cout << "node_editor_import_export_class: import_node_group_nodes 333 : " << _node.ui_node_type.category << ":"<< _node.ui_node_type.node_type <<":" << line_number << ":" << lines[line_number]  << std::endl;
			node_basis_class *new_node = import_new_node_into_editor(_node, node_group_editor);
			if (!new_node) {
				std::cout << "node_editor_import_export_class: !new_node "  << line_number << ":" << lines[line_number] << std::endl;
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
				//error message
				return false;
			}

			line_number++;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		}
//std::cout << "node_editor_import_export_class: import_node_group_nodes 555 : " << line_number << ":" << lines[line_number]  << std::endl;
		if (FW::stringtools::contains(lines[line_number], GRAPH_NODE_END_FLAG))
			return true;
		else
			return false;
	}

	bool import_group_node_links_data(scene_node_editor_class *node_group_editor) {
		graph_class graph = node_group_editor->graph;

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "node_editor_import_export_class: import_group_node_links_data 000 : " << line_number << ":" << lines[line_number] << std::endl;
		if (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_START_FLAG)) {
			std::cout << "Import voxel import node group node data ERROR : \n Could not find start of nodes links data block to import node data\n";
			return false;
		}

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "node_editor_import_export_class: import_group_node_links_data 111 : " << line_number << ":" << lines[line_number] << std::endl;
		while (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_END_FLAG)) {// Have not reached end of node pin data block
			if (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_START_DATA_FLAG)) { // There msut allways be at least two nodes. The input and output link nodes
				std::cout << "node_editor_import_export_class: !FW::stringtools::contains(lines[line_number], GRAPH_LINK_START_DATA_FLAG) : " << line_number << ":" << lines[line_number] << std::endl;
				return false;
			}

			links_class &links = node_group_editor->graph.links;
			link_struct_type link;

			line_number++;
			int link_id = stoi(lines[line_number]); line_number++;// !!!! Probably not needed
			int link_from_pin_id    = stoi(lines[line_number]); line_number++;// !!!! This is the output pin id of the saved link that is no longer is valid 
			int link_to_pin_id      = stoi(lines[line_number]); // !!!! This is the input pin id of the saved link that is no longer is valid

			line_number++; line = FW::stringtools::truncate(line, line.size());
			if (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_END_DATA_FLAG)) { // There msut allways be at least two nodes. The input and output link nodes
				std::cout << "node_editor_import_export_class: !FW::stringtools::contains(lines[line_number], GRAPH_LINK_END_DATA_FLAG) : " << line_number << ":" << lines[line_number] << std::endl;
				return false;
			}

			// Find old from pin id in the old_new_pin_id vector list
			int new_pin_from = find_new_pin_id_from_old(link_from_pin_id);
			if (graph.pins.find_pin(new_pin_from).pin_id == INVALID_ID) {
				std::cout << "node_editor_import_export_class: graph.pins.find_pin(new_pin_from).pin_id == INVALID_ID : " << line_number << ":" << lines[line_number] << std::endl;
				return false;
			}
			
			// Find old to pin id in the old_new_pin_id vector list
			int new_pin_to = find_new_pin_id_from_old(link_to_pin_id);
			if (graph.pins.find_pin(new_pin_to).pin_id == INVALID_ID) {
				std::cout << "node_editor_import_export_class: graph.pins.find_pin(new_pin_to).pin_id == INVALID_ID : " << line_number << ":" << lines[line_number] << std::endl;
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
		else
			return false;
	}

	bool import_node_group_link_node_data(scene_node_editor_class *node_group_editor) {
//std::cout << "node_editor_import_export_class: import_node_group_link_node_data 000ABB : " << line_number << ":" << lines[line_number] << std::endl;
		node_basis_class *input_link_node  = node_group_editor->graph.nodes.find_node_of_category_and_type(NODE_CATEGORY_ID_GROUP, NODE_TYPE_ID_GROUP_INPUT);
		node_basis_class *output_link_node = node_group_editor->graph.nodes.find_node_of_category_and_type(NODE_CATEGORY_ID_GROUP, NODE_TYPE_ID_GROUP_OUTPUT);

		if (input_link_node == nullptr || output_link_node == nullptr) {
			std::cout << "node_editor_import_export_class: input_link_node == nullptr || output_link_node == nullptr 000ACC : " << line_number << ":" << lines[line_number] << std::endl;
			return false;
		}

		if (node_group_editor->parent_group == NULL) {
			std::cout << "node_editor_import_export_class: node_group_editor->parent_group == NULL 000ADDD : " << line_number << ":" << lines[line_number] << std::endl;
			return false;
		}

		node_basis_class *group_basis_node = node_group_editor->parent_group->graph.nodes.get_node(node_group_editor->editor_id);
		if (group_basis_node == nullptr) {
			std::cout << "node_editor_import_export_class: group_basis_node == nullptr  000AEEEE : " << line_number << ":" << lines[line_number] << std::endl;
			return false;
		}
//std::cout << "Scene node node_editor_import_export_class : import_node_group_link_node_data 000AFFFF 3333 : " << std::endl;
		node_group_class *group_node = dynamic_cast<node_group_class*>(group_basis_node);
		if (group_node == nullptr) {
			std::cout << "node_editor_import_export_class: group_node == nullptr  000AGGGG : " << line_number << ":" << lines[line_number] << std::endl;
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
			std::cout << "node_editor_import_export_class: input_link_node == nullptr || output_link_node == nullptr 000ACC : " << line_number << ":" << lines[line_number] << std::endl;
			return false;
		}

//std::cout << "node_editor_import_export_class: import_node_group_link_node_data 000DDD : " << line_number << ":" << lines[line_number] << std::endl;
		// Read Input node link data

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "node_editor_import_export_class: import_node_group_link_node_data 000A : " << line_number << ":" << lines[line_number] << std::endl;
		if (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_NODE_INPUT_START_FLAG)) {
			std::cout << "Import voxel import node group node data ERROR : \n Could not find start of nodes links data block to import node data\n";
			return false;
		}

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "node_editor_import_export_class: import_node_group_link_node_data 111A : " << line_number << ":" << lines[line_number] << std::endl;
		if (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_NODE_INPUT_END_FLAG)) {// Have not reached end of node pin data block
			if (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_NODE_START_DATA_FLAG)) { // There msut allways be at least two nodes. The input and output link nodes
				//error message
				return false;
			}

			if (node_group_editor->parent_group == nullptr) {
				std::cout << "node_editor_import_export_class: node_group_editor->parent_group == nullptr 444A : " << std::endl;
				// error message
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
						int input_link_input_pin_id  = stoi(link_node_link_data[0]);// Link input pin id of parent editor graph input pins
						int input_link_output_pin_id = stoi(link_node_link_data[1]);// Link output pin id of new_group_scene_node_editor editor graph output pins

						// Find old from pin id in the old_new_pin_id vector list
						int parent_editor_input_link_input_pin_id = find_new_pin_id_from_old(input_link_input_pin_id);
						if (node_group_editor->parent_group->graph.pins.get_input_pin_index(parent_editor_input_link_input_pin_id) == INVALID_ID) return false;

						// Find old to pin id in the old_new_pin_id vector list
						int node_editor_input_link_output_pin_id = find_new_pin_id_from_old(input_link_output_pin_id);
						if (node_group_editor->graph.pins.get_output_pin_index(node_editor_input_link_output_pin_id) == INVALID_ID) return false;
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
				//error message
				return false;
			}

			line_number++;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		}

		if (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_NODE_INPUT_END_FLAG))
			return false;
		
		// Read output node link data : Not yet tested but should work as the input does

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "node_editor_import_export_class: import_node_group_link_node_data 666A : " << line_number << ":" << lines[line_number] << std::endl;
		if (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_NODE_OUTPUT_START_FLAG)) {
			std::cout << "Import voxel import node group node data ERROR : \n Could not find start of nodes links data block to import node data\n";
			return false;
		}

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "node_editor_import_export_class: import_node_group_link_node_data 777A : " << line_number << ":" << lines[line_number] << std::endl;
		if (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_NODE_OUTPUT_END_FLAG)) {// Have not reached end of node pin data block
			if (!FW::stringtools::contains(lines[line_number], GRAPH_LINK_NODE_START_DATA_FLAG)) { // There msut allways be at least two nodes. The input and output link nodes
				//error message
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
						int output_link_input_pin_id  = stoi(link_node_link_data[0]);// Link input pin id of parent editor graph input pins
						int output_link_output_pin_id = stoi(link_node_link_data[1]);// Link output pin id of new_group_scene_node_editor editor graph output pins

						// Find old from pin id in the old_new_pin_id vector list
						int parent_editor_output_link_input_pin_id = find_new_pin_id_from_old(output_link_input_pin_id);
						if (node_group_editor->graph.pins.get_input_pin_index(parent_editor_output_link_input_pin_id) == INVALID_ID) return false;

						// Find old to pin id in the old_new_pin_id vector list
						int node_editor_output_link_output_pin_id = find_new_pin_id_from_old(output_link_output_pin_id);
						if (node_group_editor->parent_group->graph.pins.get_output_pin_index(node_editor_output_link_output_pin_id) == INVALID_ID) return false;
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
				//error message
				return false;
			}

			line_number++;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		}		

//std::cout << "node_editor_import_export_class: import_node_group_link_node_data 2000A : " << line_number << ":" << lines[line_number] << std::endl;
		if (FW::stringtools::contains(lines[line_number], GRAPH_LINK_NODE_OUTPUT_END_FLAG))
			return true;
		else
			return false;

		//return false;
	}

	node_basis_class get_import_node_basic_attribute_data() {
		node_basis_class new_node;
		std::pair< node_id_type, node_id_type> old_new_id;
		line_number++;
		// Node general data

		new_node.ui_node_type.category = stoi(lines[line_number]); line_number++;
		new_node.ui_node_type.node_type = stoi(lines[line_number]); line_number++;

		new_node.visible = stoi(lines[line_number]); line_number++;

		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		new_node.label = line; line_number++;

		new_node.node_id = stoi(lines[line_number]); line_number++;// !!!! This is the node id of the saved node that is no longer is valid 

		new_node.node_group_id = stoi(lines[line_number]); line_number++;// !!!! This is the node group id of the saved node that is no longer is valid 
		new_node.node_entity_id = stoi(lines[line_number]); line_number++;// !!!! This is the node entity id of the saved node that is no longer is valid 
		
		new_node.node_entity_category_id = stoi(lines[line_number]); line_number++;
		new_node.node_entity_type_id = stoi(lines[line_number]); line_number++;

		// Node editor grid location
		line = lines[line_number];line = FW::stringtools::truncate(line, line.size());
		std::vector<std::string> editor_grid_coord_s = FW::stringtools::split(line, ':');// Create a list of strings for x:y loction coordinate
		ImVec2 loc = { stof(editor_grid_coord_s[0]),stof(editor_grid_coord_s[1]) };
		new_node.node_location = loc;

		return new_node;
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

	void import_node_group_basic_attribute_data(node_basis_class *new_node,scene_node_editor_class *node_group_editor,ImVec2 click_pos, bool base_node = false) {
		std::pair< node_id_type, node_id_type> old_new_id;
		line_number++;
		// Node general data

		new_node->ui_node_type.category = stoi(lines[line_number]); line_number++;
		new_node->ui_node_type.node_type = stoi(lines[line_number]); line_number++;

		new_node->visible = stoi(lines[line_number]); line_number++;

		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		new_node->label   = line; line_number++;

		node_id_type old_node_id		  = stoi(lines[line_number]); line_number++;// !!!! This is the node id of the saved node that is no longer is valid 
		old_new_id = { old_node_id ,new_node->node_id };
		old_new_node_ids.push_back(old_new_id);
		node_id_type old_group_id		  = stoi(lines[line_number]); line_number++;// !!!! This is the node group id of the saved node that is no longer is valid 
		old_new_id = { old_group_id ,new_node->node_group_id };
		old_new_group_ids.push_back(old_new_id);
		new_node->node_group_id = old_group_id;// *****

		node_id_type old_node_entity_id   = stoi(lines[line_number]); line_number++;// !!!! This is the node entity id of the saved node that is no longer is valid 
		old_new_id = { old_node_entity_id ,new_node->node_group_id };
		old_new_entity_ids.push_back(old_new_id);
		new_node->node_entity_id = old_node_entity_id; // *****
		new_node->node_entity_category_id = stoi(lines[line_number]); line_number++;
		new_node->node_entity_type_id     = stoi(lines[line_number]); line_number++;

		// Node editor grid location
		line = lines[line_number];
		if (new_node->graph) {
			if (!base_node) {
				if (line.size() > 0) {// Node has output pins
					line = FW::stringtools::truncate(line, line.size());
					if (line.size() > 0) {// just to be sure
						std::vector<std::string> editor_grid_coord_s = FW::stringtools::split(line, ':');// Create a list of strings for x:y loction coordinate
						ImVec2 loc = { stof(editor_grid_coord_s[0]),stof(editor_grid_coord_s[1]) };
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
			std::cout << "Import voxel import node group pin data ERROR : \n Could not find start of pin data block to import node pin data\n";
			return false;
		}
//std::cout << "node_editor_import_export_class: import_node_input_pin_data 111 " << line_number << ":" << lines[line_number]  << std::endl;
		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		while (!FW::stringtools::contains(lines[line_number], GRAPH_INPUT_PIN_END_FLAG)) {// Have not reached end of node pin data block
//std::cout << "node_editor_import_export_class: import_node_input_pin_data 333 " << std::endl;
			if (!FW::stringtools::contains(lines[line_number], GRAPH_PIN_START_DATA_FLAG)) { //Must Have pin data to read 
				std::cout << "Import voxel import node group pin data ERROR : \n Could not find start of pin data to import node pin data\n";
				return false;
			}
//std::cout << "node_editor_import_export_class: import_node_input_pin_data 444 " << line_number << ":" << lines[line_number]  << std::endl;
			pins_class &pins = node_group_editor->graph.pins;
			pin_struct_type pin;

			line_number++;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
			pin.label = line; line_number++;
			int pin_type = stoi(lines[line_number]); line_number++;
			pin.pin_type = static_cast<pin_type_enum>(pin_type);

			pin.pin_value_type.category   = stoi(lines[line_number]); line_number++;
			pin.pin_value_type.value_type = stoi(lines[line_number]); line_number++;
			pin.context                   = stoi(lines[line_number]); line_number++;
			pin.data_type                 = stoi(lines[line_number]); line_number++;
			int old_pin_id                = stoi(lines[line_number]); line_number++;
			int old_node_id               = stoi(lines[line_number]); line_number++;
			pin.pin_id  = old_pin_id;
			pin.node_id = old_node_id;

			// Pin Drawing attributes 
			std::vector<std::string> s_data;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size()); line_number++;
			s_data = FW::stringtools::split(line, ':');
			pin.pin_color = { stof(s_data[0]),stof(s_data[1]), stof(s_data[2]), stof(s_data[3]) };

			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size()); line_number++;
			s_data = FW::stringtools::split(line, ':');
			pin.pin_hovered_color = { stof(s_data[0]),stof(s_data[1]), stof(s_data[2]), stof(s_data[3]) };

			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size()); line_number++;
			s_data = FW::stringtools::split(line, ':');
			pin.pin_selected_color = { stof(s_data[0]),stof(s_data[1]), stof(s_data[2]), stof(s_data[3]) };

			int pin_shape = stoi(lines[line_number]); line_number++;
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
				std::cout << "Import voxel import node group pin data ERROR : \n Could not find end of pin data to import node pin data\n";
				return false;
			}
//std::cout << "node_editor_import_export_class: import_node_input_pin_data 666 " << line_number << ":" << lines[line_number]  << std::endl;
			line_number++;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		}

		if (FW::stringtools::contains(lines[line_number], GRAPH_INPUT_PIN_END_FLAG))
			return true;
		else
			return false;
	}

	bool import_node_output_pin_data(node_basis_class* new_node, scene_node_editor_class* node_group_editor) {
		std::pair< node_id_type, node_id_type> old_new_id;
		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "node_editor_import_export_class: import_node_output_pin_data 000 " << line_number << ":" << lines[line_number]  << std::endl;
		if (!FW::stringtools::contains(lines[line_number], GRAPH_OUTPUT_PIN_START_FLAG)) {
			std::cout << "Import voxel import node group pin data ERROR : \n Could not find start of pin data block to import node pin data\n";
			return false;
		}
//std::cout << "node_editor_import_export_class: import_node_output_pin_data 111" << line_number << ":" << lines[line_number]  << std::endl;
		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		while (!FW::stringtools::contains(lines[line_number], GRAPH_OUTPUT_PIN_END_FLAG)) {// Have not reached end of node pin data block
//std::cout << "node_editor_import_export_class: import_node_output_pin_data 333" << std::endl;
			if (!FW::stringtools::contains(lines[line_number], GRAPH_PIN_START_DATA_FLAG)) { //Must Have pin data to read 
				std::cout << "Import voxel import node group pin data ERROR : \n Could not find start of pin data to import node pin data\n";
				return false;
			}
//std::cout << "node_editor_import_export_class: import_node_output_pin_data 444" << line_number << ":" << lines[line_number]  << std::endl;
			pins_class &pins = node_group_editor->graph.pins;
			pin_struct_type pin;

			line_number++;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
			pin.label = line; line_number++;
			int pin_type = stoi(lines[line_number]); line_number++;
			pin.pin_type = static_cast<pin_type_enum>(pin_type);

			pin.pin_value_type.category   = stoi(lines[line_number]); line_number++;
			pin.pin_value_type.value_type = stoi(lines[line_number]); line_number++;
			pin.context                   = stoi(lines[line_number]); line_number++;
			pin.data_type                 = stoi(lines[line_number]); line_number++;
			int old_pin_id                = stoi(lines[line_number]); line_number++;
			int old_node_id               = stoi(lines[line_number]); line_number++;
			pin.pin_id  = old_pin_id;
			pin.node_id = old_node_id;

			// Pin Drawing attributes 
			std::vector<std::string> s_data;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size()); line_number++;
			s_data = FW::stringtools::split(line, ':');
			pin.pin_color = { stof(s_data[0]),stof(s_data[1]), stof(s_data[2]), stof(s_data[3])};

			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size()); line_number++;
			s_data = FW::stringtools::split(line, ':');
			pin.pin_hovered_color = { stof(s_data[0]),stof(s_data[1]), stof(s_data[2]), stof(s_data[3]) };

			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size()); line_number++;
			s_data = FW::stringtools::split(line, ':');
			pin.pin_selected_color = { stof(s_data[0]),stof(s_data[1]), stof(s_data[2]), stof(s_data[3]) };

			int pin_shape = stoi(lines[line_number]); line_number++;
			pin.pin_shape = static_cast<ImNodesPinShape_>(pin_shape);

//std::cout << "node_editor_import_export_class: import_node_output_pin_data 555 :" << line_number << ":" << lines[line_number]  << std::endl;
			int new_pin_id = new_node->add_node_output_pin(pin, pins);
			if (new_pin_id == INVALID_ID) return false;
			old_new_id = { old_pin_id ,new_pin_id };
			old_new_pin_ids.push_back(old_new_id);

			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
			if (!FW::stringtools::contains(lines[line_number], GRAPH_PIN_END_DATA_FLAG)) {
				std::cout << "Import voxel import node group pin data ERROR : \n Could not find end of pin data to import node pin data\n";
				return false;
			}
//std::cout << "node_editor_import_export_class: import_node_output_pin_data 666 :" << line_number << ":" << lines[line_number]  << std::endl;
			line_number++;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		}

		if (FW::stringtools::contains(lines[line_number], GRAPH_OUTPUT_PIN_END_FLAG))
			return true;
		else
			return false;
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
	template <class T> // Tnis class type must be a derivative node of node_basis_class
	T *import_node(scene_node_editor_class *node_group_editor,ImVec2 click_pos) {
		T* new_node = new T;

		if (!new_node) {
			// error message
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

//std::cout << "node_editor_import_export_class: import_node ABABABA !!!!! :" << line_number << ":" << lines[line_number] << ":" << new_node->ui_node_type.category << std::endl;
		if (node_group_editor->graph.import_node(new_node, click_pos, INVALID_ID, &node_misc_import_data) == INVALID_NODE_ID) {
//std::cout << "node_editor_import_export_class: import_node ADADDADDA !!!!! :" << std::endl;
			delete new_node;
			return nullptr;
		}// create node in editor that has no pin data

		if (!new_node->import_node(lines, line_number)) {
			std::cout << "node_editor_import_export_class: import_node !new_node->import_node(lines, int& line_number)" << std::endl;
			delete new_node;
			return nullptr;
		}
			
//std::cout << "node_editor_import_export_class: import_node ACAACACA !!!!! :" << line_number << ":" << lines[line_number] << std::endl;

node_group_editor->display_graph_status(node_group_editor);// testing only
std::cout << "scene_node_editor_class : Total scene graph status : LINE !!!! : " << line_number << ":" << lines[line_number] << std::endl;

		return new_node;
	}

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

private:
};
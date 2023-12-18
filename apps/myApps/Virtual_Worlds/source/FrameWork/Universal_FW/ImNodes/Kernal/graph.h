#pragma once
#include <vector>

#include <iostream>// for testing only. comment out when not in use or delete

#include <Universal_FW/Kernal/id_key_manager.h>

#include "node.h"
#include "node_functions.h"

// ####### NOTE : IF THIS HEADER FILE IS EDITED WITHOUT COMPILING THE node_group.cpp file WILL GET LINKING ERRORS #######
// #######        COMPLILE NODE_GROUP.CPP FILE THROUGH THE SOLUTION EXPLORER AND THEN BUILD THE SOLUTION          #######

inline id_key_manager_class<unsigned int> node_id_key; // This needed as global so all nodes in all editors have a unique node id

class node_editor_basis_class;
class graph_class {
public:
	node_editor_basis_class *node_editor;

	nodes_class nodes;
	pins_class  pins;
	links_class links;

	//int  add_node(node_basis_class* node, ImVec2 click_pos, node_id_type entity_id, node_editor_basis_class *_node_editor);// This crashes the appl
	int  add_node(node_basis_class *node, ImVec2 click_pos, node_id_type entity_id) {
		if (node == NULL) return INVALID_NODE_ID;

		node->node_id = node_id_key.get_available_id_key();
std::cout << ("graph_class::add_node:: 000 %i \n", node->node_id);
		node->graph = this;//*****

		node->define_inputs(pins);
		node->define_outputs(pins);
		node->define_node(click_pos,entity_id);
		nodes.insert(node);

//printf("graph_class::add_node:: 111 %i \n", node->node_id);
		return node->node_id;
	}

	int import_node(node_basis_class *node, ImVec2 click_pos, node_id_type entity_id, void *node_misc_data = NULL) {
std::cout << "graph_class::import_node:: 000 %i \n";
		if (node == NULL) return INVALID_NODE_ID;
std::cout << "graph_class::import_node:: 111 : "<< node->node_id << std::endl;
		node->node_id = node_id_key.get_available_id_key();
std::cout << "graph_class::import_node:: 222 : "<< node->node_id << std::endl;
		node->graph = this;

		if (!node->define_import_node(click_pos, entity_id, node_misc_data)) {
			std::cout << "ERROR : Import_node : Could not define node of node type : " << node->ui_node_type.node_type << " into node editor graph." << std::endl;
			return INVALID_NODE_ID;
		}
		
		nodes.insert(node);

//printf("graph_class::add_node:: 111 %i \n", node->node_id);
		return node->node_id;
	}

	void delete_node(int node_id) {
		node_basis_class *node = nodes.get_node(node_id);
		if (node == NULL) {
std::cout <<"graph_class::delete_node:: AAAAA node == NULL : " << node_id << " : " << nodes.nodes.size() << std::endl;
			return;
		}
//std::cout <<"graph_class::delete_node:: 000 : " << node->node_id << std::endl;
		node->delete_node_links(links);
//std::cout <<"graph_class::delete_node:: 111 : " << node->node_id << std::endl;
		node->delete_node_pins(pins);
//std::cout <<"graph_class::delete_node:: 222 : " << node->node_id << std::endl;
		//node->delete_node_entity(); // Not needed as this is performed in nodes.delete_node(node_id);
//std::cout <<"graph_class::delete_node:: 333 : " << node->node_id << std::endl;
		nodes.delete_node(node_id);
//std::cout <<"graph_class::delete_node:: 444 : " << node->node_id << std::endl;
		node_id_key.assign_free_id_key(node_id);
	}

	//*****
	void delete_node_links(int node_id) {
		node_basis_class *node = nodes.get_node(node_id);
		if (node == NULL) {
//printf("graph_class::delete_node:: AAAAA node == NULL %i : %i\n", node_id, nodes.nodes.size());
			return;
		}
		node->delete_node_links(links);
	}

	//*****
	bool node_exists(int entity_id, int entity_type_id, int entity_category_id) {
		for (node_basis_class *node : nodes.nodes) {
			if (node->node_entity_id          == entity_id          &&
				node->node_entity_category_id == entity_category_id &&
				node->node_entity_type_id     == entity_type_id) {
				return true;
			}
		}
		return false;
	}

	link_struct_type find_link(int link_id) {
		return links.find_link(link_id);
	}

	link_struct_type find_link_with_start_pin(int pin_id) {
		return links.find_link_with_start_pin(pin_id);
	}

	link_struct_type find_link_with_end_pin(int pin_id) {
		return links.find_link_with_end_pin(pin_id);
	}

	void delete_link(int link_id) {
		links.delete_link(link_id);
	}

	void clear_graph() {
		for (node_basis_class *node : nodes.nodes) {
			delete_node(node->node_id);
		}
	}

protected:

private:
	//id_key_manager_class<unsigned int> node_id_key;
};

// ****** VW NEXT VERSION ******
// Node tree class to perform node tree operations
// 
// Designed at moment to select a node tree of any kind from an initial
// selected entity node and delete all nodes on a tree connected to that entity
// Not fully tested on large node trees of different node types nut functional
// on initial tests.
// The feature to me moved into graph class or as an independent class ???
// 
// Designed for virtual functions to save load specific entity categories
// Not yet implemented but for future use when have decided how to perform
// these tasks universally in a defined consistent manner. 

// 

class node_tree_class_basis {
public:

	//virtual bool create_node_tree(graph_class *graph, node_basis_class *node) {}

	bool delete_node_tree(graph_class *graph,node_basis_class *node) {
		if (node == NULL || graph == NULL) return false;

//printf("node_tree_class_basis::delete_node_tree 0000\n");
		get_node_tree_input_links(graph,node,NULL);
//printf("node_tree_class_basis::delete_node_tree 1111\n");
		get_node_tree_output_links(graph,node,NULL);

//printf("node_tree_class_basis::delete_node_tree 2222\n");
		remove_node_tree_node_duplicates();

//printf("node_tree_class_basis::delete_node_tree 33333\n");
		delete_tree_nodes(graph);

//printf("node_tree_class_basis::delete_node_tree 4444\n");
		return true;
	}

	//virtual bool save_node_tree(graph_class* graph, node_basis_class* node) {}
	//virtual bool load_node_tree(graph_class* graph) {}



private:
	std::vector<int> tree_inputs;
	std::vector<int> tree_outputs;
	std::vector<node_basis_class*> tree_nodes;


	//Recursive function to create list of all links and nodes that are inputs into the initially selected
	//root node from which the node tree is defined from.

	void get_node_tree_input_links(graph_class *graph,node_basis_class *node, node_basis_class *parent_node) {
		for (int input_link_id : node->inputs) {
			tree_inputs.push_back(input_link_id);
			link_struct_type link = graph->find_link(input_link_id);
			int from_pin_id = link.from;
			pin_struct_type from_pin = graph->pins.find_pin(from_pin_id);
			int input_node_id = from_pin.node_id;
			node_basis_class *input_node = graph->nodes.get_node(input_node_id);

//printf("node_tree_class_basis::get_node_tree_input_links EEEEE\n");
			if(input_node != NULL) get_node_tree_input_links(graph,input_node, node);
//printf("node_tree_class_basis::get_node_tree_input_links FFFFF\n");
			if(parent_node != NULL && input_node != NULL) get_node_tree_output_links(graph,input_node, node);
		}

		tree_nodes.push_back(node);
	}

	//Recursive function to create list of all links and nodes that are outputs from the initially selected
	//root node from which the node tree is defined from.

	void get_node_tree_output_links(graph_class *graph,node_basis_class *node, node_basis_class *parent_node) {
		for (int output_link_id : node->outputs) {
			tree_outputs.push_back(output_link_id);
			link_struct_type link = graph->find_link(output_link_id);
			int to_pin_id = link.to;
			pin_struct_type to_pin = graph->pins.find_pin(to_pin_id);
			int output_node_id = to_pin.node_id;
			node_basis_class *output_node = graph->nodes.get_node(output_node_id);

//printf("node_tree_class_basis::get_node_tree_output_links !!!!EEEEE\n");
			if (output_node != NULL) get_node_tree_output_links(graph,output_node, node);
//printf("node_tree_class_basis::get_node_tree_output_links !!!!FFFFF\n");
			if (parent_node != NULL && output_node != NULL) get_node_tree_input_links(graph,output_node, node);
		}

		tree_nodes.push_back(node);
	}


	// Remove all duplicate node tree elements from node tree lists
	// Esential not to have duplicate links or nodes in a list or can have
	// complications, errors or application crashing

	void remove_node_tree_node_duplicates() {
		int i = 0;
		while(i<tree_nodes.size()){
			if (have_duplicate_node(tree_nodes[i], i)) {
				tree_nodes.erase(tree_nodes.begin() + i);
			} else{
				i++;
			}
		}

	}

	bool have_duplicate_node(node_basis_class *node, int index) {
		for(int i = 0 ; i< tree_nodes.size();i++) {
			if (index != i) {
				if (node == tree_nodes[i]) {
					return true;
					break;
				}
			}
		}

		return false;
	}

	void delete_tree_nodes(graph_class *graph) {
		for (node_basis_class *node : tree_nodes) {
			graph->delete_node(node->node_id);
		}
	}

};

// ****************************
#pragma once

#include <Universal_FW/ImNodes/Kernal/graph.h>


class scene_graph_class {
public:
		
	nodes_class nodes;
	pins_class  pins;
	links_class links;

// ####### testing only ########
	void output_scene_graph_status() {
		std::cout << "scene_graph_class status : nodes " << nodes.nodes.size() << " input Pins : " << pins.input_pins.size() << " output Pins : " << pins.output_pins.size() << " Links : " << links.links.size() << std::endl;
	}
// !!!!! NODES !!!!!!

	int  add_node(node_basis_class* node, pins_class gpins) {
std::cout << "scene_graph_class : add_node 000\n";
		if (node == NULL) return INVALID_NODE_ID;

std::cout << "scene_graph_class::add_node:: 0000 node input : " << node->inputs.size() << " output Pins : " << node->outputs.size() << std::endl;
		// make a copy of the node to put into the scene_graph_class nodes storage;

		// Do not assign a base class to a storage structure that will be used
		// to perform a dynamic or other cast to access its derived inherited
		// class. An error will occur as the derived stucture or class is not
		// defined. In other words, do not use the following commented out like code
		//node_basis_class *new_node = new node_basis_class;
		//*new_node = *node;// make a copy of the node contents;
		//nodes.insert(new_node);

		nodes.insert(node);

		for (int pin_id : node->inputs) {
			pin_struct_type pin = gpins.find_pin(pin_id);
			pins.input_pins.push_back(pin);
		}

		for (int pin_id : node->outputs) {
			pin_struct_type pin = gpins.find_pin(pin_id);
			pins.output_pins.push_back(pin);
		}

		output_scene_graph_status();// testing only
		return node->node_id;
	}

	void delete_node(int node_id) {
		node_basis_class *node = nodes.get_node(node_id);
		if (node == NULL) {
std::cout << "graph_class::delete_node:: AAAAA node == NULL\n";
			return;
		}
std::cout << "scene_graph_class::delete_node:: 000 "<< node->node_id<< std::endl;
		node->delete_node_links(links);
std::cout << "scene_graph_class::delete_node:: 1111 input Pins : " << pins.input_pins.size() << " output Pins : " << pins.output_pins.size() << std::endl;
		node->delete_node_pins(pins);
//printf("graph_class::delete_node:: 222 %i \n", node->node_id);
		node->delete_node_entity();

		nodes.delete_node(node_id);
		output_scene_graph_status();// testing only
	}

	void delete_sub_graph_nodes(graph_class subgraph) {
		for (node_basis_class *node : subgraph.nodes.nodes) {
			delete_node(node->node_id);
		}
		output_scene_graph_status();// testing only
	}

// !!!!! PINS !!!!!

	void add_pin(pin_struct_type pin) {
		if (pin.pin_type == pin_type_enum::single_input || pin.pin_type == pin_type_enum::multi_input) {
			pins.add_input_pin(pin);
		} else{
			if (pin.pin_type == pin_type_enum::single_output || pin.pin_type == pin_type_enum::multi_output) {
				pins.add_output_pin(pin);
			}
		}

		output_scene_graph_status();// testing only
	}

	void delete_pin(pin_struct_type pin) {
		if (pin.pin_type == pin_type_enum::single_input || pin.pin_type == pin_type_enum::multi_input) {
			pins.delete_input_pin(pin.pin_id);
		}
		else {
			if (pin.pin_type == pin_type_enum::single_output || pin.pin_type == pin_type_enum::multi_output) {
				pins.delete_output_pin(pin.pin_id);
			}
		}
		output_scene_graph_status();// testing only
	}

// !!!! LINKS !!!!!
	void add_node_link(link_struct_type link) {
		links.links.push_back(link);
		output_scene_graph_status();// testing only
	}

	void delete_node_link(link_struct_type link) {
		links.delete_link(link.link_id);
		output_scene_graph_status();// testing only
	}

	void delete_node_link(node_id_type link_id) {
		links.delete_link(link_id);
		output_scene_graph_status();// testing only
	}

	void delete_node_links(int node_id) {
		node_basis_class *node = nodes.get_node(node_id);
		if (node == NULL) {
//printf("graph_class::delete_node:: AAAAA node == NULL %i : %i\n", node_id, nodes.nodes.size());
			return;
		}
		node->delete_node_links(links);
		output_scene_graph_status();// testing only
	}


	//graph_class               *parent_graph = nullptr;
	//std::vector<graph_class*> child_graphs;


	void create_scene_graph() {

	}

	void evaluate_scene_graph() {

	}

protected:

private:
};
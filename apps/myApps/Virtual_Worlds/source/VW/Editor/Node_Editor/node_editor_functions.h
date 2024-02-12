#pragma once

// This Class is called exclusively from the scene_node_editor.cpp file
// and used in the scene_node_editor.cpp code thus avoiding circular 
// referencing and need to have functions defined in scene_node_editor.h

//#include "../Main_Window/Widgets/scene_node_editor_widget.h"


class node_editor_evaluation_functions_class {
public:
	scene_node_editor_class* root_scene_node_editor = nullptr;

	std::vector<node_basis_class*> evaluation_tree;

	// ----------- Node tree evaluation setup -------------

	bool create_node_editor_evaluation_tree(scene_node_editor_class* scene_node_editor) {
		if (!scene_node_editor) {
			std::cout << "ERROR : Node editor from which to create and evaluate nodes is undefined.\n";
			return false;
		}

		evaluation_tree.clear();

		root_scene_node_editor = scene_node_editor->get_root_editor(scene_node_editor);
		if (!root_scene_node_editor) {
			std::cout << "ERROR : Could not find a root node editor.\n";
			return false;
		}

//std::cout << "node_editor_evaluation_functions_class : create_node_editor_evaluation_tree : Root node editor found\n";

		scene_node_editor_class* curent_scene_editor = root_scene_node_editor;
		add_root_evaluation_nodes_to_evaluation_tree(curent_scene_editor);

		std::cout << "node_editor_evaluation_functions_class : create_node_editor_evaluation_tree : evaluation_tree size :" << evaluation_tree.size() << std::endl;

		if (evaluation_tree.size() > 0)
			return true;
		else
			return false;
	}

	// Create a list of all nodes that have no input dependancies as the initial nodes that need to
	// be evaluated or to provide input values to other dependant nodes. May also be stand alone nodes
	void add_root_evaluation_nodes_to_evaluation_tree(scene_node_editor_class* curent_scene_editor) {
		if (!curent_scene_editor) return;
		for (scene_node_editor_class* child : curent_scene_editor->child_groups) {
			add_root_evaluation_nodes_to_evaluation_tree(child);
		}

		for (node_basis_class* node : curent_scene_editor->graph.nodes.nodes) {
			bool root_node = true;
			// If a node has a link to one of its input pins, then it is not a root node to start evaluations from
			// need to acount for link and group nodes
			if (node->ui_node_type.node_type == NODE_TYPE_ID_GROUP_INPUT || node->ui_node_type.node_type == NODE_TYPE_ID_GROUP_OUTPUT) { // Testing Only : comment or delete when finished and uncomment above line of code
				root_node = false;
			}

			for (int input_pin_id : node->inputs) {
				if (curent_scene_editor->graph.links.end_pin_has_link(input_pin_id)) {
					root_node = false;
					break;
				}
			}

			if (root_node) {
				evaluation_tree.push_back(node);
			}
		}
	}

	// ----------- Node tree evaluation execution -------------

	bool evaluate_node_tree() {
		std::cout << "node_editor_evaluation_functions_class  : evaluate_node_tree 0000 : " << evaluation_tree.size() << std::endl;
		if (evaluation_tree.size() < 1) return false; // Must have at least one initial node that has no dependancies of inputs

		for (node_basis_class* node : evaluation_tree) {
			std::cout << "node_editor_evaluation_functions_class : evaluate_node_tree 1111 : " << node->node_id << std::endl;
			if (!node->evaluated()) {
				perform_node_evaluation(node);
			}

			if (!node->evaluated()) {
				std::cout << "Warning : node of id : " << node->node_id << " could not be evaluated " << std::endl;
				// option to exit with false result
			}
			else //option to only evaluate dependants only if successful node evaluation
				evaluate_node_dependants(node);
		}

		return true;
	}

	void evaluate_node_dependants(node_basis_class* node) {
		std::vector<std::vector<link_struct_type>>   dependancy_links;
		std::vector<node_basis_class*>              dependancy_nodes;

		std::cout << "node_editor_evaluation_functions_class  : evaluate_node_dependants 0000 : \n";
		// Create a list of all links that correspond to the output pin of this node : ie link start pin id = node output_pin_id
		for (int output_pin_id : node->outputs) {
			std::vector<link_struct_type> pin_dependancy_links = node->graph->links.get_all_links_with_start_pin(output_pin_id);
			dependancy_links.push_back(pin_dependancy_links);
		}

		std::cout << "node_editor_evaluation_functions_class  : evaluate_node_dependants 1111 : " << dependancy_links.size() << std::endl;
		// Create a list of all node that are linked to the output pins of this node : ie link start pin id = node output_pin_id
		for (std::vector<link_struct_type> pin_dependancy_link : dependancy_links) {
			for (link_struct_type link : pin_dependancy_link) {
				for (node_basis_class* node : node->graph->nodes.nodes) {
					if (node->has_input_link(link.to)) {
						dependancy_nodes.push_back(node);
					}
				}
			}
		}

		std::cout << "node_editor_evaluation_functions_class  : evaluate_node_dependants 2222 : " << dependancy_nodes.size() << std::endl;
		for (node_basis_class* node : dependancy_nodes) {
			std::cout << "node_editor_evaluation_functions_class  : evaluate_node_dependants 3333 : " << node->node_id << std::endl;
			switch (node->ui_node_type.node_type) {
			case NODE_TYPE_ID_GROUP: perform_node_group_evaluation(node); break;
			case NODE_TYPE_ID_GROUP_INPUT: perform_node_group_link_evaluation(node); break;
			case NODE_TYPE_ID_GROUP_OUTPUT: perform_node_group_link_evaluation(node); break;
			default: perform_node_evaluation(node); break;
			}

			if (node->evaluated()) {
				evaluate_node_dependants(node);
			}
		}
	}

	void perform_node_evaluation(node_basis_class* node) {
		bool have_evaluated_inputs = true;
		std::vector<pin_struct_type> input_pins;

//std::cout << "node_editor_evaluation_functions_class  : perform_node_evaluation 0000 : \n";
		// Test to make sure all inputs to perform a node eveluation have
		// the pin value that have been evaluated and add those evaluated pins to a
		// list to access the evaluated values to be used.
		for (int input_pin_id : node->inputs) {
			link_struct_type input_link = node->graph->links.find_link_with_end_pin(input_pin_id);// Can only have one link
			pin_struct_type  input_pin = node->graph->pins.find_pin(input_link.from);

			if (!input_pin.evaluated && input_pin.pin_id != INVALID_ID) {// Have a link to an unevaluated pin 
				have_evaluated_inputs = false;
				//break;// Uncomment when testig complete.
			}

			input_pins.push_back(input_pin);
		}

		// Do not have all input values evaluated return to last call and wait for
		// the next call of an evaluated input node
		if (!have_evaluated_inputs) {
//std::cout << "node_editor_evaluation_functions_class  : perform_node_evaluation 2222 !have_evaluated_inputs: \n";
			return;
		}

		// Have all input nodes with evaluated value(s) that can now be used to
		// perform the evaluation of the current node.
//std::cout << "node_editor_evaluation_functions_class  : perform_node_evaluation 3333 : " << node->inputs.size() << std::endl;
		if (input_pins.size() == 0)
			node->evaluate();
		else
			node->evaluate(input_pins);
	}

	void perform_node_group_evaluation(node_basis_class* group_node) {
		group_node->evaluate();
		// Do ne see that group nodes need to be evaluated as group link node evaluation
		// performs the task of the input and output pin and link functions to assign
		// values for other nodes linked to the group nodes to access.
	}

	void perform_node_group_link_evaluation(node_basis_class* group_link_node) {
		group_link_node->evaluate();
	}

	bool is_duplicate_node(node_basis_class *node, std::vector<node_basis_class*> node_list) {
		for (node_basis_class* list_node : node_list) {
			if (node->node_id == list_node->node_id) return true;
			break;
		}
		return false;
	}

protected:

private:


};
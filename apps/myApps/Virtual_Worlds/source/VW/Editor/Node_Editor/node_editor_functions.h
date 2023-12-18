#pragma once

// This Class is called exclusively from the scene_node_editor.cpp file
// and used in the scene_node_editor.cpp code thus avoiding circular 
// referencing and need to have functions defined in scene_node_editor.h

//#include "../Main_Window/Widgets/scene_node_editor_widget.h"


class node_editor_evaluation_functions_class {
public:

	//scene_node_editor_class *scene_node_editor = nullptr;
	scene_node_editor_class* root_scene_node_editor = nullptr;

	std::vector<node_basis_class*> evaluation_tree;

	//bool perform_evaluation_step() {

	//}

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

std::cout << "node_editor_evaluation_functions_class : create_node_editor_evaluation_tree : Root node editor found\n";

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
			//if (node->ui_node_type.node_type == NODE_TYPE_ID_GROUP_INPUT || node->ui_node_type.node_type == NODE_TYPE_ID_GROUP_OUTPUT || node->ui_node_type.node_type != NODE_TYPE_ID_GROUP) {
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
		//std::vector<link_struct_type> input_links;

		std::cout << "node_editor_evaluation_functions_class  : perform_node_evaluation 0000 : \n";
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
			//input_links.push_back(input_link);
		}

		// Do not have all input values evaluated return to last call and wait for
		// the next call of an evaluated input node
		if (!have_evaluated_inputs) {
			std::cout << "node_editor_evaluation_functions_class  : perform_node_evaluation 2222 !have_evaluated_inputs: \n";
			return;
		}

		// Have all input nodes with evaluated value(s) that can now be used to
		// perform the evaluation of the current node.
		std::cout << "node_editor_evaluation_functions_class  : perform_node_evaluation 3333 : " << node->inputs.size() << std::endl;
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

	/*
		node_basis_class *get_linked_input_node(int input_pin, node_basis_class *node) {
			graph_class *graph = node->graph;

			link_struct_type link = graph->links.find_link_with_end_pin(input_pin);

			pin_struct_type output_node_pin = graph->pins.find_pin(link.from);

			return graph->nodes.get_node(output_node_pin.node_id);

		}
	*/

	// --------------------------------------------------------
//#define GRAPH_NODE_GROUP_START_FLAG "["
//#define GRAPH_NODE_GROUP_END_FLAG   "]"
//#define GRAPH_NODE_START_FLAG       "{"
//#define GRAPH_NODE_START_FLAG       "}"
//#define GRAPH_PIN_START_FLAG        "<"
//#define GRAPH_PIN_START_FLAG        ">"
//#define GRAPH_LINK_START_FLAG       "("
//#define GRAPH_LINK_START_FLAG       ")"


	// ********** Import / Export node tree functions ***************
/*
	bool export_node_group(scene_node_editor_class *node_group_editor) {

		//output group_node start flag

		graph_class group_graph = node_group_editor->graph;

		export_node_group_node_data(node_group_editor->graph.nodes);
		export_node_group_pin_data(node_group_editor->graph.pins);
		export_node_group_link_data(node_group_editor->graph.links);

		for (scene_node_editor_class *child_group : node_group_editor->child_groups) {
			export_node_group(child_group);
		}

		//output groupnode end flag
		return true;
	}

	bool export_node_group_node_data(nodes_class group_graph_nodes) {
		for (node_basis_class *node : group_graph_nodes.nodes) {
			// output node data start flag
			
			node->export_node();
			
			// output node data end flag


		}
		return true;
	}

	bool export_node_group_pin_data(pins_class group_graph_pins) {
		for (pin_struct_type pin : group_graph_pins.input_pins) {
			// output input pin data start flag

			// ouput input pin data

			// output input pin data end flag
		}

		for (pin_struct_type pin : group_graph_pins.output_pins) {
			// output output pin data start flag

			// ouput output pin data

			// output output pin data end flag
		}
		return true;
	}

	bool export_node_group_link_data(links_class group_graph_links) {
		for (link_struct_type link : group_graph_links.links) {
			// output link data start flag

			// ouput_link data

			// output link data end flag
		}
		return true;
	}
*/

	//std::vector<node_basis_class*> node_tree;

	//std::vector<node_basis_class*> node_tree;
/*
	bool export_node_group(scene_node_editor_class *node_group_editor) {
		std::vector<node_basis_class*> node_tree;
		if(!create_group_root_node_tree(node_group_editor, node_tree)) return false;
		return export_graph_node_tree(node_group_editor,node_tree);
	}

	bool create_group_root_node_tree(scene_node_editor_class *node_editor, std::vector<node_basis_class*> &node_tree) {
		if (!node_editor) {
			std::cout << "ERROR : Node editor from which to create and evaluate nodes is undefined.\n";
			return false;
		}

		node_tree.clear();

std::cout << "node_editor_evaluation_functions_class : export_editor_graph 0000\n";

		scene_node_editor_class *curent_scene_editor = node_editor;
		add_root_nodes_to_node_tree(curent_scene_editor, node_tree);

std::cout << "node_editor_evaluation_functions_class : export_editor_graph 1111 : evaluation_tree size :" << evaluation_tree.size() << std::endl;

		if (node_tree.size() > 0)
			return true;
		else
			return false;		
	}

	void add_root_nodes_to_node_tree(scene_node_editor_class* curent_scene_editor, std::vector<node_basis_class*> &node_tree) {
		if (!curent_scene_editor) return;
		//for (scene_node_editor_class* child : curent_scene_editor->child_groups) {
		//	add_root_nodes_to_node_tree(child);
		//}

		for (node_basis_class* node : curent_scene_editor->graph.nodes.nodes) {
			bool root_node = true;
			// If a node has a link to one of its input pins, then it is not a root node. Need to acount for link and group nodes
			if (node->ui_node_type.node_type == NODE_TYPE_ID_GROUP_OUTPUT) {
				root_node = false;
			} else {
				for (int input_pin_id : node->inputs) {
					link_struct_type link = curent_scene_editor->graph.links.find_link_with_end_pin(input_pin_id);//find if a graph link to this nodes input pins

					if (link.link_id != INVALID_ID) { // Found a link to this node input pin of input_pin_id
						//if (!curent_scene_editor->input_link_node->has_output_link(link.from)) { // This link is not a link to the group nodes input links then it is not a root node
							root_node = false;
							break;
						//}
					}
				}
			}

			if (root_node) {
				node_tree.push_back(node);
			}
		}
	}
 
	bool export_graph_node_tree(scene_node_editor_class *node_group_editor,std::vector<node_basis_class*> node_tree) {
		
		
		return true;
	}
*/
/*
	bool export_graph_node_tree(node_basis_class *node,scene_node_editor_class *scene_node_editor) {
		if (!node || !scene_node_editor) return false;

		std::vector<std::vector<link_struct_type>> dependancy_links;
		std::vector<node_basis_class*>             dependancy_nodes;

		std::vector<std::pair<link_struct_type, node_basis_class*>> node_links;

std::cout << "node_editor_evaluation_functions_class  : export_graph_node_tree 0000 : \n";
		// Create a list of all links that correspond to the output pin of this node : ie link start pin id = node output_pin_id
		for (int output_pin_id : node->outputs) {
			std::vector<link_struct_type> pin_dependancy_links = node->graph->links.get_all_links_with_start_pin(output_pin_id);
			dependancy_links.push_back(pin_dependancy_links);
		}

std::cout << "node_editor_evaluation_functions_class  : export_graph_node_tree 1111 : " << dependancy_links.size() << std::endl;
		// Create a list of all node that are linked to the output pins of this node : ie link start pin id = node output_pin_id
		for (std::vector<link_struct_type> pin_dependancy_link : dependancy_links) {
			for (link_struct_type link : pin_dependancy_link) {
				for (node_basis_class* node : node->graph->nodes.nodes) {
					if (node->has_input_link(link.to)) {
						//if (node->has_input_link(link.to) && !duplicate_node(node, dependancy_nodes)) {
							//dependancy_nodes.push_back(node);
						std::pair<link_struct_type, node_basis_class*> node_link = { link ,node };
						node_links.push_back(node_link);
					}
				}
			}
		}

std::cout << "node_editor_evaluation_functions_class  : evaluate_node_dependants 2222 : " << dependancy_nodes.size() << std::endl;
		for (std::pair<link_struct_type, node_basis_class*> node_link : node_links) {
std::cout << "node_editor_evaluation_functions_class  : evaluate_node_dependants 3333 : " << node->node_id << std::endl;

			switch (node_link.second->ui_node_type.node_type) {
				case NODE_TYPE_ID_GROUP: export_node_group_link(node_link,scene_node_editor); break;
				case NODE_TYPE_ID_GROUP_INPUT: perform_node_group_link_evaluation(node); break;
				case NODE_TYPE_ID_GROUP_OUTPUT: perform_node_group_link_evaluation(node); break;
				default: perform_node_evaluation(node); break;
			}
			node->export_node();
		}


		return true;
	}

	void export_node_group_link(std::pair<link_struct_type, node_basis_class*> node_link,scene_node_editor_class *scene_node_editor) {
		graph_class child_graph;
		group_link_node_class *input_link_node;
		std::vector<std::pair<int, int>> input_links;
		for (scene_node_editor_class *child_group : scene_node_editor->child_groups) {
			if (child_group->editor_id == node_link.second->node_id) {
				child_graph = child_group->graph;
				
				node_group_class *group_node = dynamic_cast<node_group_class*>(node_link.second);
				if(group_node == nullptr || child_group->input_link_node == nullptr) return;
				for (std::pair<int, int> input_link : child_group->input_link_node->input_links) {
					if (input_link.first == node_link.first.to) {
						// find link in child_graph with input_link.second.id == link.from
						// find node that has input pin id == link.to
						// export group and input pin data
						// export group input link data
						// export_graph_node_tree(found node,child_group);
					}
				}


			}
		}
	}
*/



	bool is_duplicate_node(node_basis_class *node, std::vector<node_basis_class*> node_list) {
		for (node_basis_class* list_node : node_list) {
			if (node->node_id == list_node->node_id) return true;
			break;
		}
		return false;
	}

	// **************************************************************
protected:

private:


};
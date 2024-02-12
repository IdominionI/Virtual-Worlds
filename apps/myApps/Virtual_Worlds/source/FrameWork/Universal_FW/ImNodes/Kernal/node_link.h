#pragma once

#include "node_editor.h"

class group_link_node_class : public node_basis_class {
public:

	std::string link_type_text = "";

	std::vector<std::pair<int, int>> input_links;  // input::output pair of parent group node input pin_id and link node output pin_id
	std::vector<std::pair<int, int>> output_links; // input::output pair of link output node input pin_id and parent group node output pin_id 
	graph_class *parent_group_node_graph = nullptr;

	bool define_inputs(pins_class& pins) {
		return define_link_node(ui_node_type.node_type);
	}

	bool define_outputs(pins_class& pins) {
		return define_link_node(ui_node_type.node_type);
	}

	bool define_link_node(int node_link_type) {
		if (node_link_type == NODE_TYPE_ID_GROUP_INPUT) {
			link_type_text = " inputs";

			//define input link node draw attributes here"
			return true;
		}

		if (node_link_type == NODE_TYPE_ID_GROUP_OUTPUT) {
			link_type_text = " outputs";

			//define output link node draw attributes here"
			return true;
		}

		return false;
	}

	bool define_node(ImVec2 click_pos, node_id_type  entity_id_) {
		label = "Node Group " + std::to_string(node_id) + link_type_text;

		// !!! CRITICAL : MUST HAVE NODE UI NODE TYPE DEFINITION DEFINED
		// !!! ELSE CANNOT LINK NODE PINS, PERFORM EVALUATIONS OR DISPLAY
		// !!! NODE PARAMETERS TO BE DISPLAYED AND MODIFIED AS WELL AS
		// !!! OTHER NODE AND NODE EDITOR FUCTIONALITY

		// !!! CRITICAL : THE LINK NODE MUST HAVE ITS ui_node_type.node_type 
		// !!!            DEFINED BEFORE CALLING define_node AS THERE ARE TWO
		// !!!            POSSIBLE VALUES

		ui_node_type.category = NODE_CATEGORY_ID_GROUP;
		if (ui_node_type.node_type == NODE_TYPE_ID_GROUP_INPUT) {
			ui_node_type.node_data_context = NODE_GROUP_INPUT_LINK_DATA_CONTEXT;
			graph->node_editor->SetNodeGridSpacePos(node_id, { 50.0,100.0 });// This creates node in ImNodes editor
			return true;
		}

		if (ui_node_type.node_type == NODE_TYPE_ID_GROUP_OUTPUT) {
			ui_node_type.node_data_context = NODE_GROUP_OUTPUT_LINK_DATA_CONTEXT;
			graph->node_editor->SetNodeGridSpacePos(node_id, { 250.0,100.0 });// This creates node in ImNodes editor
			return true;
		}

		return false;
	}

	bool define_import_node(ImVec2 click_pos, node_id_type  entity_id_, void *node_misc_data = NULL) {
		return define_node(click_pos, entity_id_);
	}

	//bool evaluated() {
		//Find group node input pin that this linked output pin is associated with
		//find link the group input  pin is assocated with
		//find node that this link has as a start pin associated with.
		// return the node output pin evaluated value
	//}

	void evaluate() {

		if (ui_node_type.node_type == NODE_TYPE_ID_GROUP_INPUT) {
			evaluate_inputs();
		}
		else 
		{
			if (ui_node_type.node_type == NODE_TYPE_ID_GROUP_OUTPUT) {
				evaluate_outputs();
			}
		}
	}

	// input link nodes do not have input pins but only output pins 
	// Link node evaluation is only to set output pins to values of node pins
	// in the group parent node links to. This means that output pins are not
	// all evaluated at the same time and thus need to be skipped if no evaluation
	// in a linked pin of a node is performed.
	void evaluate_inputs() {
		for (std::pair<int, int> input_link : input_links) {
			//if (input_link.first == INVALID_ID) continue; // Have not found an input pin in the parent group node : skip to next input link

			//Find group node input pin that this linked output pin is associated with
			pin_struct_type output_pin = graph->pins.find_pin(input_link.second);

			//find link the group input  pin is assocated with
			link_struct_type group_input_link = parent_group_node_graph->links.find_link_with_end_pin(input_link.first);
			if (group_input_link.link_id == INVALID_ID) continue;// Have not found parent group input link : skip to next input link
				//return false;

			//find node pin that this link has as a start pin associated with.
			pin_struct_type pin = parent_group_node_graph->pins.find_pin(group_input_link.from);

			// assign a copy of the linked pins value and evaluation status
			if (!pin.evaluated){
				output_pin.evaluated = pin.evaluated;
			} else {
				output_pin.value     = pin.value;
				output_pin.evaluated = pin.evaluated;
			}
		}

	}

	void evaluate_outputs() {

		for (std::pair<int, int> output_link : output_links) {
			pin_struct_type output_pin = parent_group_node_graph->pins.find_pin(output_link.second);// Get the node pin of an existing node within the group that this output lin exists within
			if (output_pin.pin_id == INVALID_ID) continue;

		//find node pin that this link has as a start pin associated with.
			link_struct_type input_link = graph->links.find_link_with_end_pin(output_link.first);
			if (input_link.link_id == INVALID_ID) continue;

			pin_struct_type pin  = graph->pins.find_pin(input_link.from);// Get the node pin of an existing node within the group that this output lin exists within
			

			// assign a copy of the linked pins value and evaluation status
			if (!pin.evaluated) {
				output_pin.evaluated = pin.evaluated;
			}
			else {
				output_pin.value = pin.value;
				output_pin.evaluated = pin.evaluated;
			}
		}
	}

	void export_node(std::fstream& stream) {

		export_mandatory_node_data(stream);// mandatory node export function that must be present in all graph node exports
		export_node_pin_data(stream);
		// specific Node data here
	}

	void export_node_input_link_data(std::fstream &stream) {
		stream << GRAPH_LINK_NODE_START_DATA_FLAG << std::endl;
		for (std::pair<int, int> input_link : input_links) {
			stream << input_link.first << ":" << input_link.second << std::endl;
		}
		stream << GRAPH_LINK_NODE_END_DATA_FLAG << std::endl;
	}

	void export_node_output_link_data(std::fstream &stream) {
		stream << GRAPH_LINK_NODE_START_DATA_FLAG << std::endl;
//std::cout << "@@@@@@@@@@node_link :: export_node_output_link_data : " << output_links.size() << std::endl;
		for (std::pair<int, int> output_link : output_links) {
//std::cout << "@@@@@@@@node_link :: export_node_output_link_data : " << output_link.first << ":" << output_link.second << std::endl;
			stream << output_link.first << ":" << output_link.second << std::endl;
		}
		stream << GRAPH_LINK_NODE_END_DATA_FLAG << std::endl;
	}
};
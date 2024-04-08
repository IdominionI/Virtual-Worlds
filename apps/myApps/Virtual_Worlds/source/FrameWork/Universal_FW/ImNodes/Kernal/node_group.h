#pragma once

#include "node.h"
#include "graph.h"

//node_draw_data_struct_type node_group_draw_data;

class node_group_class : public node_basis_class {
public:
	node_group_class() {}
	~node_group_class() {}

	std::vector<std::pair<int, int>> group_input_links;  // input::output pair of parent group node input pin_id and group input node output pin_id
	std::vector<std::pair<int, int>> group_output_links; // input::output pair of parent group output node input pin_id and group node output pin_id 

	//void *get_node_data();// required so as not to compile .cpp file so as not to get linking errors 

	std::pair<int, int> find_input_link(int input_link_id) {
		for (std::pair<int, int> group_input_link : group_input_links) {
			if (group_input_link.second == input_link_id) {
				return group_input_link;
			}
		}

		return { INVALID_ID,INVALID_ID };
	}

	int find_link_output_index(int output_link_id) {
		for (int i = 0; i < group_input_links.size();i++) {
			if (group_input_links[i].second == output_link_id) {
				return i;
			}
		}

		return INVALID_ID;
	}

	int find_link_input_index(int input_link_id) {
		for (int i = 0; i < group_output_links.size(); i++) {
			if (group_output_links[i].first == input_link_id) {
				return i;
			}
		}

		return INVALID_ID;
	}

	void delete_group_input_link(int index) {
		group_input_links.erase(group_input_links.begin() + index);
	}

	void delete_group_output_link(int index) {
		group_output_links.erase(group_output_links.begin() + index);
	}

	// virtual node basis functions to implement group node functions
	bool define_node(ImVec2 click_pos, node_id_type  entity_id_);
	bool define_import_node(ImVec2 click_pos, node_id_type  entity_id_, void* node_misc_data = NULL);
	//{
	//	return define_node(click_pos, entity_id_);
	//}

	void evaluate(std::vector<pin_struct_type> &input_pins) {
		evaluate();
	
	}

	// evaluation Testing only : Delete or comment out when finished
	void evaluate() {
std::cout << "node_group_class : Evaluate node group 0000 : " << node_id << std::endl;

		for (int output_pin_id : outputs) {
			pin_struct_type &output_pin = graph->pins.find_pin(output_pin_id);
std::cout << "node_group_class : Evaluate node group 1111 : " << output_pin.pin_id << std::endl;

			if (output_pin.pin_id == INVALID_ID) {
				std::cout << "Evaluate node group pin of id " << output_pin_id << " has no group pin !!!" << std::endl;
				continue;
			}

			output_pin.data_type = DATA_VALUE_TYPE_INT;
			output_pin.value     = new int(node_id);
			output_pin.evaluated = true;

std::cout << "node_group_class : Evaluate node group 2222 : " << output_pin.pin_id << std::endl;
		}
	}

	void export_node(std::fstream& stream) {

		export_mandatory_node_data(stream);// mandatory node export function that must be present in all graph node exports
		export_node_pin_data(stream);
		// specific Node data here
	}

protected:

private:


};


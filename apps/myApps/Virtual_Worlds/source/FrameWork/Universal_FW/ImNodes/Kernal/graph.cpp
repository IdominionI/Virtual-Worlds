/*#include "graph.h"

#include "node_editor.h"

int  graph_class::add_node(node_basis_class* node, ImVec2 click_pos, node_id_type entity_id, node_editor_basis_class *_node_editor) {
	if (node == NULL || _node_editor == NULL) return INVALID_NODE_ID;

	node->node_id = node_id_key.get_available_id_key();
std::cout << ("graph_class::add_node:: 000 %i \n", node->node_id);
	node->graph = this;//*****

	_node_editor->BeginNode(node->node_id);
	node->define_inputs(pins);
	node->define_outputs(pins);
	node->define_node(click_pos, entity_id);
	nodes.insert(node);
	_node_editor->EndNode();
//printf("graph_class::add_node:: 111 %i \n", node->node_id);
	return node->node_id;
}

int graph_class::import_node(node_basis_class* node, ImVec2 click_pos, node_id_type entity_id, node_editor_basis_class* _node_editor) {
	if (node == NULL || _node_editor == NULL) return INVALID_NODE_ID;

	node->node_id = node_id_key.get_available_id_key();
std::cout << ("graph_class::import_node:: 000 %i \n", node->node_id);
	node->graph = this;//*****

	_node_editor->BeginNode(node->node_id);
	node->define_node(click_pos, entity_id);
	nodes.insert(node);
	_node_editor->EndNode();
//printf("graph_class::add_node:: 111 %i \n", node->node_id);
	return node->node_id;
}
*/
#include "node_group.h"
#include "node_editor.h"

// Cannot have define_node function in the node_group.h file as will incur circular
// referencing errors.
bool node_group_class::define_node(ImVec2 click_pos, node_id_type  entity_id_) {
    label = "Node Group " + std::to_string(node_id);

    // !!! CRITICAL : MUST HAVE NODE UI NODE TYPE DEFINITION DEFINED
    // !!! ELSE CANNOT LINK NODE PINS, PERFORM EVALUATIONS OR DISPLAY
    // !!! NODE PARAMETERS TO BE DISPLAYED AND MODIFIED AS WELL AS
    // !!! OTHER NODE AND NODE EDITOR FUCTIONALITY 
    ui_node_type.category          = NODE_CATEGORY_ID_GROUP;
    ui_node_type.node_type         = NODE_TYPE_ID_GROUP;
    ui_node_type.node_data_context = NODE_GROUP_DATA_CONTEXT;

    node_dimensions = { 60.0f,5.0f };

    node_draw_data.node_header_color          = ImColor(70, 0, 255, 255);
    node_draw_data.node_header_hovered_color  = ImColor(70, 50, 255, 255);
    node_draw_data.node_header_selected_color = ImColor(70, 100, 255, 255);
    node_draw_data.node_header_label_color    = ImColor(255, 255, 255, 255);// ImGui does not have a color function that accepts its own ImColor type for textS

    node_draw_data.node_body_color            = ImColor(75, 75, 75, 255);
    node_draw_data.node_body_hovered_color    = ImColor(100, 100, 100, 255);
    node_draw_data.node_body_selected_color   = ImColor(150, 150, 150, 255);
    node_draw_data.node_outline_color         = ImColor(175, 175, 175, 255);

    graph->node_editor->SetNodeScreenSpacePos(node_id, click_pos);// This creates node in ImNodes editor

    return true;
}

bool node_group_class::define_import_node(ImVec2 click_pos, node_id_type  entity_id_, void* node_misc_data) {
    // Need this function in the node_group.cpp file rather than the node_group.h file otherwise will get linking
    // errors without compiling the node_group.cpp file seperatly before the whole project.
    // define_node function needed in a .cpp file to avoid cicular referencing errors with the node_editor.h
	return define_node(click_pos, entity_id_);
}

//void *node_group_class::get_node_data() { return nullptr; }//required so as not to compile .cpp file so as not to get linking errors 
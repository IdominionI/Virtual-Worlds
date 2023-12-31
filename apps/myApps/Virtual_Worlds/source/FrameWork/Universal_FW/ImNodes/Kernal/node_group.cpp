#include "node_group.h"
#include "node_editor.h"

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
    //Define Node display attributes
    //ImVec2 node_dimensions     = { 100.0f,100.0f };
    //node_header_color          = ImColor(0, 0, 139, 255);
    //node_header_hovered_color  = ImColor(0, 175, 175, 255);
    //node_header_selected_color = ImColor(0, 255, 255, 255);
    //node_header_label_color    = ImColor(255, 255, 255, 255);// ImGui does not have a color function that accepts its own ImColor type for textS

    //node_body_color            = ImColor(0, 0, 255, 255);
    //node_body_hovered_color    = ImColor(0, 175, 175, 255);
    //node_body_selected_color   = IM_COL32(0, 255, 255, 255);
    //node_outline_color         = IM_COL32(255, 255, 255, 255);

    graph->node_editor->SetNodeScreenSpacePos(node_id, click_pos);// This creates node in ImNodes editor

    return true;
}


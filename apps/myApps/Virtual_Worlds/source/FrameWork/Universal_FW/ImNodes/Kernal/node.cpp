#include "node.h"

#include "graph.h"
#include "node_editor.h"

void* node_basis_class::get_node_data() { return nullptr; }// Node data to be accessed by other nodes// +++++++++++++++++++

// !!!!!!!!!!!!!!!!!!! DRAW NODE FUNCTIONS !!!!!!!!!!!!!!!!!!!

bool node_basis_class::draw_node(pins_class pins, float scale) {
//std::cout <<"hcp_voxel_node_class::draw_node:: 000\n";

    if (!visible) return false;
    // Define titlebare colors
    graph->node_editor->PushColorStyle(ImNodesCol_TitleBar, node_header_color);
    graph->node_editor->PushColorStyle(ImNodesCol_TitleBarHovered, node_header_hovered_color);
    graph->node_editor->PushColorStyle(ImNodesCol_TitleBarSelected, node_header_selected_color);

    graph->node_editor->PushColorStyle(ImNodesCol_NodeBackground, node_body_color);
    graph->node_editor->PushColorStyle(ImNodesCol_NodeBackgroundHovered, node_body_hovered_color);
    graph->node_editor->PushColorStyle(ImNodesCol_NodeBackgroundSelected, node_body_selected_color);
    graph->node_editor->PushColorStyle(ImNodesCol_NodeOutline, node_outline_color);

    graph->node_editor->BeginNode(node_id);
//printf("hcp_voxel_node_class::draw_node:: 000\n");
    ImGui::PushStyleColor(ImGuiCol_Text, node_header_label_color);// Node header text color
    draw_node_header(scale);
    ImGui::PopStyleColor();
//std::cout <<"hcp_voxel_node_class::draw_node:: 7777 " <<  offset.x << " : " << offset.y << std::endl;);
    if (!draw_body(node_draw_dimensions, scale)) {
        //error message
        return false;
    }

    for (int i = 0; i < inputs.size(); i++) {
        draw_node_input_link(inputs[i], pins);
    }

    for (int i = 0; i < outputs.size(); i++) {
        draw_node_output_link(outputs[i], pins);
    }

    graph->node_editor->EndNode();
    // Node body
    graph->node_editor->PopColorStyle();
    graph->node_editor->PopColorStyle();
    graph->node_editor->PopColorStyle();
    graph->node_editor->PopColorStyle();
    // Node header    
    graph->node_editor->PopColorStyle();
    graph->node_editor->PopColorStyle();
    graph->node_editor->PopColorStyle();

    return true;
};

bool node_basis_class::draw_body(const ImVec2& size, float scale) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    if (graph->node_editor->zoom_on) {
        ImNodesEditorContext* editor = graph->node_editor->EditorContextGet();
        ImNodeData            node = editor->Nodes.Pool[graph->node_editor->GImNodes.CurrentNodeIdx];

        graph->node_editor->update_canvas_node_zoom(window, *editor, node);
    }

    ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);

    ImGui::ItemSize(size);
    ImGui::ItemAdd(bb, 0);
    return true;
}

void node_basis_class::draw_node_header(std::string header_label, float scale) {
    if (!graph->node_editor) return;

    graph->node_editor->BeginNodeTitleBar();
    float title_width = ImGui::CalcTextSize(label.c_str()).x * scale;
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
    ImGui::TextUnformatted(header_label.c_str());

    node_draw_dimensions = { node_dimensions.x * scale,node_dimensions.y * scale };// ++++
    if (title_width > node_draw_dimensions.x) {
        node_draw_dimensions.x = title_width;
        node_draw_dimensions.y = node_draw_dimensions.x / node_dimensions.x * node_dimensions.y;
    }
    if (node_draw_dimensions.x < MIN_NODE_WIDTH) {
        node_draw_dimensions.x = MIN_NODE_WIDTH;
        node_draw_dimensions.y = node_draw_dimensions.x / node_dimensions.x * node_dimensions.y;
    }

    graph->node_editor->EndNodeTitleBar();
}

void node_basis_class::draw_node_header(float scale) {
    if (!graph->node_editor) return;

    graph->node_editor->BeginNodeTitleBar();
    float title_width = ImGui::CalcTextSize(label.c_str()).x * scale;
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
    ImGui::TextUnformatted(label.c_str());
    node_draw_dimensions = { node_dimensions.x * scale,node_dimensions.y * scale };// ++++
    if (title_width > node_draw_dimensions.x) {
        node_draw_dimensions.x = title_width;
        node_draw_dimensions.y = node_draw_dimensions.x / node_dimensions.x * node_dimensions.y;
    }
    if (node_draw_dimensions.x < MIN_NODE_WIDTH) {
        node_draw_dimensions.x = MIN_NODE_WIDTH;
        node_draw_dimensions.y = node_draw_dimensions.x / node_dimensions.x * node_dimensions.y;
    }

    graph->node_editor->EndNodeTitleBar();
}

void node_basis_class::draw_node_input_link(int input_pin_id,
                                            pins_class &pins)
{
    if (!graph->node_editor) return;
//std::cout << "node_basis_class :: draw_node_input_link 0000\n";

     int i = pins.get_input_pin_index(input_pin_id);
//std::cout << "node_basis_class::draw_node_input_link 2222 "<< i << std::endl;
    if (i == INVALID_ID) return;

    pin_struct_type pin = pins.input_pins[i];

    graph->node_editor->PushColorStyle(ImNodesCol_Pin, pin.pin_color);
    graph->node_editor->PushColorStyle(ImNodesCol_PinHovered, pin.pin_hovered_color);
    // Add other pin attributes here
    // ******

    graph->node_editor->BeginInputAttribute(input_pin_id, pin.pin_shape);

    std::string label = pin.label;
//std::cout << "node_basis_class::draw_node_input_link 3333\n";
    float label_width = ImGui::CalcTextSize(label.c_str()).x;
    ImGui::TextUnformatted(label.c_str());

    ImGui::SameLine();

    ImGui::PushItemWidth(node_draw_dimensions.x - label_width);;

    // need a function here to define what iput widget is to be defined to have the link
    // value assigned to and the display of that widget.
    // since for virtual worlds all inputs are to be displayed in a seperate widget
    // no widget display within the node itself.
    ImGui::PopItemWidth();
    // }
//std::cout << "node_basis_class :: draw_node_input_link 44444\n";
    graph->node_editor->EndInputAttribute();

    // ******
    graph->node_editor->PopColorStyle();
    graph->node_editor->PopColorStyle();
    // add any other end pin attributes here
    // ******

}

void node_basis_class::draw_node_output_link(int output_pin_id,
                                            pins_class& pins)
{
//printf("node_basis_class :: draw_node_input_link 0000\n");
    if (!graph->node_editor) return;

    // ******
     int i = pins.get_output_pin_index(output_pin_id);
//std::cout << "node_basis_class::draw_node_input_link 2222 "<< i << std::endl;
    if (i == INVALID_ID) return;

    pin_struct_type pin = pins.output_pins[i];

    graph->node_editor->PushColorStyle(ImNodesCol_Pin, pin.pin_color);
    graph->node_editor->PushColorStyle(ImNodesCol_PinHovered, pin.pin_hovered_color);
    // Add other pin attributes here
    // ******

    graph->node_editor->BeginOutputAttribute(output_pin_id, pin.pin_shape);
//printf("node_basis_class :: draw_node_input_link 1111  %i \n",pins.input_pins.size());
    std::string label = pin.label;

    float label_width = ImGui::CalcTextSize(label.c_str()).x;
    ImGui::Indent(node_draw_dimensions.x - label_width);
    ImGui::TextUnformatted(label.c_str());

//printf("node_basis_class :: draw_node_input_link 44444\n");
    graph->node_editor->EndOutputAttribute();

    graph->node_editor->PopColorStyle();
    graph->node_editor->PopColorStyle();
    // add any other end pin attributes here
}

bool node_basis_class::evaluated() { // Find if all the node output values have been evaluated
    for (int output_pin_id : outputs) {
        pin_struct_type output_pin = graph->pins.find_pin(output_pin_id);
        if (!output_pin.evaluated) {
            return false;
        }
    }
    return true;
}


void  node_basis_class::export_mandatory_node_data(std::fstream& stream) {
    // !!!!!!!!!!! These must be present for all nodes of any type

    stream << ui_node_type.category << std::endl;
    stream << ui_node_type.node_type << std::endl;

    stream << visible << std::endl;
    stream << label << std::endl;

    stream << node_id << std::endl;
    stream << node_group_id << std::endl;

    // These are for the editor to use to display node entity parameter UI
    stream << node_entity_id << std::endl;
    stream << node_entity_category_id << std::endl;
    stream << node_entity_type_id << std::endl;

    ImVec2 node_grid_space_pos = graph->node_editor->GetNodeGridSpacePos(node_id);
    stream << node_grid_space_pos.x << ":" << node_grid_space_pos.y << std::endl;
}

void node_basis_class::export_node_pin_data(std::fstream& stream) {
    // Export Node pin data  
    stream << GRAPH_INPUT_PIN_START_FLAG << ENDL;
    for (int input_pin_id : inputs) {
        pin_struct_type input_pin = graph->pins.find_pin(input_pin_id);

        if (input_pin.pin_id != INVALID_ID && input_pin.node_id == node_id) input_pin.export_pin_data(stream);
    }
    stream << GRAPH_INPUT_PIN_END_FLAG << ENDL;

    stream << GRAPH_OUTPUT_PIN_START_FLAG << ENDL;
    for (int output_pin_id : outputs) {
        pin_struct_type output_pin = graph->pins.find_pin(output_pin_id);

        if (output_pin.pin_id != INVALID_ID && output_pin.node_id == node_id)output_pin.export_pin_data(stream);
    }
    stream << GRAPH_OUTPUT_PIN_END_FLAG << ENDL;
}

// export_null_node_pin_data used for all node types that have its pins created when the node is created
// otherwise end up with multple pins every time a that node is imported into the editor.
void node_basis_class::export_null_node_pin_data(std::fstream& stream) {
    // Export Node pin data  
    stream << GRAPH_INPUT_PIN_START_FLAG << ENDL;
    stream << GRAPH_INPUT_PIN_END_FLAG << ENDL;

    stream << GRAPH_OUTPUT_PIN_START_FLAG << ENDL;
    stream << GRAPH_OUTPUT_PIN_END_FLAG << ENDL;
}
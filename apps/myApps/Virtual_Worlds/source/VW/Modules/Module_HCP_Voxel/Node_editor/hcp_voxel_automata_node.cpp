#include "hcp_voxel_automata_node.h"

#include <Universal_FW/ImNodes/Kernal/node_editor.h>>

#include <VW/Editor/Node_Editor/node_editor_extras.h>

bool hcp_voxel_automata_node_class::define_node(ImVec2 click_pos, node_id_type entity_id_) {
    node_dimensions = { 60.0f,5.0f };

    // !!! CRITICAL : MUST HAVE NODE UI NODE TYPE DEFINITION DEFINED
    // !!! ELSE CANNOT LINK NODE PINS, PERFORM EVALUATIONS OR DISPLAY
    // !!! NODE PARAMETERS TO BE DISPLAYED AND MODIFIED AS WELL AS
    // !!! OTHER NODE AND NODE EDITOR FUCTIONALITY 

    ui_node_type.category          = NODE_CATEGORY_ID_MODIFIER;
    ui_node_type.node_type         = NODE_TYPE_ID_HCP_VOXEL_AUTOMATA;
    ui_node_type.node_data_context = NODE_DATA_CONTEXT;

    std::cout << "hcp_voxel_node_class :: define_node 111  : " << std::endl;

    label = "HCP AUTOMATA";// +std::to_string(hcp_voxel->id);

    node_header_color = ImColor(0, 139, 139, 255);
    node_header_hovered_color = ImColor(0, 175, 175, 255);
    node_header_selected_color = ImColor(0, 255, 255, 255);
    node_header_label_color = ImColor(255, 255, 255, 255);// ImGui does not have a color function that accepts its own ImColor type for textS

    node_body_color = ImColor(75, 75, 75, 255);
    node_body_hovered_color = ImColor(100, 100, 100, 255);
    node_body_selected_color = ImColor(150, 150, 150, 255);
    node_outline_color = ImColor(50, 50, 50, 255);

    graph->node_editor->SetNodeScreenSpacePos(node_id, click_pos);
    return true;
}

bool hcp_voxel_automata_node_class::define_import_node(ImVec2 click_pos, node_id_type entity_id_, void* node_misc_data) {
    node_misc_import_struct_type* node_misc_import_data = static_cast<node_misc_import_struct_type*>(node_misc_data);

    if (!node_misc_import_data) {
        std::cout << "ERROR : Editor scene and log data not defined. Aborting import of HCP Voxel node.\n";
        return false;
    }

    if (node_misc_import_data->log_panel == nullptr) {
        std::cout << "WARNING : Imported HCP Voxel node has no defined log panel to output messages to.\n";
    }

    if (node_misc_import_data->vw_scene == nullptr) {
        std::cout << "ERROR : Imported HCP Voxel node has no defined scene manager. Aborted import.\n" << std::endl;
        if (log_panel) log_panel->application_log.AddLog("ERROR :Imported HCP Voxel node has no defined scene manager. Aborted import.\n");
        return false;
    }

    log_panel = node_misc_import_data->log_panel;
    vw_scene = node_misc_import_data->vw_scene;

    return define_node(click_pos, entity_id_);
}

void hcp_voxel_automata_node_class::editor_menu_options() {
    //if (ImGui::BeginMenu("Node/Entity ...")) {
    if (ImGui::MenuItem("automata test00")) {
        //printf("scene_node_editor_class : display : save entity selected\n");
    }

    if (ImGui::MenuItem("automata test01")) {
        //printf("scene_node_editor_class : display : save entity selected\n");
    }

    if (ImGui::MenuItem("automata test02")) {
        //printf("scene_node_editor_class : display : save entity selected\n");
                                // Not yet implemented    
    }

    if (ImGui::MenuItem("automata test03")) {
        //printf("scene_node_editor_class : display : select node tree\n");
                                // Not yet implemented    
    }


    // ImGui::EndMenu();
 //}
}

bool hcp_voxel_automata_node_class::define_ui() {
//std::cout << "hcp_voxel_automata_node_class::display_ui\n";

    globalc::set_current_selected_data_context_id(this->ui_node_type.node_data_context);
    globalc::set_current_selected_entity_id(INVALID_ID);
    globalc::set_current_selected_entity_category_type_id(INVALID_ID);
    globalc::set_current_selected_entity_type_id(INVALID_ID);

    globalc::set_current_selected_node(this);

    return true;
}

void hcp_voxel_automata_node_class::display_ui(node_id_type current_selected_object_id) {
//std::cout << "hcp_voxel_automata_node_class::display_ui\n";

    // Best to perform this code block only when connecting or diconnecting node input link
    //if (!vw_scene || !node_hcp_object) {
    if (!vw_scene ) {
        // error message
        return;
    }

    voxel_hcp_object_class *voxel_hcp_object_to_execute = nullptr;
    // find link to input pin
    link_struct_type input_link = graph->links.find_link_with_end_pin(inputs[0]);// Only have one input pin which is defined as default

    if (input_link.link_id != INVALID_ID) {
        pin_struct_type link_from_pin = graph->pins.find_pin(input_link.from);

        if (link_from_pin.node_id != INVALID_ID) {
            node_basis_class *node = graph->nodes.get_node(link_from_pin.node_id);

            if (node) {
                vw_object_base_class *hcp_voxel_base_object = vw_scene->scene_entities_manager.get_object_of_category(node->node_entity_id, node->node_entity_category_id);
                if (hcp_voxel_base_object) {
//std::cout << "hcp_voxel_automata_node_class::display_ui : hcp_voxel_base_object != NULL\n";
                    voxel_hcp_object_to_execute = dynamic_cast<voxel_hcp_object_class*>(hcp_voxel_base_object);
                }
            }
        }
    }

    display_voxel_automata_widget(voxel_hcp_object_to_execute);
}

void hcp_voxel_automata_node_class::display_voxel_automata_widget(voxel_hcp_object_class *voxel_hcp_object_to_execute) {
    voxel_hcp_automata_widget.voxel_hcp_object_to_execute = voxel_hcp_object_to_execute;

    //if (voxel_hcp_automata_widget.voxel_hcp_object_to_execute) {
    //    if (log_panel != NULL) log_panel->application_log.AddLog("have voxel object to perform automata on.\n");
       // return;
    //}

    voxel_hcp_automata_widget.log_panel = log_panel;
    voxel_hcp_automata_widget.voxel_hcp_automata_byte_rules = &voxel_hcp_automata_byte_rules;

    voxel_hcp_automata_widget.automata_functions_logging(log_panel);
    voxel_hcp_automata_widget.display();
}
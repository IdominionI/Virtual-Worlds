#include "hex_surface_automata_node.h"

#include <Universal_FW/ImNodes/Kernal/node_editor.h>>

#include <VW/Editor/Node_Editor/node_editor_extras.h>

#include <Universal_FW/Timeline/timeline_tracks.h>

bool hex_surface_automata_node_class::define_node(ImVec2 click_pos, node_id_type entity_id_) {
    node_dimensions = { 60.0f,5.0f };

    // !!! CRITICAL : MUST HAVE NODE UI NODE TYPE DEFINITION DEFINED
    // !!! ELSE CANNOT LINK NODE PINS, PERFORM EVALUATIONS OR DISPLAY
    // !!! NODE PARAMETERS TO BE DISPLAYED AND MODIFIED AS WELL AS
    // !!! OTHER NODE AND NODE EDITOR FUCTIONALITY 

    ui_node_type.category          = NODE_CATEGORY_ID_MODIFIER;
    ui_node_type.node_type         = NODE_TYPE_ID_HEX_SURFACE_AUTOMATA;
    ui_node_type.node_data_context = NODE_DATA_CONTEXT;

std::cout << "hex_surface_automata_node_class :: define_node 111  : " << std::endl;

    label = "Hex Automata " + std::to_string(node_id);

    node_header_color          = ImColor(0, 139, 139, 255);
    node_header_hovered_color  = ImColor(0, 175, 175, 255);
    node_header_selected_color = ImColor(0, 255, 255, 255);
    node_header_label_color    = ImColor(255, 255, 255, 255);// ImGui does not have a color function that accepts its own ImColor type for textS

    node_body_color          = ImColor(75, 75, 75, 255);
    node_body_hovered_color  = ImColor(100, 100, 100, 255);
    node_body_selected_color = ImColor(150, 150, 150, 255);
    node_outline_color       = ImColor(50, 50, 50, 255);

    graph->node_editor->SetNodeScreenSpacePos(node_id, click_pos);
    return true;
}

bool hex_surface_automata_node_class::define_import_node(ImVec2 click_pos, node_id_type entity_id_, void* node_misc_data) {
    node_misc_import_struct_type* node_misc_import_data = static_cast<node_misc_import_struct_type*>(node_misc_data);

    if (!node_misc_import_data) {
std::cout << "ERROR : Editor scene and log data not defined. Aborting import of Hex Surface node.\n";
        return false;
    }

    if (node_misc_import_data->log_panel == nullptr) {
std::cout << "WARNING : Imported Hex Surface node has no defined log panel to output messages to.\n";
    }

    if (node_misc_import_data->vw_scene == nullptr) {
std::cout << "ERROR : Imported Hex Surface node has no defined scene manager. Aborted import.\n" << std::endl;
        if (log_panel) log_panel->application_log.AddLog("ERROR :Imported Hex Surface node has no defined scene manager. Aborted import.\n");
        return false;
    }

    log_panel = node_misc_import_data->log_panel;
    vw_scene = node_misc_import_data->vw_scene;

    return define_node(click_pos, entity_id_);
}

// ++++++++++++++++++++++++++++++
bool hex_surface_automata_node_class::create_hcp_automata_timeline_link() {
    hex_animation_object = new hex_surface_automata_animation_object_class;

    if (!hex_animation_object) return false;

    // It may be that in future with some altered code this using a pointer to the master voxel_hcp_automata_widget class
    // does not work and this pointer becomes a local variable that needs initialisation 
    hex_animation_object->hex_surface_automata_widget = &hex_surface_automata_widget;

    // ================ define as a create timeline group function ===============
    if (!animation_timeline_tracks_widget->timeline_track_group_exists(node_id)) {
 //std::cout << "hcp_voxel_node_class::create_hcp_animation_link 000 : !animation_timeline_tracks_widget->timeline_track_group_exists(node_id)\n";
        int index = animation_timeline_tracks_widget->add_timeline_group(label, node_id, hex_animation_object, TIMELINE_OBJECT_DATA_TYPE_ID_HEX_SURFACE_AUTOMATA);
        if (index != INVALID_ID && index < animation_timeline_tracks_widget->timeline_track_groups.size()) {

            timeline_interval_track_id = animation_timeline_tracks_widget->timeline_track_groups[index].add_interval_track("Hex Surface\nAutomata");
            if (timeline_interval_track_id == UINT_MAX) {
                animation_timeline_tracks_widget->delete_timeline_group(node_id);
                return false;
            }
        }
    }
    // =====================================================

    return true;
}

hex_surface_object_class* hex_surface_automata_node_class::get_hex_surface_object_to_execute() {
    if (!vw_scene) {
        // error message
        return nullptr;
    }

    hex_surface_object_class* voxel_hcp_object_to_execute = nullptr;
    // find link to input pin
    link_struct_type input_link = graph->links.find_link_with_end_pin(inputs[0]);// Only have one input pin which is defined as default

    if (input_link.link_id != INVALID_ID) {
        pin_struct_type link_from_pin = graph->pins.find_pin(input_link.from);

        if (link_from_pin.node_id != INVALID_ID) {
            node_basis_class* node = graph->nodes.get_node(link_from_pin.node_id);

            if (node) {
                vw_object_base_class* hcp_voxel_base_object = vw_scene->scene_entities_manager.get_object_of_category(node->node_entity_id, node->node_entity_category_id);
                if (hcp_voxel_base_object) {
//std::cout << "hex_surface_automata_node_class::display_ui : hcp_voxel_base_object != NULL\n";
                    voxel_hcp_object_to_execute = dynamic_cast<hex_surface_object_class*>(hcp_voxel_base_object);
                }
            }
        }
    }

    return voxel_hcp_object_to_execute;
}

void hex_surface_automata_node_class::delete_hcp_automata_timeline_link() {
    animation_timeline_tracks_widget->delete_timeline_group(node_id);
    delete hex_animation_object;
    hex_animation_object = nullptr;
    timeline_interval_track_id = UINT_MAX;

    // If have more tha one trck in a group
    // : delete track hex_animation_object and initialise variables to unassigned values here
}

// ++++++++++++++++++++++++++++++

void hex_surface_automata_node_class::editor_menu_options() {
    if (ImGui::BeginMenu("Node/Entity ...")) {
        std::string menu_text = "Add timeline track###anvfs" + std::to_string(node_id);
        if (ImGui::MenuItem(menu_text.c_str())) {
            if (hex_animation_object == nullptr)
                create_hcp_automata_timeline_link();
        }

        menu_text = "Delete timeline group###nvfs" + std::to_string(node_id);
        if (ImGui::MenuItem(menu_text.c_str())) {
            delete_hcp_automata_timeline_link();
        }

        // When more than one track in a group uncomment the following
        //menu_text = "Delete animation track###anvfs" + std::to_string(node_id);
        //if (ImGui::MenuItem(menu_text.c_str())) {
        //    animation_timeline_tracks_widget->delete_timeline_group_track(node_id, timeline_interval_track_id);
        //}

        ImGui::EndMenu();
    }
}

bool hex_surface_automata_node_class::define_ui() {
//std::cout << "hex_surface_automata_node_class::display_ui\n";

    globalc::set_current_selected_data_context_id(this->ui_node_type.node_data_context);
    globalc::set_current_selected_entity_id(INVALID_ID);
    globalc::set_current_selected_entity_category_type_id(INVALID_ID);
    globalc::set_current_selected_entity_type_id(INVALID_ID);

    globalc::set_current_selected_node(this);

    return true;
}

void hex_surface_automata_node_class::display_ui(node_id_type current_selected_object_id) {
//std::cout << "hex_surface_automata_node_class::display_ui\n";

    // Best to perform this code block only when connecting or diconnecting node input link
    if (!vw_scene) {
        // error message
        return;
    }

    hex_surface_object_class* voxel_hcp_object_to_execute = get_hex_surface_object_to_execute();
    display_voxel_automata_widget(voxel_hcp_object_to_execute);
}

void hex_surface_automata_node_class::display_voxel_automata_widget(hex_surface_object_class* hex_surface_object_to_execute) {
    hex_surface_automata_widget.hex_surface_object_to_execute = hex_surface_object_to_execute;

    hex_surface_automata_widget.log_panel = log_panel;
    hex_surface_automata_widget.hex_surface_automata_rules = &hex_automata_byte_rules;

    hex_surface_automata_widget.automata_functions_logging(log_panel);
    hex_surface_automata_widget.display();
}
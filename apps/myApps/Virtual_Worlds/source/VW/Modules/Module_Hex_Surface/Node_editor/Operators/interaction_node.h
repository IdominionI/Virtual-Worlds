#pragma once
// Mandatory for all VW node editor nodes
#include <Universal_FW/ImNodes/Kernal/node.h>

#include <FrameWork/Kernels/ImGuiEx/imgui_widgets.h>

#include <VW/Editor/Main_Window/Panels/log_panel.h>
#include <VW/Editor/Node_Editor/node_editor_extras.h>

#include <VW_framework/Scene/vw_scene.h>
//---------------------------

//#include <Source/Editor/Scene/scene_manager.h>
//#include <Source/Editor/Main_Window/Panels/log_panel.h>
//#include <Source/Editor/Node_editor/ImNodes/Kernal/graph.h>
//#include <Source/Editor/Common/global.h>

#include "../../Object/hex_surface_object.h"

#include "../hex_surface_node_definitions.h"
#include "../../Functions/hex_surface_interactions.h"

#define HEX_SURFACE_INTERACTIONS_NONE               0
#define HEX_SURFACE_INTERACTIONS_SUPERPOSITION_ADD  1
#define HEX_SURFACE_INTERACTIONS_SUPERPOSITION_SUB  2
#define HEX_SURFACE_INTERACTIONS_UNION              3
#define HEX_SURFACE_INTERACTIONS_AND                4
#define HEX_SURFACE_INTERACTIONS_OR                 5
#define HEX_SURFACE_INTERACTIONS_EXOR               6
#define HEX_SURFACE_INTERACTIONS_COMPLEMENT         7

class hex_surface_interaction_node_class : public node_basis_class {
public:
    node_status_enum node_status = node_status_enum::none;

    // Mandatory for all VW node editor nodes
    log_panel_class *log_panel = nullptr; // Define the application log panel to display application messages to
    vw_scene_class  *vw_scene  = nullptr;

    int interaction_type;

    // ++++++++++++++++++++++
    //hex_surface_interaction_animation_class *hcp_animation_object = nullptr; // This is defined in translation.cpp
    hex_surface_object_class              *get_hex_surface_object_to_execute(int input_pin_index);
    unsigned int                           timeline_interval_track_id = UINT_MAX;
    // future added tracks id variables go here
    // ++++++++++++++++++++++

    void *get_node_data() {
        //return get_hex_surface_hcp_object_to_execute();
        return interaction_object; // ++++++++++++++
    }

    bool define_node(ImVec2 click_pos,id_type  entity_id);
    bool define_import_node(ImVec2 click_pos, node_id_type entity_id_, void* node_misc_data = NULL);

    bool define_inputs(pins_class& pins);
    bool define_outputs(pins_class& pins);

    void delete_node_entity();
    void delete_hex_grid(); // So as to be able to delete interaction hex data without deleting the node

    // +++++++++++++++++++
    bool create_hex_surface_interaction_timeline_link();
    void delete_hex_surface_interaction_timeline_link();
    // +++++++++++++++++++
    void editor_menu_options();

    // The display_ui here uses a different hex_surface_hcp_parameters_widget to the main 
    // and thus will have different parameter values stored. Need to cetralise to access
    // this hex_surface_hcp_parameters_widget instance, or the central one only
    bool define_ui();
    void display_ui(node_id_type current_selected_object_id = -1);
    void display_interaction_ui(id_type current_selected_object_id = -1);
    
    void action();

    //node_basis_class* find_node_to_modify(int input_pin_id);
    bool peform_interaction(hex_surface_object_class* hex_surface_object_A, hex_surface_object_class* hex_surface_object_B, int interaction_type);
    void assign_hex_surface(int xi, int yi, hex_surface_data_type value);

    void export_node(std::fstream &stream) {
        // Exports must be in the following order
        export_mandatory_node_data(stream);// mandatory node export function that must be present in all graph node exports

        // specific Node data here
        export_hex_surface_interaction_node_parameter_data(stream);

        export_node_pin_data(stream);// mandatory node export function that must be present in all graph node exports if have pins
    }

    void export_hex_surface_interaction_node_parameter_data(std::fstream& stream);
    bool import_node(std::vector<std::string> lines, int& line_number);

protected:
private:
    hex_surface_object_class *interaction_object = NULL;

    hex_surface_interactions_class hex_surface_interaction;
  
    int error_code = 0;
};

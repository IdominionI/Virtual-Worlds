#pragma once
// Mandatory for all VW node editor nodes
#include <Universal_FW/ImNodes/Kernal/node.h>

#include <FrameWork/Kernels/ImGuiEx/imgui_widgets.h>

#include <VW/Editor/Main_Window/Panels/log_panel.h>
#include <VW/Editor/Node_Editor/node_editor_extras.h>

#include <VW_framework/Scene/vw_scene.h>
//---------------------------

#include "../hcp_voxel_node_definitions.h"
#include "../../Functions/voxel_interactions.h"

#define VOXEL_INTERACTIONS_NONE               0
#define VOXEL_INTERACTIONS_SUPERPOSITION_ADD  1
#define VOXEL_INTERACTIONS_SUPERPOSITION_SUB  2
#define VOXEL_INTERACTIONS_UNION              3
#define VOXEL_INTERACTIONS_AND                4
#define VOXEL_INTERACTIONS_OR                 5
#define VOXEL_INTERACTIONS_EXOR               6
#define VOXEL_INTERACTIONS_COMPLEMENT         7

class hcp_voxel_interaction_node_class : public node_basis_class {
public:
    node_status_enum node_status = node_status_enum::none;

    // Mandatory for all VW node editor nodes
    log_panel_class *log_panel = nullptr; // Define the application log panel to display application messages to
    vw_scene_class  *vw_scene  = nullptr;

    int interaction_type;

    // ++++++++++++++++++++++
    //hcp_voxel_translation_animation_class *hcp_animation_object = nullptr; // This is defined in translation.cpp
    voxel_hcp_object_class                *get_voxel_hcp_object_to_execute(int input_pin_index);
    unsigned int                           timeline_interval_track_id = UINT_MAX;
    // future added tracks id variables go here
    // ++++++++++++++++++++++

     //voxel_hcp_object_class *interaction_object = nullptr;

    // node_basis_class functions
    bool define_node(ImVec2 click_pos, id_type entity_id_);
    bool define_import_node(ImVec2 click_pos, node_id_type entity_id_, void* node_misc_data = NULL);

    void *get_node_data() {
        return interaction_object; // ++++++++++++++
    }
    
    void delete_node_entity() {
        delete_hcp_interaction_timeline_link();
        delete_voxel_matrix();
    }

    void delete_voxel_matrix() { // So as to be able to delete interaction voxel data without deleting the node
        if (!interaction_object || !vw_scene) return; // have no object to delete

//std::cout<< "hcp_voxel_interaction_node_class:delete_voxel_matrix !!!! " << node_entity_id << ":" << node_entity_category_id << std::endl;
        if (node_entity_id != INVALID_ID && node_entity_category_id != INVALID_ID) {
            vw_scene->scene_entities_manager.delete_object(node_entity_id, node_entity_category_id);
            node_entity_id          = INVALID_ID;
            node_entity_category_id = INVALID_ID;
            interaction_object      = nullptr;
        }
    }

    bool define_inputs(pins_class& pins) {
        pin_struct_type pin;

        pin.label     = "Matrix A";
        pin.context   = CONTEXT_ID_HCP_VOXEL;
        pin.data_type = DATA_VALUE_TYPE_HCP_VOXEL_MATRIX;
        pin.pin_type  = pin_type_enum::single_input;

        pin.pin_color          = IM_COL32(120, 30, 30, 255);
        pin.pin_hovered_color  = IM_COL32(200, 85, 85, 255);
        pin.pin_selected_color = IM_COL32(200, 120, 120, 255);
        pin.pin_shape          = ImNodesPinShape_::ImNodesPinShape_QuadFilled;

        if (add_node_input_pin(pin, pins) == INVALID_ID) return false;

        pin.label     = "Matrix B";
        pin.context   = CONTEXT_ID_HCP_VOXEL;
        pin.data_type = DATA_VALUE_TYPE_HCP_VOXEL_MATRIX;
        pin.pin_type  = pin_type_enum::single_input;

        pin.pin_color          = IM_COL32(120, 30, 30, 255);
        pin.pin_hovered_color  = IM_COL32(200, 85, 85, 255);
        pin.pin_selected_color = IM_COL32(200, 120, 120, 255);
        pin.pin_shape          = ImNodesPinShape_::ImNodesPinShape_QuadFilled;

        if (add_node_input_pin(pin, pins) == INVALID_ID) return false;

        return true;
    }

    bool define_outputs(pins_class& pins) {
        pin_struct_type pin; 

        pin.label     = "Matrix";
        pin.context   = CONTEXT_ID_HCP_VOXEL;
        pin.data_type = DATA_VALUE_TYPE_HCP_VOXEL_MATRIX;
        pin.pin_type  = pin_type_enum::single_output;

        pin.pin_color          = IM_COL32(120, 30, 30, 255);
        pin.pin_hovered_color  = IM_COL32(200, 85, 85, 255);
        pin.pin_selected_color = IM_COL32(200, 120, 120, 255);
        pin.pin_shape          = ImNodesPinShape_::ImNodesPinShape_QuadFilled;

        if (add_node_output_pin(pin, pins) == INVALID_ID) return false;

        return true;
    }

    //bool create_hcp_voxel();

    // +++++++++++++++++++
    bool create_hcp_interaction_timeline_link();
    void delete_hcp_interaction_timeline_link();
    // +++++++++++++++++++

    void editor_menu_options();

    // The display_ui here uses a different voxel_hcp_parameters_widget to the main 
    // and thus will have different parameter values stored. Need to cetralise to access
    // this voxel_hcp_parameters_widget instance, or the central one only

    bool define_ui();
    void display_ui(node_id_type current_selected_object_id = -1);

    void display_interaction_ui(id_type current_selected_object_id = -1);

    void action();

    bool perform_interaction(voxel_hcp_object_class* voxel_hcp_object_A, voxel_hcp_object_class* voxel_hcp_object_B, int interaction_type);
    void assign_voxel(int xi, int yi, int zi, int value);

    void export_node(std::fstream &stream) {
        // Exports must be in the following order
        export_mandatory_node_data(stream);// mandatory node export function that must be present in all graph node exports

        // specific Node data here
        export_hcp_interaction_node_parameter_data(stream);

        export_node_pin_data(stream);// mandatory node export function that must be present in all graph node exports if have pins
    }

    void export_hcp_interaction_node_parameter_data(std::fstream& stream);
    bool import_node(std::vector<std::string> lines, int& line_number);

protected:
private:
    voxel_hcp_object_class *interaction_object = nullptr;

    hcp_voxel_interactions_class voxel_interaction;
};

#pragma once
// Mandatory for all VW node editor nodes
#include <Universal_FW/ImNodes/Kernal/node.h>

#include <VW/Editor/Main_Window/Panels/log_panel.h>
#include <VW/Editor/Node_Editor/node_editor_extras.h>

#include <VW_framework/Scene/vw_scene.h>

// Header files unique for Voxel Node
#include "hcp_voxel_node_definitions.h"
#include "../Object/voxel_hcp_object.h"
#include "../Editor/Widgets/hcp_voxel_automata_widget.h"


#include "../Animation/hcp_voxel_automata_animation.h" // +++++++++++

class hcp_voxel_automata_node_class : public node_basis_class {
public:

    node_status_enum node_status = node_status_enum::none;

    // Mandatory for all VW node editor nodes
    log_panel_class *log_panel = nullptr; // Define the application log panel to display application messages to
    vw_scene_class  *vw_scene  = nullptr;

    std::vector<voxel_hcp_automata_byte_rule_struct_type> voxel_hcp_automata_byte_rules;

    // ++++++++++++++++++++++
    hcp_voxel_automata_animation_object_class *hcp_animation_object = nullptr;
    voxel_hcp_object_class                    *get_voxel_hcp_object_to_execute();
    unsigned int                               timeline_interval_track_id = UINT_MAX;
    // future added tracks id variables go here
    // ++++++++++++++++++++++

    // node_basis_class functions
    bool define_node(ImVec2 click_pos, node_id_type entity_id_);
    bool define_import_node(ImVec2 click_pos, node_id_type entity_id_, void* node_misc_data = NULL);

    void delete_node_entity() {
        delete_hcp_automata_timeline_link();
    }

    bool define_inputs(pins_class& pins) {// This probably will not be needed but kept just in case
        pin_struct_type pin;

        pin.label     = "Matrix";
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

    // +++++++++++++++++++
    bool create_hcp_automata_timeline_link();
    void delete_hcp_automata_timeline_link();
    // +++++++++++++++++++

    void editor_menu_options();

    // The display_ui here uses a different voxel_hcp_parameters_widget to the main 
    // and thus will have different parameter values stored. Need to cetralise to access
    // this voxel_hcp_parameters_widget instance, or the central one only

    bool define_ui();
    void display_ui(node_id_type current_selected_object_id = -1);

    void export_node(std::fstream &stream) {
        // Exports must be in the following order
        export_mandatory_node_data(stream);// mandatory node export function that must be present in all graph node exports

        // specific Node data here
        export_hcp_voxel_node_parameter_data(stream);

        export_node_pin_data(stream);// mandatory node export function that must be present in all graph node exports if have pins
    }

    void  export_hcp_voxel_node_parameter_data(std::fstream &stream) {
        import_export_byte_automata_rules_class import_export_byte_automata_rules;
        import_export_byte_automata_rules.byte_automata_rules = voxel_hcp_automata_byte_rules;

        stream << RULE_DATA_START << ENDL;
        import_export_byte_automata_rules.write_hcp_automata_byte_rules(stream);
        stream << RULE_DATA_END << ENDL;
    }

    bool import_node(std::vector<std::string> lines, int& line_number) {
        return import_hcp_voxel_automata_rules(lines, line_number);
    }

    bool import_hcp_voxel_automata_rules(std::vector<std::string> lines, int& line_number) {
        import_export_byte_automata_rules_class import_export_byte_automata_rules;
        line_number++;

        std::cout << "hcp_voxel_automata_node_class::import_hcp_voxel_automata_rules <------ : " << line_number << ":" << lines[line_number] << std::endl;
        return import_export_byte_automata_rules.read_automata_byte_rules_into_byte_rules_parameters(lines, voxel_hcp_automata_byte_rules, line_number);
    }


private:
    voxel_hcp_automata_widget_class	   voxel_hcp_automata_widget;

    void display_voxel_automata_widget(voxel_hcp_object_class *voxel_hcp_object_to_execute);
};


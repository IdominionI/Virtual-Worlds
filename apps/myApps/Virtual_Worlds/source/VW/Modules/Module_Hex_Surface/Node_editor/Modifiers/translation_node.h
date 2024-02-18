#pragma once
// Mandatory for all VW node editor nodes
#include <Universal_FW/ImNodes/Kernal/node.h>

#include <FrameWork/Kernels/ImGuiEx/imgui_widgets.h>

#include <VW/Editor/Main_Window/Panels/log_panel.h>
#include <VW/Editor/Node_Editor/node_editor_extras.h>

#include <VW_framework/Scene/vw_scene.h>

// Header files unique for hex Node
#include "../hex_surface_node_definitions.h"
#include "../../Object/hex_surface_object.h"

class hex_surface_translation_node_class : public node_basis_class {
public:
     node_status_enum node_status = node_status_enum::none;

    // Mandatory for all VW node editor nodes
    log_panel_class *log_panel = nullptr; // Define the application log panel to display application messages to
    vw_scene_class  *vw_scene  = nullptr;

    // ++++++++++++++++++++++
    //hex_surface_translation_animation_class *hcp_animation_object = nullptr; // This is defined in translation.cpp
    hex_surface_object_class              *get_hex_surface_object_to_execute();
    unsigned int                           timeline_interval_track_id = UINT_MAX;
    // future added tracks id variables go here
    // ++++++++++++++++++++++

    // node_basis_class functions
    bool define_node(ImVec2 click_pos, id_type entity_id_);
    bool define_import_node(ImVec2 click_pos, node_id_type entity_id_, void* node_misc_data = NULL);

    void *get_node_data() {
        return get_hex_surface_object_to_execute();
    }

    void delete_node_entity() {
       delete_hex_surface_translation_timeline_link();
    }

    bool define_inputs(pins_class& pins) {
        pin_struct_type pin;

        pin.label     = "Grid";
        pin.context   = CONTEXT_ID_HEX_SURFACE_GRID;
        pin.data_type = DATA_VALUE_TYPE_HEX_SURFACE_GRID;
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

        pin.label     = "Grid";
        pin.context   = CONTEXT_ID_HEX_SURFACE_GRID;
        pin.data_type = DATA_VALUE_TYPE_HEX_SURFACE_GRID;
        pin.pin_type  = pin_type_enum::single_output;

        pin.pin_color          = IM_COL32(120, 30, 30, 255);
        pin.pin_hovered_color  = IM_COL32(200, 85, 85, 255);
        pin.pin_selected_color = IM_COL32(200, 120, 120, 255);
        pin.pin_shape          = ImNodesPinShape_::ImNodesPinShape_QuadFilled;

        if (add_node_output_pin(pin, pins) == INVALID_ID) return false;

        return true;
    }

    // +++++++++++++++++++
    bool create_hex_surface_translation_timeline_link();
    void delete_hex_surface_translation_timeline_link();
    // +++++++++++++++++++

    void editor_menu_options();

    bool define_ui();
    void display_ui(node_id_type current_selected_object_id = -1);
    void display_translation_ui(node_id_type current_selected_object_id = -1);
 
    void action();
    void decrement_tanslation();
    void increment_translation();
    void coordinate_translation(bool relative_translation = true);

    void export_node(std::fstream &stream) {
        // Exports must be in the following order
        export_mandatory_node_data(stream);// mandatory node export function that must be present in all graph node exports

        // specific Node data here
        export_hex_translation_node_parameter_data(stream);

        export_node_pin_data(stream);// mandatory node export function that must be present in all graph node exports if have pins
    }

    void export_hex_translation_node_parameter_data(std::fstream& stream);
    bool import_node(std::vector<std::string> lines, int& line_number);

 
protected:

private:
    hex_surface_render_class hex_surface_render;
    bool translate = true;
    bool translate_increment = true;
    glm::vec2  location, step, translation, translation_step;
    glm::bvec2 active; // ImGui Checkbox does not like std::vector<bool> types !!!!



};
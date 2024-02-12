#pragma once
// Mandatory for all VW node editor nodes
#include <Universal_FW/ImNodes/Kernal/node.h>

#include <VW_framework/Scene/vw_scene.h>

#include <VW/Editor/Main_Window/Panels/log_panel.h>
#include <VW/Editor/Node_Editor/node_editor_extras.h>

//----------

// Header files unique for Hex Node
#include "hex_surface_node_definitions.h"
#include "../Object/hex_surface_object.h"
#include "../Editor/Menus/hex_surface_main_menu_functions.h"

#include "../Animation/hex_surface_animation.h"


class hex_surface_node_class : public node_basis_class {
public:
    node_status_enum node_status = node_status_enum::none;

    // Mandatory for all VW node editor nodes
    log_panel_class        *log_panel = nullptr; // Define the application log panel to display application messages to
    vw_scene_class         *vw_scene  = nullptr;

    //Unique class that node class is related to
    hex_surface_object_class *hex_surface = nullptr;

    hex_surface_animation_object_class *hex_surface_animation_object = nullptr;
    unsigned int                        timeline_interval_track_id = UINT_MAX;

    bool define_node(ImVec2   click_pos, id_type  entity_id_);

    bool hex_surface_node_class::define_import_node(ImVec2 click_pos, node_id_type entity_id_, void* node_misc_data);

    bool define_inputs(pins_class& pins) {// This probably will not be needed but kept just in case
        pin_struct_type pin; 

        pin.label     = "Shader";
        pin.context   = CONTEXT_ID_SHADER_PROGRAM;
        pin.data_type = DATA_VALUE_TYPE_SHADER_PROGRAM_ID;
        pin.pin_type  = pin_type_enum::single_input;

        pin.pin_color          = IM_COL32(120, 30, 130, 255);
        pin.pin_hovered_color  = IM_COL32(200, 85, 185, 255);
        pin.pin_selected_color = IM_COL32(200, 120, 200, 255);
        pin.pin_shape          = ImNodesPinShape_::ImNodesPinShape_CircleFilled;

        if (add_node_input_pin(pin, pins) == INVALID_ID) return false;
        return true;
    }

    bool define_outputs(pins_class& pins) {// This probably will not be needed but kept just in case
       //return define_node_output_value_of_type("Grid", ENTITY_CATEGORY_HEX_SURF, HEX_CONTEXT_SURFACE_GRID, OPERATOR, HEX_SURFACE_GRID_VALUE, pin_type_enum::single_output, pins);
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


    bool create_hex_surface();

    bool create_hex_surface_timeline_link();
    void delete_hex_surface_timeline_link();

    void delete_node_entity();

    void editor_menu_options();

    bool define_ui();

    // ----------- Export Node parameter data --------------
    void export_node(std::fstream& stream) {
        if (!hex_surface) {
            //error message
            return;
        }

        // Exports must be in the following order
        export_mandatory_node_data(stream);// mandatory node export function that must be present in all graph node exports

        // specific Node data here
        export_hex_surface_node_parameter_data(stream);
        export_hex_surface_node_shader_data(stream);

        export_node_pin_data(stream);// mandatory node export function that must be present in all graph node exports if have pins
    }

    void  export_hex_surface_node_parameter_data(std::fstream& stream) {
        hex_surface_function_import_export_class hex_surface_function_import_export;
        hex_surface_function_import_export.hex_surface_generator_parameters = hex_surface->hex_surface_object_data.hex_surface_generator_parameters;
        hex_surface_function_import_export.write_hex_generation_function_paramater_data(stream);
    }

    void export_hex_surface_node_shader_data(std::fstream& stream) {
        voxel_texture_import_export_class voxel_texture_import_export;
        voxel_texture_import_export.shader_parameters = hex_surface->hex_surface_object_data.hex_surface_shader_parameters;
        voxel_texture_import_export.write_shader_parameter_data(stream);
    }


    // ----------- Import Node parameter data --------------
    bool import_node(std::vector<std::string> lines, int& line_number) {
        // Imports must be in the following order
        if (!import_hcp_voxel_node_parameter_data(lines, line_number)) return false;

        return import_hcp_voxel_node_shader_data(lines, line_number);
    }

    bool import_hcp_voxel_node_parameter_data(std::vector<std::string> lines, int& line_number) {
        if (!hex_surface) return false;

        hex_surface_function_import_export_class hex_surface_import_export;
        line_number++;
        return hex_surface_import_export.read_expression_into_hex_surface_generator_parameters(lines, hex_surface->hex_surface_object_data.hex_surface_generator_parameters, line_number);
    }


    bool import_hcp_voxel_node_shader_data(std::vector<std::string> lines, int& line_number) {
        if (!hex_surface) return false;

        voxel_texture_import_export_class voxel_texture_import_export;
//std::cout << "hcp_voxel_node_class: import_hcp_voxel_node_shader_data 0000 : " << line_number << ":" << lines[line_number] << std::endl;
        line_number++;
        return voxel_texture_import_export.read_shader_parameter_data(lines, &hex_surface->hex_surface_object_data.hex_surface_shader_parameters, line_number);
    }

private:
    hex_surface_main_menu_functions_class hex_suface_main_window_menu_functions;
};

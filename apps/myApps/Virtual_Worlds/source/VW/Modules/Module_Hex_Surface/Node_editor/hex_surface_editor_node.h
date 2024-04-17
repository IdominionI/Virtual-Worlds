#pragma once
// Mandatory for all VW node editor nodes
#include <Universal_FW/ImNodes/Kernal/node.h>

#include <VW_framework/Scene/vw_scene.h>

#include <VW/Editor/Main_Window/Panels/log_panel.h>
#include <VW/Editor/Node_Editor/node_editor_extras.h>


// Header files unique for Voxel Node
#include "hex_surface_node_definitions.h"
#include "../Object/hex_surface_object.h"

#include "../Hex_Editor/Widgets/hex_surface_editor_parameters_widget.h"

class hex_surface_editor_node_class : public node_basis_class {
public:
    
    node_status_enum node_status = node_status_enum::none;

    // Mandatory for all VW node editor nodes
    log_panel_class        *log_panel = nullptr; // Define the application log panel to display application messages to
    vw_scene_class         *vw_scene  = nullptr;

    //Unique class that node class is related to
    hex_surface_object_class *hex_surface = nullptr;

    // node_basis_class functions
    bool define_node(ImVec2 click_pos,node_id_type entity_id_);
    bool define_import_node(ImVec2 click_pos,node_id_type entity_id_,  void *node_misc_data = NULL);
/*
    bool define_inputs(pins_class& pins) {// This probably will not be needed but kept just in case
        pin_struct_type pin;

        pin.label     = "Matrix";
        pin.context   = CONTEXT_ID_hex_surface;
        pin.data_type = DATA_VALUE_TYPE_hex_surface_MATRIX;
        pin.pin_type  = pin_type_enum::single_input;

        pin.pin_color          = IM_COL32(120, 30, 30, 255);
        pin.pin_hovered_color  = IM_COL32(200, 85, 85, 255);
        pin.pin_selected_color = IM_COL32(200, 120, 120, 255);
        pin.pin_shape          = ImNodesPinShape_::ImNodesPinShape_QuadFilled;

        if (add_node_input_pin(pin, pins) == INVALID_ID) return false;

        return true;
    }
*/
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

    bool create_hex_surface();
    void delete_node_entity();

    void editor_menu_options();

    // The display_ui here uses a different voxel_hcp_parameters_widget to the main 
    // and thus will have different parameter values stored. Need to cetralise to access
    // this voxel_hcp_parameters_widget instance, or the central one only
     
    bool define_ui();
    void display_ui(node_id_type current_selected_object_id = -1);
      // ++++++++++++++++
    void* get_node_data() {
        return hex_surface;
    }
    // ++++++++++++++++

    void export_node(std::fstream &stream) {
        if (!hex_surface) {
            //error message
            return;
        }

        // Exports must be in the following order
        export_mandatory_node_data(stream);// mandatory node export function that must be present in all graph node exports

        // specific Node data here
        export_hex_surface_node_parameter_data(stream);
        //export_hex_surface_node_shader_data(stream);

        export_node_pin_data(stream);// mandatory node export function that must be present in all graph node exports if have pins
    }

    void  export_hex_surface_node_parameter_data(std::fstream &stream) {
        hex_surface_function_import_export_class hex_surface_function_import_export;
        hex_surface_function_import_export.hex_surface_generator_parameters = hex_surface->hex_surface_object_data.hex_surface_generator_parameters;
        hex_surface_function_import_export.write_hex_generation_function_paramater_data(stream);
    }

    //void export_hex_surface_node_shader_data(std::fstream &stream) {
    //    //voxel_texture_import_export_class voxel_texture_import_export;
    //    //voxel_texture_import_export.shader_parameters = hex_surface->voxel_object_data.shader_parameters;
    //    //voxel_texture_import_export.write_shader_parameter_data(stream);
    //}

    bool import_node(std::vector<std::string> lines, int& line_number) {
        // Imports must be in the following order
        return import_hex_surface_node_parameter_data(lines, line_number);

        //if (!import_hex_surface_node_parameter_data(lines, line_number)) return false;

        //return import_hex_surface_node_shader_data(lines, line_number);
        //return true;
    }

    bool import_hex_surface_node_parameter_data(std::vector<std::string> lines, int& line_number) {
        if (!hex_surface) return false;

        hex_surface_function_import_export_class hex_surface_function_import_export;
        line_number++;
//std::cout << "hex_surface_editor_node_class: import_hex_surface_node_parameter_data 0101010 !!!!! :" << std::endl;
        return hex_surface_function_import_export.read_expression_into_hex_surface_generator_parameters(lines, hex_surface->hex_surface_object_data.hex_surface_generator_parameters, line_number);
//std::cout << "hex_surface_editor_node_class: import_hex_surface_node_parameter_data 0202020 !!!!! :" << std::endl;
        //return false;
    }


    //bool import_hex_surface_node_shader_data(std::vector<std::string> lines, int& line_number) {
        //if (!hex_surface) return false;

        //voxel_texture_import_export_class voxel_texture_import_export;
//std::cout << "hex_surface_node_class: import_hex_surface_node_shader_data 0000 : " << line_number << ":" << lines[line_number] << std::endl;
        //line_number++;
       // return voxel_texture_import_export.read_shader_parameter_data(lines, &hex_surface->voxel_object_data.shader_parameters,line_number);
        //return false;
    //}

private:
    void display_hex_surface_editor_widget(hex_surface_object_class *voxel_hcp_object_to_execute);

    hex_surface_parameters_editor_widget_class hex_generation_editor_widget;
};

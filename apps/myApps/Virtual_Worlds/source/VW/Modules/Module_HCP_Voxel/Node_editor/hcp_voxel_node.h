#pragma once
// Mandatory for all VW node editor nodes
#include <Universal_FW/ImNodes/Kernal/node.h>


#include <VW_framework/Scene/vw_scene.h>

#include <VW/Editor/Main_Window/Panels/log_panel.h>
#include <VW/Editor/Node_Editor/node_editor_extras.h>


// Header files unique for Voxel Node
#include "hcp_voxel_node_definitions.h"
#include "../Object/voxel_hcp_object.h"
#include "../Editor/Menus/hcp_voxel_menu_functions.h"

#include "../Animation/hcp_voxel_animation.h"

class hcp_voxel_node_class : public node_basis_class {
public:
    
    node_status_enum node_status = node_status_enum::none;

    // Mandatory for all VW node editor nodes
    log_panel_class        *log_panel = nullptr; // Define the application log panel to display application messages to
    vw_scene_class         *vw_scene  = nullptr;

    //Unique class that node class is related to
    voxel_hcp_object_class *hcp_voxel = nullptr;

    hcp_animation_object_class *hcp_animation_object = nullptr;
    unsigned int                timeline_interval_track_id = UINT_MAX;

    // node_basis_class functions
    bool define_node(ImVec2 click_pos,node_id_type entity_id_);
    bool define_import_node(ImVec2 click_pos,node_id_type entity_id_,  void *node_misc_data = NULL);

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

    bool create_hcp_voxel();

    bool create_hcp_timeline_link();
    void delete_hcp_timeline_link();

    void delete_node_entity();

    void editor_menu_options();

    // The display_ui here uses a different voxel_hcp_parameters_widget to the main 
    // and thus will have different parameter values stored. Need to cetralise to access
    // this voxel_hcp_parameters_widget instance, or the central one only
     
    bool define_ui();

    void export_node(std::fstream &stream) {
        if (!hcp_voxel) {
            //error message
            return;
        }

        // Exports must be in the following order
        export_mandatory_node_data(stream);// mandatory node export function that must be present in all graph node exports

        // specific Node data here
        export_hcp_voxel_node_parameter_data(stream);
        export_hcp_voxel_node_shader_data(stream);

        export_node_pin_data(stream);// mandatory node export function that must be present in all graph node exports if have pins
    }

    void  export_hcp_voxel_node_parameter_data(std::fstream &stream) {
        voxel_function_import_export_class voxel_function_import_export;
        voxel_function_import_export.voxel_generator_parameters = hcp_voxel->voxel_object_data.voxel_generator_parameters;
        voxel_function_import_export.write_voxel_generation_function_paramater_data(stream);
    }

    void export_hcp_voxel_node_shader_data(std::fstream &stream) {
        voxel_texture_import_export_class voxel_texture_import_export;
        voxel_texture_import_export.shader_parameters = hcp_voxel->voxel_object_data.shader_parameters;
        voxel_texture_import_export.write_shader_parameter_data(stream);
    }

    bool import_node(std::vector<std::string> lines, int& line_number) {
        // Imports must be in the following order
        if (!import_hcp_voxel_node_parameter_data(lines, line_number)) return false;
        
        return import_hcp_voxel_node_shader_data(lines, line_number);
    }

    bool import_hcp_voxel_node_parameter_data(std::vector<std::string> lines, int& line_number) {
        if (!hcp_voxel) return false;

        voxel_function_import_export_class voxel_function_import_export;
        line_number++;
        return voxel_function_import_export.read_expression_into_voxel_generator_parameters(lines, hcp_voxel->voxel_object_data.voxel_generator_parameters, line_number);
    }


    bool import_hcp_voxel_node_shader_data(std::vector<std::string> lines, int& line_number) {
        if (!hcp_voxel) return false;

        voxel_texture_import_export_class voxel_texture_import_export;
//std::cout << "hcp_voxel_node_class: import_hcp_voxel_node_shader_data 0000 : " << line_number << ":" << lines[line_number] << std::endl;
        line_number++;
        return voxel_texture_import_export.read_shader_parameter_data(lines, &hcp_voxel->voxel_object_data.shader_parameters,line_number);
    }

private:
    voxel_main_window_menu_functions_class voxel_main_window_menu_functions;
};

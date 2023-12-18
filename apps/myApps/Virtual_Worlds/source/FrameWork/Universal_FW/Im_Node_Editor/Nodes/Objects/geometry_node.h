#pragma once

#include "../../Node_Editors/node_editor_definitions.h"
#include "node_object_definitions.h"

#include <Universal_FW/ImNodes/Kernal/node.h>

class geometry_node_class : public node_basis_class {
public:
    geometry_node_class() {
        node_width = 60.0f;
    }

    bool define_node(ImVec2         click_pos,
                     entity_id_type _entity_id) {

        entity_id = _entity_id;

       // object_type = object_type_enum::geometry;

        ui_node_type.node_type    = ENTITY_NODE_CATEGORY_GEOMETRY;
        ui_node_type.node_context = ENTITY_NODE_CONTEXT_GEOMETRY;
        //ui_node_type.function_category = ENTITY_NODE_FUNCTION_CATEGORY_GEOMETRY;
        //ui_node_type.function          = ENTITY_NODE_FUNCTION_GEOMETRY;

        ImNodes::SetNodeScreenSpacePos(node_id, click_pos);
        return true;
    }


    bool define_inputs(pins_class& pins) {
        return define_node_input_value_of_type("Material", ENTITY_NODE_CONTEXT_MATERIAL, ENTITY_NODE_DATA_MATERIAL, pin_type_enum::single_input, pins);
    }

    bool define_outputs(pins_class& pins) {
        return define_node_output_value_of_type("Geometry", ENTITY_NODE_CONTEXT_GEOMETRY, ENTITY_NODE_DATA_GEOMETRY, pin_type_enum::single_output,  pins);
    }

    bool define_parent(pins_class& pins)   { 
        return define_node_output_value_of_type("Parent", ENTITY_NODE_CONTEXT_OBJECT, ENTITY_NODE_DATA_OBJECT, pin_type_enum::parent, pins);
    }
    bool define_children(pins_class& pins) {
        return define_node_input_value_of_type("Material", ENTITY_NODE_CONTEXT_OBJECT, ENTITY_NODE_DATA_OBJECT, pin_type_enum::children, pins);
    }


    bool draw_node(pins_class pins)
    {
//printf("hcp_voxel_node_class::draw_node:: 000\n");
        // Define titlebare colors
        ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(0, 139, 139, 255));
        ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(0, 175, 175, 255));
        ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(0, 255, 255, 255));

        ImNodes::BeginNode(node_id);
 //printf("hcp_voxel_node_class::draw_node:: 000\n");
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));// Node header text color
            draw_node_header();
            ImGui::PopStyleColor();
//printf("hcp_voxel_node_class::draw_node:: 111\n");
            ImGui::Dummy(ImVec2(node_width, node_height));

//printf("hcp_voxel_node_class::draw_node:: 222\n");

            ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(120, 30, 30, 255));
            ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(200, 85, 85, 255));
            draw_node_input_link(inputs[0],pins, ImNodesPinShape_::ImNodesPinShape_QuadFilled);
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
// printf("hcp_voxel_node_class::draw_node:: 333\n");

            ImGui::Spacing();

            ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(100, 30, 150, 255));
            ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(160, 40, 230, 255));
            draw_node_output_link(outputs[0], pins, ImNodesPinShape_::ImNodesPinShape_QuadFilled);
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();

            // Place here draw parent-child pins

        ImNodes::EndNode();

        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();


        return true;
    }

   //voxel_hcp_parameters_widget_class   voxel_hcp_parameters_widget;

    // The display_ui here uses a different voxel_hcp_parameters_widget to the main 
    // and thus will have different parameter values stored. Need to cetralise to access
    // this voxel_hcp_parameters_widget instance, or the central one only

 //   void display_ui(scene_manager_class *scene_manager = NULL, log_panel_class* log_panel = NULL, id_type current_selected_object_id = -1) {
 //printf("display_ui HCP VOxel 0000\n");
 /*       if (log_panel == NULL) {
            printf("ERROR: No log_panel defined to display messages in the log panbel from the node editor\n");
        }

        if (scene_manager == NULL) {
            if (log_panel != NULL) 
                log_panel->application_log.AddLog("ERROR: No scene manager defined to display hcp Voxel paramater data from the node editor\n");
            else
                printf("ERROR: No scene manager defined to display hcp Voxel paramater data from the node editor\n");
            return;
        }

        if (current_selected_object_id == NULL) {
            if (log_panel != NULL)
                log_panel->application_log.AddLog("ERROR: No current_selected HCP voxel entity defined to display hcp Voxel paramater data from the node editor\n");
            else
                printf("ERROR: No current_sel;ected HCP voxel entity defined to display hcp Voxel paramater data from the node editor\n");
            return;
        }

        //voxel_hcp_parameters_widget_class   voxel_hcp_parameters_widget;
 //printf("display_ui HCP VOxel 111111\n");
 		voxel_hcp_parameters_widget.scene_manager = scene_manager;
		voxel_hcp_parameters_widget.log_panel     = log_panel;

        voxel_hcp_parameters_widget.display_voxel_hcp_generation_widget(*current_selected_object_id);
*/
 //   }





private:
};

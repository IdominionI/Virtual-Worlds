#pragma once

#include "../node_editor_menus.h"
#include "../../Nodes/Objects/node_object_definitions.h"

class object_node_editor_menu_class : public Node_editor_menu_basis_class {
public:
    object_node_editor_menu_class() { menu_name = "Object node editor "; }

    void define_menu_items(node_type_struct& editor_node_definition, scene_node_editor_function_enum& scene_node_editor_function) {
        //static void HCP_Voxel_test_menu_items(std::function <void(int, int, int, int, ImVec2)>(create_node), ImVec2 click_pos){
        if (ImGui::MenuItem("Add Geometry Object")) {
            editor_node_definition = define_node_definition(ENTITY_NODE_CATEGORY_GEOMETRY, ENTITY_NODE_CONTEXT_GEOMETRY);
            scene_node_editor_function = scene_node_editor_function_enum::create_object_node;
        }

        if (ImGui::MenuItem("Add Light Object")) {
            editor_node_definition = define_node_definition(ENTITY_NODE_CATEGORY_LIGHT, ENTITY_NODE_CONTEXT_LIGHT);
            scene_node_editor_function = scene_node_editor_function_enum::create_object_node;
        }

        if (ImGui::MenuItem("Add Material Object")) {
            editor_node_definition = define_node_definition(ENTITY_NODE_CATEGORY_MATERIAL, ENTITY_NODE_CONTEXT_MATERIAL);
            scene_node_editor_function = scene_node_editor_function_enum::create_object_node;
        }

    }

protected:
private:

};
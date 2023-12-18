#pragma once
//#include <Kernels/ImGuiEx/menus.h>

//#include "ImNodes/Kernal/node.h"
#include <Universal_FW/ImNodes/Kernal/node.h>
#include "node_editor_definitions.h"

class Node_editor_menu_basis_class  {
public:
    std::string    menu_name = "menu";
    int            menu_id   = -1;

    virtual void define_menu_items(node_type_struct &editor_node_definition, scene_node_editor_function_enum &scene_node_editor_function) {
        //static void HCP_Voxel_test_menu_items(std::function <void(int, int, int, int, ImVec2)>(create_node), ImVec2 click_pos){
        if (ImGui::MenuItem("Menu Item01")) {
            editor_node_definition = define_node_definition(INVALID_NODE_CATEGORY_ID, INVALID_NODE_CONTEXT_ID);
            scene_node_editor_function = scene_node_editor_function_enum::none;
        }

        if (ImGui::MenuItem("Menu Item02")) {
            editor_node_definition = define_node_definition(INVALID_NODE_CATEGORY_ID, INVALID_NODE_CONTEXT_ID);
            scene_node_editor_function = scene_node_editor_function_enum::none;
        }

        if (ImGui::MenuItem("Menu Item03")) {
            editor_node_definition = define_node_definition(INVALID_NODE_CATEGORY_ID, INVALID_NODE_CONTEXT_ID);
            scene_node_editor_function = scene_node_editor_function_enum::none;
        }
    }

    // Put other functions to create specific virtual menu item creation functions here

    virtual void define_imgui_menu(node_type_struct &editor_node_definition, scene_node_editor_function_enum &scene_node_editor_function,
                                    std::string label = "node editor menu", bool enabled = true) {
        if (ImGui::BeginMenu(label.c_str(), enabled)) {
            {
                define_menu_items(editor_node_definition, scene_node_editor_function);

                ImGui::EndMenu();
            }
        }
    }



protected:

    void menu_seperator() { ImGui::Separator(); }

    void menu_label(std::string label) { ImGui::MenuItem(label.c_str(), NULL, false, false); }

    node_type_struct define_node_definition(int category, int context) {

        //select existing voxel entity to reference and get entity id
//printf("scene_node_editor_class::create_entity_node 0000\n");
        node_type_struct editor_node_definition;
        editor_node_definition.node_type = category;
        editor_node_definition.node_context  = context;
        //editor_node_definition.function = function;
        //editor_node_definition.function_category = function_category;

        return editor_node_definition;
    }

};
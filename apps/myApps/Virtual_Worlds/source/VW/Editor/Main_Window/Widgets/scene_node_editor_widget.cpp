#include "scene_node_editor_widget.h"

#include <FrameWork/VW_framework/App/global.h>
#include <FrameWork/Universal_FW/Tools/dialogs.h>

// Place node headerfiles here
// hcp Voxel nodes
#include <VW/Modules/Module_HCP_Voxel/Node_editor/hcp_voxel_node.h>
#include <VW/Modules/Module_HCP_Voxel/Node_editor/cart_to_hcp_voxel_node.h>
#include <VW/Modules/Module_HCP_Voxel/Node_editor/hcp_voxel_automata_node.h>
// hcp Voxel modifiers
#include <VW/Modules/Module_HCP_Voxel/Node_editor/Modifiers/translation_node.h>
// hcp Voxel operators
#include <VW/Modules/Module_HCP_Voxel/Node_editor/Operators/interaction_node.h>


// hex suface nodes
#include <VW/Modules/Module_Hex_Surface/Node_editor/hex_surface_node.h>
#include <VW/Modules/Module_Hex_Surface/Node_editor/hex_surface_automata_node.h>
//hex surface modifiers
#include <VW/Modules/Module_Hex_Surface/Node_editor/Modifiers/translation_node.h>
//hex surface operators
#include <VW/Modules/Module_Hex_Surface/Node_editor/Operators/interaction_node.h>

// Critical these includes are placed here in this order
#include "../../Node_Editor/node_editor_extras.h"
#include "../../Node_Editor/node_editor_functions.h"
#include "../../Node_Editor/node_editor_import_export.h"

node_editor_evaluation_functions_class evaluate;

void scene_node_editor_class::display() {
//printf("scene_node_editor_class:: display : TEST : %i : %i \n", global_sclass::get_current_selected_entity_id(), global_sclass::get_current_selected_entity_type_id());

    if (!vw_scene) {
        if (log_panel) log_panel->application_log.AddLog("ERROR: No scene manager defined to create node in node editor\n");
        return;
    }

    BeginNodeEditor();

    // MouseInCanvas() takes into account of over laying editor canvases and gives correct 
    // result, but all node editor functionality is gone if used as a test for any node editor
    // functions or assignments.  Ridiculous, absurd. Not know if ImnNode or ImGUI problem.
    if (MouseInCanvas()) {
        current_active_editor = editor_id;
    }

    // Right-click on blank space of the node editor
    // Create a menu to perform actions if nothing is selected and right mouse pressed
    if (no_node_element_is_hovered_over && selected_node == NULL && selected_link.link_id == INVALID_ID && selected_pin.pin_id == INVALID_ID) { // This menu is for when no node or link is hovered over

        //if (ImGui::BeginPopupContextWindow(0, 1, false)) {
        if (ImGui::BeginPopupContextWindow(0, 1)) {
            ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();
            menu_selection = true;

            if (ImGui::BeginMenu("Main Menu")) {
                {
                    if (ImGui::BeginMenu("Create Node ...")) {
                        if (ImGui::BeginMenu("HCP Voxel ...")){
                            if (ImGui::MenuItem("Create HCP Voxel Node")) {
                                create_node<hcp_voxel_node_class>( click_pos);
                            }

                            if (ImGui::MenuItem("Create Cart to HCP Voxel Node")) {
                                create_node<cart_to_hcp_voxel_node_class>(click_pos);
                            }

                            if (ImGui::MenuItem("Create HCP Voxel Automata Node")) {
                                create_node<hcp_voxel_automata_node_class>(click_pos);
                            }

                            if (ImGui::MenuItem("Create HCP Voxel Translation Node")) {
                                create_node<hcp_voxel_translation_node_class>(click_pos);
                            }

                            if (ImGui::MenuItem("Create HCP Voxel Interaction Node")) {
                                create_node<hcp_voxel_interaction_node_class>(click_pos);
                            }

                            ImGui::EndMenu();
                        }

                        if (ImGui::BeginMenu("Hex Surface ...")) {
                            if (ImGui::MenuItem("Create Hex suface Node")) {
                                create_node<hex_surface_node_class>(click_pos);
                            }

                            //if (ImGui::MenuItem("Create Cart to Hex suface Node")) {
                            //    //create_node<cart_to_hcp_voxel_node_class>(click_pos);
                            //}

                            if (ImGui::MenuItem("Create Hex suface Translation Node")) {
                                create_node<hex_surface_translation_node_class>(click_pos);
                            }

                            if (ImGui::MenuItem("Create Hex suface Interaction Node")) {
                                create_node<hex_surface_interaction_node_class>(click_pos);
                            }

                            if (ImGui::MenuItem("Create Hex suface Automata Node")) {
                                create_node<hex_surface_automata_node_class>(click_pos);
                            }

                            ImGui::EndMenu();
                        }


                        if (ImGui::MenuItem("Create Group Node")) {
                            create_group_node(click_pos);
                        }

                        if (ImGui::MenuItem("import Group")) {
                            import_group(this, click_pos);
//std::cout << "scene_node_editor_class : after import group 000" << std::endl;
                        }

                        ImGui::EndMenu();
                    }

                    if (ImGui::MenuItem("Create Group Node")) {
                        create_group_node(click_pos);
                    }

                    if (ImGui::MenuItem("evaluate Scene")) {
                        if (evaluate.create_node_editor_evaluation_tree(this)) {
                            evaluate.evaluate_node_tree();
                        }
                    }
                        
                    if (ImGui::MenuItem("Save Scene")) {

                    }

                    if (ImGui::MenuItem("Import Scene")) {

                    }

                    ImGui::EndMenu();
                }
            };
/*
            if (ImGui::BeginMenu("HCP Voxel ...")) {
                {
                    if (ImGui::MenuItem("Create node for currently selected voxel")) 
                        create_node(ENTITY_CATEGORY_HCP_VOXEL,HCP_CONTEXT_VOXEL_MATRIX, HCP_VOXEL_FUNCTION_ENTITY, FUNCTION_CATEGORY_GENERATOR, click_pos);
                    if (ImGui::MenuItem("Translation")) 
                        create_node(ENTITY_CATEGORY_HCP_VOXEL, HCP_CONTEXT_VOXEL_MATRIX, HCP_VOXEL_FUNCTION_TRANSLATION, FUNCTION_CATEGORY_OPERATOR, click_pos);
                    if (ImGui::MenuItem("Boolean"))     
                        create_node(ENTITY_CATEGORY_HCP_VOXEL, HCP_CONTEXT_VOXEL_MATRIX, HCP_VOXEL_FUNCTION_INTERACTION, FUNCTION_CATEGORY_MODIFIER, click_pos);

                    ImGui::EndMenu();
                }
            };

            if (ImGui::BeginMenu("Hex Surface ...")) {
                {
                    if (ImGui::MenuItem("Create node for currently selected hex_surface"))
                        create_node(ENTITY_CATEGORY_HEX_SURF, HEX_CONTEXT_SURFACE_GRID, HEX_SURFACE_FUNCTION_ENTITY, FUNCTION_CATEGORY_GENERATOR, click_pos);
                    if (ImGui::MenuItem("Translation")) 
                        create_node(ENTITY_CATEGORY_HEX_SURF, HEX_CONTEXT_SURFACE_GRID, HEX_SURFACE_FUNCTION_TRANSLATION, FUNCTION_CATEGORY_OPERATOR, click_pos);
                    if (ImGui::MenuItem("Boolean"))     
                        create_node(ENTITY_CATEGORY_HEX_SURF, HEX_CONTEXT_SURFACE_GRID, HEX_SURFACE_FUNCTION_INTERACTION, FUNCTION_CATEGORY_MODIFIER, click_pos);
                    ImGui::EndMenu();
                }
            };
*/
            ImGui::EndPopup();
            GImNodes.LeftMouseClicked = false;
            menu_selection = false;
        }
    }

    // Right click on a selected node to display a menu selection of actions to perform on that node
    if (selected_node != NULL) {

        //if (ImGui::BeginPopupContextWindow(0, 1, false)) {
        if (ImGui::BeginPopupContextWindow(0, 1)) {
            menu_selection = true;

            // Menu selection when have a node group selected
            if (selected_node->ui_node_type.node_type == NODE_TYPE_ID_GROUP) {
                if (ImGui::BeginMenu("Group Node ...")) {          
                    if (ImGui::MenuItem("Delete group")) {
                        if (current_selected_node_id > -1) {
                            delete_group_node(selected_node);
                        }
                    }

                    if (ImGui::MenuItem("delete group links")) {
                        if (current_selected_node_id > -1) {
                            graph.delete_node_links(current_selected_node_id);
                        }
                    }

                    if (ImGui::MenuItem("Expand group node")) {
                        expand_group_node(current_selected_node_id);
                    }

                    if (ImGui::MenuItem("Collapse group node")) {
                        collapse_group_node(current_selected_node_id);
                    }

                    if (ImGui::MenuItem("Save Group")) {
                        save_group(selected_node);
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Right)) {
                    toggle_group_node_visibility(current_selected_node_id);
                }

            }

            // If selected node is an entity node, then these options are available to be performed
            if (selected_node->node_id > INVALID_ID && selected_node->ui_node_type.node_type != NODE_TYPE_ID_GROUP 
                && selected_node->ui_node_type.node_type != NODE_TYPE_ID_GROUP_INPUT && selected_node->ui_node_type.node_type != NODE_TYPE_ID_GROUP_OUTPUT) {
                if (ImGui::BeginMenu("Node/Entity ...")) {
                    if (ImGui::MenuItem("delete")) {
//printf("scene_node_editor_class : display : save entity selected\n");
                        delete_node(selected_node);
                    }

                    if (ImGui::BeginMenu("Functions ...")) {
//printf("scene_node_editor_class : display : save entity selected\n");
                        selected_node->editor_menu_options();

                        ImGui::EndMenu();
                    }

                    if (ImGui::MenuItem("save node tree")){
//printf("scene_node_editor_class : display : save entity selected\n");
                        // Not yet implemented    
                    }

                    ImGui::EndMenu();
                }

            }

            ImGui::EndPopup();
            GImNodes.LeftMouseClicked = false;
            menu_selection = false;
        }
    }

    if (selected_link.link_id != INVALID_ID) {

        if (ImGui::BeginPopupContextWindow(0, 1)) {
            menu_selection = true;

            if (ImGui::BeginMenu("Link ...")) {
                {
                    if (ImGui::MenuItem("Delete link")) {
                        if (selected_link.link_id > -1) {
                            delete_link(selected_link.link_id);
                            display_graph_status(this);// testing only
                        }
                    }
                    ImGui::EndMenu();
                }
            };

            ImGui::EndPopup();
            GImNodes.LeftMouseClicked = false;
            menu_selection = false;
        }
    }

    if (selected_pin.pin_id != INVALID_ID) {
//std::cout << "Scene node editor : MENU hovered_pin_id != INVALID_ID " << std::endl;
        if (ImGui::BeginPopupContextWindow(0, 1)) {
            //ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();
            menu_selection = true;
//std::cout << "Scene node editor : BeginPopupContextWindow : " << std::endl;      
            node_basis_class *pin_node = graph.nodes.get_node(selected_pin.node_id);
            if (pin_node) {
//std::cout << "Scene node editor : pin node != NULL : " << pin_node->node_id << std::endl;
                if (selected_pin.pin_type == pin_type_enum::single_input) {
//std::cout << "Scene node editor : input pin hovered : " << hover_pin_id << std::endl;  
                    if (pin_node->ui_node_type.node_type != NODE_TYPE_ID_GROUP_INPUT && pin_node->ui_node_type.node_type != NODE_TYPE_ID_GROUP_OUTPUT) {
                        if (ImGui::BeginMenu("Node Input Socket ...", true)) {
//std::cout << "Scene node editor : BeginMenu : " << std::endl;
                            if (ImGui::MenuItem("Add To Group Node Inputs")) {
//std::cout << "Scene node editor : Add To Group Node Inputs : " << hover_pin_id << std::endl;
                                add_group_node_input_pin(selected_pin);
                            }

                            // More menu items here as required

                            ImGui::EndMenu();
                        }
                    }
                    else {
                        if (pin_node->ui_node_type.node_type == NODE_TYPE_ID_GROUP_OUTPUT) {
                            if (ImGui::BeginMenu("Node Output Socket ...", true)) {
                                if (ImGui::MenuItem("Delete Group Node Output")) {
//std::cout << "Scene node editor : Delete To Group Node Input : " << hover_pin_id << std::endl;
                                    delete_group_node_output_pin(selected_pin);
                                }

                                // More menu items here as required

                                ImGui::EndMenu();
                            }
                        }
                    }
                } else {
                    if (selected_pin.pin_type == pin_type_enum::single_output) {
//std::cout << "Scene node editor : output pin hovered : " << hover_pin_id << std::endl;
                        if (pin_node->ui_node_type.node_type == NODE_TYPE_ID_GROUP_INPUT) {
//std::cout << "Scene node editor :pin_node->ui_node_type.node_type != NODE_TYPE_ID_GROUP_INPUT : " << std::endl;
                            if (ImGui::BeginMenu("Node Input Socket ...", true)) {
//std::cout << "Scene node editor : BeginMenu : " << std::endl;
                                if (ImGui::MenuItem("delete Node Group Input")) {
//std::cout << "Scene node editor : Add To Group Node Outputs : " << hover_pin_id << std::endl;
                                    delete_group_node_input_pin(selected_pin);
                                }

                                // More menu items here as required

                                ImGui::EndMenu();
                            }
                        }
                        else {
                            if (pin_node->ui_node_type.node_type != NODE_TYPE_ID_GROUP_OUTPUT && pin_node->ui_node_type.node_type != NODE_TYPE_ID_GROUP_INPUT) {
                                if (ImGui::BeginMenu("Node Output Socket ...", true)) {
                                    if (ImGui::MenuItem("Add Group Node Output")) {
//std::cout << "Scene node editor : Delete To Group Node Output : " << hover_pin_id << std::endl;
                                        add_group_node_output_pin(selected_pin);
                                    }

                                    // More menu items here as required

                                    ImGui::EndMenu();
                                }
                            }
                        }
                    }
                }

            }

            ImGui::EndPopup();
            GImNodes.LeftMouseClicked = false;
            menu_selection = false;
        }
    }

//printf("scene_node_editor_class::display:: 00000\n");
    // Draw nodes
    ImGui::SetWindowFontScale(zoom_scale);
    for (node_basis_class* node : graph.nodes.nodes) {
        node->draw_node(graph.pins, zoom_scale);
    }
    ImGui::SetWindowFontScale(1.0f);

//printf("scene_node_editor_class::display:: 11111\n");

    // Display existing link lines
    for (link_struct_type link : graph.links.links)
    {
        // If edge doesn't start at value, then it's an internal edge, i.e.
        // an edge which links a node's operation to its input. We don't
        // want to render node internals with visible links.

        PushColorStyle(ImNodesCol_Link, IM_COL32(0, 139, 139, 255));
        PushColorStyle(ImNodesCol_LinkHovered, IM_COL32(0, 175, 175, 255));
        PushColorStyle(ImNodesCol_LinkSelected, IM_COL32(100, 255, 255, 255));
        Link(link.link_id, link.from, link.to);
        PopColorStyle();
        PopColorStyle();
        PopColorStyle();
    }

    zoom_on = false;
//printf("scene_node_editor_class::display:: 22222\n");
    EndNodeEditor();

    // ---------------------------------------------------------------------------------
    // These are driven by Imnodes, so we place the code after EndNodeEditor().

//printf("scene_node_editor_class::display:: 333333\n");
    // Handle links
    handle_new_links();
    handle_deleted_links();


    no_node_element_is_hovered_over = no_element_hovered_over();

    if (GImNodes.LeftMouseReleased && inside_editor_canvas(GImNodes.MousePos) && !menu_selection) {
//printf("Left mouse clicked inside canvas 00 %f : %f \n", nodes_context->MousePos.x, nodes_context->MousePos.y);
//printf("Left mouse clicked 11 %f : %f \n", nodes_context->CanvasOriginScreenSpace.x, nodes_context->CanvasOriginScreenSpace.y);
//printf("Left mouse clicked 22 %f : %f \n", nodes_context->CanvasRectScreenSpace.Min.x, nodes_context->CanvasRectScreenSpace.Max.x);
//printf("Left mouse clicked 33 %f : %f \n", nodes_context->CanvasRectScreenSpace.Min.y, nodes_context->CanvasRectScreenSpace.Max.y);
        if (current_selected_node_id == INVALID_ID) {
            globalc::set_current_selected_data_context_id(INVALID_ID);
            globalc::set_current_selected_entity_type_id(INVALID_ID);
            globalc::set_current_selected_entity_id(INVALID_ID);
            globalc::set_current_selected_entity_category_type_id(INVALID_ID);// ****
            // globalc::set_outliner_selection(false);
        }
        else {
//std::cout << "GImNodes.LeftMouseReleased && inside_editor_canvas(GImNodes.MousePos) && !menu_selection : " << editor_id << std::endl;
            display_node_ui(selected_node);
        }
    }

    // Only way to avoid clashes and incorrect display
    // of node parameter data or no display at all by use
    // of this test. 
    if (current_active_editor == editor_id) {
        num_selected_nodes = NumSelectedNodes();
        num_selected_links = NumSelectedLinks();
    } else {
        num_selected_nodes = 0;
        num_selected_links = 0;
    }

    if (num_selected_nodes > 0)
    {
        static std::vector<int> selected_nodes;
        selected_nodes.resize(static_cast<size_t>(num_selected_nodes));
        GetSelectedNodes(selected_nodes.data());
        for (const int node_id : selected_nodes)
        {
//if (node_id < 0) ImNodes::ClearNodeSelection(); break; // use if selected nodes not cleared and have invalid node id
            current_selected_node_id = selected_nodes[selected_nodes.size() - 1];
//printf("selected node id : %i : %i\n", node_id, current_selected_node_id);
//if (log_panel != NULL) log_panel->application_log.AddLog("selected node id : %i: %i\n", node_id, current_selected_node_id);

        }
    }
    else {
        current_selected_node_id = INVALID_ID;
        selected_node = NULL;
//printf("current node id : %i \n", current_selected_node_id);
    }

    if (current_selected_node_id > INVALID_ID) {
//printf("Found current node of id : %i  \n", current_selected_node_id);
        node_basis_class* node;
        node = graph.nodes.get_node(current_selected_node_id);

        if (node != NULL) {
//printf("Found current node of id : %i : %i : %i  \n", node->node_id, node->ui_node_type.category, node->entity_id);
            globalc::set_current_selected_entity_type_id(node->node_entity_type_id);
            globalc::set_current_selected_entity_id(node->node_entity_id);
            globalc::set_current_selected_entity_category_type_id(node->node_entity_category_id);
            globalc::set_current_selected_data_context_id(node->ui_node_type.node_data_context);// ****
            //current_selected_entity_id = node->entity_id;
            //current_selected_entity_type_id = node->ui_node_type.category;

            selected_node = node;
        }
    }

    if (num_selected_links > 0 )
    {
//printf("num_selected_links : %i \n", num_selected_links);
        static std::vector<int> selected_links;
        selected_links.resize(static_cast<size_t>(num_selected_links));
        //ImNodes::GetSelectedLinks(selected_links.data());
        GetSelectedLinks(selected_links.data());
        for (const int link_id : selected_links)
        {
//if (node_id < 0) ImNodes::ClearNodeSelection(); break; // use if selected nodes not cleared and have invalid node id
            current_selected_link_id = selected_links[selected_links.size() - 1];
//printf("selected link id : %i : %i\n", link_id, current_selected_link_id);
//if (log_panel != NULL) log_panel->application_log.AddLog("selected node id : %i: %i\n", node_id, current_selected_node_id);
        }
    }
    else {
        current_selected_link_id = -1;
        selected_link.link_id = INVALID_ID;
//printf("current node id : %i \n", current_selected_node_id);
    }

    if (current_selected_link_id > -1) {
//printf("current_selected_link_id : %i  \n", current_selected_link_id);
        link_struct_type link;
        link = graph.links.find_link(current_selected_link_id);

        if (link.link_id != INVALID_ID) {
//printf("Found current link of id : %i  \n", link.link_id);
            selected_link = link;
        }
    }

    if (IsPinHovered(&hover_pin_id) && ImGui::IsMouseDown(RIGHT_MOUSE_BUTTON)) {
//std::cout << "Scene node editor : pin hovered : " << hovered_pin_id << std::endl;
        selected_pin = graph.pins.find_pin(hover_pin_id);
    } else {
        if(!IsPinHovered(&hover_pin_id) && ImGui::IsMouseDown(RIGHT_MOUSE_BUTTON))
            selected_pin.pin_id = INVALID_ID;
    }

    // These must be placed here at the end of all scene node editing functions 
    if (inside_editor_canvas(GImNodes.MousePos) && !menu_selection) {
        if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsMouseDown(MIDDLE_MOUSE_BUTTON)) ||
            (ImGui::IsKeyDown(ImGuiKey_RightCtrl) && ImGui::IsMouseDown(MIDDLE_MOUSE_BUTTON))) {
            update_canvas_zoom(RESTORE);
            zoom_on = true;
            GImNodes.EditorCtx->zoom_scale = zoom_scale;
        }

        if (ImGui::GetIO().MouseWheel != 0.0) {
            update_canvas_zoom();
            zoom_on = true;
            GImNodes.EditorCtx->zoom_scale = zoom_scale;
        }
    }
}

//----------------------------------------------
node_group_class *scene_node_editor_class::create_group_node(ImVec2 click_pos,bool create_import_group_node) {
    node_group_class *new_node_group = new node_group_class;
    if (new_node_group == nullptr) {
        //error message
        return nullptr;
    }

    new_node_group->graph = &graph;

//std::cout << "create_group_node 000\n";
    graph.add_node(new_node_group, click_pos, INVALID_ID);// Need to define an entity ID for node and not have an INVALID_ID passed to the graph class ???
    
    scene_node_editor_class *group_scene_node_editor = new scene_node_editor_class;
    if (group_scene_node_editor == nullptr) {
        //error message
        delete new_node_group;
        return nullptr;
    }
    group_scene_node_editor->parent_group = this;
    group_scene_node_editor->editor_id    = new_node_group->node_id;
    group_scene_node_editor->display_on   = false;
    group_scene_node_editor->log_panel    = log_panel;
    group_scene_node_editor->vw_scene     = vw_scene;
    // Add link nodes to group_scene_node_editor here 

    child_groups.push_back(group_scene_node_editor);
    scene_node_editors.push_back(group_scene_node_editor);

//std::cout << "create_group_node 001 : " << graph.pins.input_pins.size() << " : " << graph.pins.output_pins.size() << std::endl;
    // **** ADD LINK NODES ******
    if(!create_import_group_node){ // only create link nodes if not creating group node from imported file
        group_scene_node_editor->input_link_node = new group_link_node_class;
        group_scene_node_editor->input_link_node->ui_node_type.node_type         = NODE_TYPE_ID_GROUP_INPUT; // Need this defined here before calling graph.add_node so as to define which group link is being created
        group_scene_node_editor->input_link_node->graph = &group_scene_node_editor->graph;
        group_scene_node_editor->input_link_node->parent_group_node_graph = &graph;

        group_scene_node_editor->graph.add_node(group_scene_node_editor->input_link_node, click_pos, INVALID_ID);// Need to define an entity ID for node and not have an INVALID_ID passed to the graph class ???

        group_scene_node_editor->output_link_node = new group_link_node_class;
        group_scene_node_editor->output_link_node->ui_node_type.node_type        = NODE_TYPE_ID_GROUP_OUTPUT; // Need this defined here before calling graph.add_node so as to define which group link is being created
        group_scene_node_editor->output_link_node->graph = &group_scene_node_editor->graph;
        group_scene_node_editor->output_link_node->parent_group_node_graph = &graph;

        group_scene_node_editor->graph.add_node(group_scene_node_editor->output_link_node, click_pos, INVALID_ID);// Need to define an entity ID for node and not have an INVALID_ID passed to the graph class ???
     }

    display_graph_status(this);// testing only
    return new_node_group;
}

template <class T> // Tnis class type must be a derivative node of node_basis_class
void scene_node_editor_class::create_node(ImVec2 click_pos) {
    T *new_node = new T;

    if (!new_node) {
        // error message
        return;
    }

    new_node->graph = &graph;

    // Need to do something about this as not all nodes will have a log panel or need a vw_scene defined 
    new_node->log_panel = log_panel;
    new_node->vw_scene  = vw_scene;

    graph.add_node(new_node, click_pos, INVALID_ID);
    display_graph_status(this);// testing only
}

void scene_node_editor_class::delete_node(node_basis_class *selected_node) {
    if (!selected_node) return;

    delete_node_group_links(selected_node);

    // selected_node->delete_node_entity();// not needed as this is called within 
                                           // graph.delete_node(selected_node->node_id)
    graph.delete_node(selected_node->node_id);
    selected_node = NULL;

    globalc::set_current_selected_data_context_id(INVALID_ID);
    globalc::set_current_selected_entity_id(INVALID_ID);
    globalc::set_current_selected_entity_category_type_id(INVALID_ID);
    globalc::set_current_selected_entity_type_id(INVALID_ID);

    globalc::set_current_selected_node(nullptr);

    no_node_element_is_hovered_over = true;
    current_selected_node_id = INVALID_ID;
}

void scene_node_editor_class::delete_node_group_links(node_basis_class *selected_node) {
    for (int pin_id : selected_node->inputs) {
        for (link_struct_type link : graph.links.links) {
            if (link.to == pin_id ) {
                pin_struct_type from_pin    = graph.pins.find_pin(link.from);
                node_basis_class *from_node = graph.nodes.get_node(from_pin.node_id);
                if(from_node->ui_node_type.node_type == NODE_TYPE_ID_GROUP_INPUT)
                    delete_group_node_input_pin(from_pin);// Deletes link as well as pin in this function
            }
        }
    }

    for (int pin_id : selected_node->outputs) {
        for (link_struct_type link : graph.links.links) {
            if (link.from == pin_id) {
                pin_struct_type to_pin    = graph.pins.find_pin(link.to);
                node_basis_class *to_node = graph.nodes.get_node(to_pin.node_id);
                if (to_node->ui_node_type.node_type == NODE_TYPE_ID_GROUP_OUTPUT)
                    delete_group_node_output_pin(to_pin);// Deletes link as well as pin in this function
            }
        }
    }
}

void scene_node_editor_class::delete_link(int link_id) {

    bool delete_link_only = true;
    link_struct_type link = graph.links.find_link(link_id);
//std::cout << "scene_node_editor_class::handle_deleted_links() 000 : " << link.link_id << std::endl;
    if (link.link_id != INVALID_ID) { // Just in case to avoid crash
        pin_struct_type  from_pin = graph.pins.find_pin(link.from);
        pin_struct_type  to_pin   = graph.pins.find_pin(link.to);

//std::cout << "scene_node_editor_class::handle_deleted_links() 111 : " << from_pin.node_id << ":" << to_pin.node_id << std::endl;
        if (from_pin.node_id != INVALID_ID) {
            node_basis_class *from_node = graph.nodes.get_node(from_pin.node_id);
            if (from_node) {
//std::cout << "scene_node_editor_class::handle_deleted_links() 222 : " << from_node->node_id << ":" << from_node->ui_node_type.category << ":" << from_node->ui_node_type.node_type << std::endl;
                if (from_node->ui_node_type.category == NODE_CATEGORY_ID_GROUP && from_node->ui_node_type.node_type == NODE_TYPE_ID_GROUP_INPUT) {
//std::cout << "scene_node_editor_class::handle_deleted_links() 222AAA : " << from_node->node_id << ":" << from_node->ui_node_type.category << ":" << from_node->ui_node_type.node_type << std::endl;
                    delete_group_node_input_pin(from_pin);// Deletes link as well as pin in this function
                    delete_link_only = false;
                }
            }
        }

        if (to_pin.node_id != INVALID_ID) {
            node_basis_class *to_node = graph.nodes.get_node(to_pin.node_id);
            if (to_node) {
//std::cout << "scene_node_editor_class::handle_deleted_links() 333 : " << to_node->node_id << ":" << to_node->ui_node_type.category << ":" << to_node->ui_node_type.node_type << std::endl;
                if (to_node->ui_node_type.category == NODE_CATEGORY_ID_GROUP && to_node->ui_node_type.node_type == NODE_TYPE_ID_GROUP_OUTPUT) {
//std::cout << "scene_node_editor_class::handle_deleted_links() 333AAAA : " << to_node->node_id << ":" << to_node->ui_node_type.category << ":" << to_node->ui_node_type.node_type << std::endl;
                    delete_group_node_output_pin(to_pin);// Deletes link as well as pin in this function
                    delete_link_only = false;
                }
            }
        }
    }


    if (delete_link_only) graph.links.delete_link(link_id);
}


void scene_node_editor_class::display_node_ui(node_basis_class *node) {
    if (!node) return;
    node->define_ui();
}

// Handle new links
// These are driven by Imnodes, so we place the code after EndNodeEditor().
void scene_node_editor_class::handle_new_links() {
    int start_attr, end_attr;
    // This does not create a link, only returns the pin ids of the the two elected pin links
    if (IsLinkCreated(&start_attr, &end_attr))
    {
std::cout << "handle_new_links 0000 :: IsLinkCreated\n";


        pin_struct_type start_pin = graph.pins.find_pin(start_attr);
        pin_struct_type end_pin   = graph.pins.find_pin(end_attr);

std::cout << "handle_new_links 1111 "<< start_pin.pin_id << " : " <<end_pin.pin_id << std::endl;

        if (start_pin.pin_value_type.category == end_pin.pin_value_type.category && start_pin.data_type == end_pin.data_type)
        {
            // Make sure do not create a link if have pin already assigned to a link if have a single link only permitted
            // Future enhancement do auto delete existing link ?????
            if ((start_pin.pin_type == pin_type_enum::single_output && !graph.links.start_pin_has_link(start_pin.pin_id)) ||
                start_pin.pin_type == pin_type_enum::multi_output)
            {
                if ((end_pin.pin_type == pin_type_enum::single_input && !graph.links.end_pin_has_link(end_pin.pin_id)) ||
                    end_pin.pin_type == pin_type_enum::multi_input)
                {
                    graph.links.insert(start_pin.pin_id, end_pin.pin_id);
                }
            }
        }
    }
}


void scene_node_editor_class::handle_deleted_links() {
    // Delete link edges that define existing link input-output pairs
    int link_id;
    if (IsLinkDestroyed(&link_id))
    {
        graph.links.delete_link(link_id);
    }

    {

        // Delete link definitions
        //const int num_selected_links = ImNodes::NumSelectedLinks();
        const int num_selected_links = NumSelectedLinks();
        if (num_selected_links > 0 && ImGui::IsKeyReleased(ImGuiKey_X))
        {
            static std::vector<int> selected_links;
            selected_links.resize(static_cast<size_t>(num_selected_links));
            //ImNodes::GetSelectedLinks(selected_links.data());
            GetSelectedLinks(selected_links.data());

            for (int link_id : selected_links) {
                delete_link(link_id);
            }

            //ImNodes::ClearLinkSelection();// or will still have selected links 
            ClearLinkSelection();// or will still have selected links 
                                 //still defined that do not exist and will cause a crash
        }
    }

    {
        // Delete node definitions
        //const int num_selected_links = ImNodes::NumSelectedNodes();
        const int num_selected_links = NumSelectedNodes();
        if (num_selected_links > 0 && ImGui::IsKeyReleased(ImGuiKey_X))
        {
            static std::vector<int> selected_nodes;
            selected_nodes.resize(static_cast<size_t>(num_selected_links));
            //ImNodes::GetSelectedNodes(selected_nodes.data());
            GetSelectedNodes(selected_nodes.data());
            for (int node_id : selected_nodes) {
//printf("scene_node_editor_class::handle_deleted_links:: 0000 %i \n", node_id);                   
                graph.delete_node(node_id);
            }

            //ImNodes::ClearNodeSelection();// or will still have selected nodes 
            ClearNodeSelection();// or will still have selected nodes 
                                 // still defined that do not exist and will cause a crash 
        }
    }
}

// +++++

void scene_node_editor_class::save_group(node_basis_class *group_node){
    node_editor_import_export_class node_editor_export;

    if (!group_node) return;
    if (!(group_node->ui_node_type.category == NODE_CATEGORY_ID_GROUP) || !(group_node->ui_node_type.node_type == NODE_TYPE_ID_GROUP)) {
        // error message
        return;
    }

    scene_node_editor_class *node_group_editor = get_node_group_editor(group_node->node_id);
    if(!node_group_editor) {
        // error message
        return;
    }

    node_group_class *node_group = dynamic_cast<node_group_class*>(group_node);

    char const* patterns[] = { "*.ngrp" };
    char const* file_pathname = vwDialogs::save_file(nullptr, patterns, 1);

//std::cout << "scene_node_editor_class:save_group 0000 : " << file_pathname << std::endl;

    node_editor_export.export_node_group(file_pathname, node_group, node_group_editor);

}

void scene_node_editor_class::import_group(scene_node_editor_class *node_group_editor, ImVec2 click_pos) {
    node_editor_import_export_class node_editor_import;

    if (!node_group_editor) return;

    //scene_node_editor_class* node_group_editor = get_node_group_editor(group_node->node_id);
    if (!node_group_editor) {
        // error message
        return;
    }

    char const* patterns[] = { "*.ngrp" };
    char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

std::cout << "scene_node_editor_class:import_group 0000 : " << file_pathname << std::endl;
if(!node_group_editor->log_panel) std::cout << "scene_node_editor_class:import_group 0000 !node_group_editor->log_panel: %%%%%%% " << std::endl;
if(!node_group_editor->vw_scene) std::cout << "scene_node_editor_class:import_group 0000 !node_group_editor->vw_scene : %%%%%%%%% "  << std::endl;

    node_editor_import.import_node_group(file_pathname, node_group_editor,click_pos);
}
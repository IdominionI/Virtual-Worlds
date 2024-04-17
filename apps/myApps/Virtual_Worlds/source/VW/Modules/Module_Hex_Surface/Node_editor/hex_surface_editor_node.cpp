#include "hex_surface_editor_node.h"

#include <Universal_FW/ImNodes/Kernal/node_editor.h>>

#include <VW/Editor/Node_Editor/node_editor_extras.h>

#include "../Functions/hex_surface_exports.h"
#include "../Editor/Widgets/hex_parameters_widget.h"

bool hex_surface_editor_node_class::define_node(ImVec2 click_pos, node_id_type entity_id_) {
    node_dimensions = { 60.0f,5.0f };

    // !!! CRITICAL : MUST HAVE NODE UI NODE TYPE DEFINITION DEFINED
    // !!! ELSE CANNOT LINK NODE PINS, PERFORM EVALUATIONS OR DISPLAY
    // !!! NODE PARAMETERS TO BE DISPLAYED AND MODIFIED AS WELL AS
    // !!! OTHER NODE AND NODE EDITOR FUCTIONALITY 

    ui_node_type.category          = NODE_CATEGORY_ID_GENERATOR;
    ui_node_type.node_type         = NODE_TYPE_ID_HEX_SURFACE_EDITOR;
    ui_node_type.node_data_context = NODE_DATA_CONTEXT;

//std::cout << "hex_surface_editor_node_class :: define_node 111  : " << std::endl;

    if (!create_hex_surface()) {
        return false;
    }
//std::cout << "hex_surface_editor_node_class :: define_node 222  : " << std::endl;

    // !!! CRITICAL : MUST HAVE node_entity_id : node_entity_category_id : node_entity_type_id
    // !!! DEFINED ELSE WILL NOT HAVE PARAMETER UI DISPLAYED

    node_entity_id          = hex_surface->id;
    node_entity_category_id = hex_surface->object_category_id;
    node_entity_type_id     = hex_surface->object_type_id;

    label = "HEX EDITOR " + std::to_string(node_id) + ":"+std::to_string(hex_surface->id);
    hex_surface->name = "HEX_EDITOR_" + std::to_string(hex_surface->id);

    node_draw_data.node_header_color          = ImColor(50, 140, 90, 255);
    node_draw_data.node_header_hovered_color  = ImColor(60, 205, 130, 255);
    node_draw_data.node_header_selected_color = ImColor(80, 240, 160, 255);
    node_draw_data.node_header_label_color    = ImColor(255, 255, 255, 255);// ImGui does not have a color function that accepts its own ImColor type for textS

    node_draw_data.node_body_color            = NODE_HEX_DRAW_BODY_COLOR            ;
    node_draw_data.node_body_hovered_color    = NODE_HEX_DRAW_BODY_HOVERED_COLOR    ;
    node_draw_data.node_body_selected_color   = NODE_HEX_DRAW_BODY_SELECTED_COLOR   ;
    node_draw_data.node_outline_color         = NODE_HEX_DRAW_OUTLINE_COLOR         ;

    graph->node_editor->SetNodeScreenSpacePos(node_id, click_pos);
    return true;
}

bool hex_surface_editor_node_class::define_import_node(ImVec2 click_pos,node_id_type entity_id_, void* node_misc_data) {
    node_misc_import_struct_type* node_misc_import_data = static_cast<node_misc_import_struct_type*>(node_misc_data);

    if (!node_misc_import_data) {
        std::cout << "ERROR : Editor scene and log data not defined. Aborting import of HCP Voxel node.\n";
        return false;
    }

    if (node_misc_import_data->log_panel == nullptr) {
        std::cout << "WARNING : Imported HCP Voxel  Editor node has no defined log panel to output messages to.\n";
    }

    if (node_misc_import_data->vw_scene == nullptr) {
        std::cout << "ERROR : Imported HCP Voxel Editor node has no defined scene manager. Aborted import.\n" << std::endl;
        if (log_panel) log_panel->application_log.AddLog("ERROR :Imported HCP Voxel Editor node has no defined scene manager. Aborted import.\n");
        return false;
    }

    log_panel = node_misc_import_data->log_panel;
    vw_scene  = node_misc_import_data->vw_scene;

    return define_node(click_pos,entity_id_);
}

bool  hex_surface_editor_node_class::create_hex_surface() {
std::cout << "hex_surface_editor_node_class:create_hex_surface 0000 \n";
if(!vw_scene) { std::cout << "hex_surface_editor_node_class:create_hex_surface 000AAAA :: vw_scene == nullptr \n"; return false; }
if (!log_panel) { std::cout << "hex_surface_editor_node_class:create_hex_surface 000AAAA :: log_panel == nullptr \n"; return false; }

    hex_surface = new hex_surface_object_class;

    if (!hex_surface) return false;

    hex_surface->log_panel = log_panel;

    int object_category_id = vw_scene->scene_entities_manager.get_objects_category_index(SCENE_CATEGORY_HEX_SURFACE);
    if (object_category_id == -1){
std::cout << "hex_surface_editor_node_class:create_hex_surface :  object_category_id == -1 \n"; 
        hex_surface->object_category_id = vw_scene->scene_entities_manager.define_new_entity_category(SCENE_CATEGORY_HEX_SURFACE);
    } else
        hex_surface->object_category_id = object_category_id;

    hex_surface->gizmo_display = node_gizmo_display_enum::none;
    hex_surface->axis_size     = 100.0;

    hex_surface_parameters_widget_class *pw = new hex_surface_parameters_widget_class;
    hex_surface->parameter_widget = pw;

    hex_surface->parameter_widget->log_panel = log_panel;

    if (!vw_scene->scene_entities_manager.add_object(hex_surface, hex_surface->object_category_id))// This is where hex_surface->id is defined
        return false;

    hex_surface->object_type_id = ENTITY_TYPE_OBJECT;

//std::cout << "hex_surface_editor_node_class:create_hex_surface :  vw_scene.scene_entities_manager.add_object : " << hex_surface->id << " : " << hex_surface->object_category_id << std::endl;

        hex_surface->define_initial_shader_program(); //**** Needs to be changed to define initial node editor shader program

        //hex_surface->geometry->create_shader();
        //hex_surface->geometry->init();
        if (hex_surface->geometry->shader) {
            if (ofIsGLProgrammableRenderer()) {
                hex_surface->update_shader();
                shader_class shader;
				shader.set_b1(hex_surface->geometry->shader->getProgram(),true, "display_as_points");
//std::cout << "hex_surface_editor_node_class:create_hex_surface: ofIsGLProgrammableRenderer() : " << std::endl;
                //shaders_loaded = hex_surface->geometry->shader->load("Shaders/passthru.vert", "Shaders/grid_fragment_shader.glsl");
                //shaders_loaded = hex_surface->geometry->shader->load("Assets/Shaders/Default/Voxel/default_gen_TRD_VS.glsl", "Assets/Shaders/Default/Voxel/default_gen_TRD_FS.glsl");
                //shaders_loaded = hex_surface->geometry->shader->load("Assets/Shaders/Default/Voxel/default_gen_TRD_VS.glsl", "Assets/Shaders/Default/Voxel/default_gen_TRD_FS.glsl", "Assets/Shaders/Default/Voxel/default_gen_TRD_PGS.glsl");
                //shaders_loaded = hex_surface->geometry->shader->load("Assets/Shaders/Default/Voxel/default_gen_TRD_VS.glsl", "Assets/Shaders/Default/Voxel/default_gen_TRD_FS.glsl", "Assets/Shaders/Default/Voxel/default_gen_TRD_GS.glsl");
            }
            else {
//std::cout << "hex_surface_editor_node_class:create_hex_surface:!!!!ofIsGLProgrammableRenderer() : " << std::endl;
                int i = vw_scene->scene_entities_manager.get_objects_category_index("hex_surface");// SCENE_CATEGORY_hex_surface does not work ?????
                if (i > -1) {
                    vw_scene->scene_entities_manager.delete_object(hex_surface->id, vw_scene->scene_entities_manager.scene_objects[i].objects_category_id);
                }
                return false;
            }

            if (!hex_surface->geometry->shader->shader_compile_successful) {
//std::cout << "hex_surface_editor_node_class:create_hex_surface: hex_surface Shaders not loaded !!!!! : " << std::endl;
//std::string s = "jjjj\n";
//cout << s << std::endl;
//std::cout << hex_surface->geometry->shader->compile_log << std::endl;
//std::cout << "hex_surface_editor_node_class:create_hex_surface: hex_surface Shaders not loaded !!!!! END : " << std::endl;
            }
            else {
//cout << " Shaders loaded ^^^^^ : " << shader.getProgram() << " : " << std::endl;
                std::cout << "hex_surface_editor_node_class:create_hex_surface: hex_surface Shaders loaded ^^^^^ : " << std::endl;
                if (log_panel) log_panel->application_log.AddLog("hex_surface_editor_node_class:create_hex_surface: hex_surface Shaders loaded.\n");
//cout << entity_object03->geometry->shader->compile_log << std::endl;
            }
        }
        else
//std::cout << "hex_surface_editor_node_class:create_hex_surface: hex_surface Shader not created : " << std::endl;

    return true;
}

void  hex_surface_editor_node_class::delete_node_entity() {
//std::cout << "hex_surface_editor_node_class:create_hex_surface: delete_node_entity : 0000" << std::endl;
    if (!hex_surface || !vw_scene) return;

//std::cout<< "hex_surface_editor_node_class:delete_node_entity !!!! " << node_entity_id << ":" << node_entity_category_id << std::endl;
    vw_scene->scene_entities_manager.delete_object(node_entity_id, node_entity_category_id);
//std::cout << "hex_surface_editor_node_class:create_hex_surface: delete_node_entity : 1111" << std::endl;
   // delete hex_surface; // Not to be used as this is called by the process of vw_scene->scene_entities_manager.delete_object(node_entity_id, node_entity_category_id);
}

void hex_surface_editor_node_class::editor_menu_options(){
/*
    std::string menu_text = "Voxel ... ###nvoxel" + std::to_string(node_id);
    if (ImGui::BeginMenu(menu_text.c_str())) {
        menu_text = "Define Surface points###nvfs" + std::to_string(node_id);
        if (ImGui::MenuItem(menu_text.c_str())) {
            voxel_main_window_menu_functions.voxel_volume_to_voxel_surface(SELECTED_EXPORT, hex_surface->id, &vw_scene->scene_entities_manager, log_panel);
        }

        ImGui::EndMenu();
    }

    menu_text = "Export ...###nexport" + std::to_string(node_id);
    if (ImGui::BeginMenu(menu_text.c_str())) {
        if (hex_surface && vw_scene) { // Must have a defined voxel and a scene with voxel data in it.
            menu_text = "As Point Cloud###nvepc" + std::to_string(node_id);
            if (ImGui::MenuItem(menu_text.c_str())) {
//std::cout << "hex_surface_editor_node_class::editor_menu_options ^^^^^^^^ : " << vw_scene->scene_entities_manager.get_objects_of_category(SCENE_CATEGORY_hex_surface).category_objects.size() << ":" << globalc::get_current_selected_entity_id() << ":" << hex_surface->id << std::endl;
                voxel_main_window_menu_functions.export_voxels_center_point_data(SELECTED_EXPORT, hex_surface->id, &vw_scene->scene_entities_manager, log_panel);
            }

            menu_text = "As Point Surface###nveps" + std::to_string(node_id);
            if (ImGui::MenuItem(menu_text.c_str())) {
                voxel_main_window_menu_functions.export_voxels_point_surface_data(SELECTED_EXPORT, hex_surface->id, &vw_scene->scene_entities_manager, log_panel);
            }

            menu_text = "As Face Surface###nvefs" + std::to_string(node_id);
            if (ImGui::MenuItem(menu_text.c_str())) {
                voxel_main_window_menu_functions.export_voxels_surface_face_data(SELECTED_EXPORT, hex_surface->id, &vw_scene->scene_entities_manager, log_panel);
            }
        }
        ImGui::EndMenu();
    }
*/
}

bool hex_surface_editor_node_class::define_ui() {
    if (!hex_surface) {
        std::cout << "hex_surface_editor_node_class : display_ui HCP Voxel 0000 : hex_surface == NULL \n";
        return false;
    }
//std::cout << "hex_surface_editor_node_class : display_ui HCP VOxel 0000 " << node_id << " : " << hex_surface->id << " : " << hex_surface->object_category_id << " : " << hex_surface->object_type_id << std::endl;
    globalc::set_current_selected_data_context_id(this->ui_node_type.node_data_context);
    globalc::set_current_selected_entity_id(INVALID_ID);
    globalc::set_current_selected_entity_category_type_id(INVALID_ID);
    globalc::set_current_selected_entity_type_id(INVALID_ID);

    globalc::set_current_selected_node(this);

    return true; 
}

void hex_surface_editor_node_class::display_ui(node_id_type current_selected_object_id) {
//std::cout << "hex_surface_editor_node_class::display_ui\n";

    // Best to perform this code block only when connecting or diconnecting node input link
    if (!vw_scene ) {
        // error message
        std::cout << "hex_surface_editor_node_class::display_ui   !vw_scene\n";
        return;
    }

    hex_surface_object_class *hex_surface_object_to_execute = hex_surface;
    display_hex_surface_editor_widget(hex_surface_object_to_execute);
}

void hex_surface_editor_node_class::display_hex_surface_editor_widget(hex_surface_object_class *hex_surface_object_to_execute) {
    if (!hex_surface_object_to_execute) {
        std::cout << "hex_surface_editor_node_class::display_ui   !have no voxel object to perform automata on.\n";
        if (log_panel != NULL) log_panel->application_log.AddLog("Voxel editor : Have no voxel object to perform automata on.\n");
        return;
    }

    hex_generation_editor_widget.current_selected_object_id = hex_surface_object_to_execute->id;
    hex_generation_editor_widget.object_to_execute          = hex_surface_object_to_execute;
    hex_generation_editor_widget.log_panel                  = log_panel;
    hex_generation_editor_widget.display_parameters_widget();
}
#include "hex_surface_node.h"

#include <Universal_FW/ImNodes/Kernal/node_editor.h>>

#include <VW/Editor/Node_Editor/node_editor_extras.h>

#include "../Functions/hex_surface_exports.h"
#include "../Editor/Widgets/hex_parameters_widget.h"

#include <Universal_FW/Timeline/timeline_tracks.h>

bool hex_surface_node_class::define_node(ImVec2   click_pos, id_type  entity_id_) {
    node_dimensions = { 60.0f,5.0f };

    // !!! CRITICAL : MUST HAVE NODE UI NODE TYPE DEFINITION DEFINED
    // !!! ELSE CANNOT LINK NODE PINS, PERFORM EVALUATIONS OR DISPLAY
    // !!! NODE PARAMETERS TO BE DISPLAYED AND MODIFIED AS WELL AS
    // !!! OTHER NODE AND NODE EDITOR FUCTIONALITY 

    ui_node_type.category          = NODE_CATEGORY_ID_GENERATOR;
    ui_node_type.node_type         = NODE_TYPE_ID_HEX_SURFACE;
    ui_node_type.node_data_context = ENTITY_DATA_CONTEXT;

//std::cout << "hcp_voxel_node_class :: define_node 111  : " << std::endl;

    if (!create_hex_surface()) {
        return false;
    }
//std::cout << "hcp_voxel_node_class :: define_node 222  : " << std::endl;

    // !!! CRITICAL : MUST HAVE node_entity_id : node_entity_category_id : node_entity_type_id
    // !!! DEFINED ELSE WILL NOT HAVE PARAMETER UI DISPLAYED
    
    node_entity_id          = hex_surface->id;
    node_entity_category_id = hex_surface->object_category_id;
    node_entity_type_id     = hex_surface->object_type_id;

    label = "Hex Surface " + std::to_string(hex_surface->id);
    hex_surface->name = "HEX_SURFACE_" + std::to_string(hex_surface->id);

    node_header_color          = ImColor(0, 139, 139, 255);
    node_header_hovered_color  = ImColor(0, 175, 175, 255);
    node_header_selected_color = ImColor(0, 255, 255, 255);
    node_header_label_color    = ImColor(255, 255, 255, 255);// ImGui does not have a color function that accepts its own ImColor type for textS

    node_body_color          = ImColor(75, 75, 75, 255);
    node_body_hovered_color  = ImColor(100, 100, 100, 255);
    node_body_selected_color = ImColor(150, 150, 150, 255);
    node_outline_color       = ImColor(50, 50, 50, 255);

    graph->node_editor->SetNodeScreenSpacePos(node_id, click_pos);
    return true;
}

bool hex_surface_node_class::define_import_node(ImVec2 click_pos, node_id_type entity_id_, void* node_misc_data) {
    node_misc_import_struct_type* node_misc_import_data = static_cast<node_misc_import_struct_type*>(node_misc_data);

    if (!node_misc_import_data) {
        std::cout << "ERROR : Editor scene and log data not defined. Aborting import of Hex surface node.\n";
        return false;
    }

    if (node_misc_import_data->log_panel == nullptr) {
        std::cout << "WARNING : Imported Hex surface node has no defined log panel to output messages to.\n";
    }

    if (node_misc_import_data->vw_scene == nullptr) {
        std::cout << "ERROR : Imported Hex surface node has no defined scene manager. Aborted import.\n" << std::endl;
        if (log_panel) log_panel->application_log.AddLog("ERROR :Imported Hex surface node has no defined scene manager. Aborted import.\n");
        return false;
    }

    log_panel = node_misc_import_data->log_panel;
    vw_scene  = node_misc_import_data->vw_scene;

    return define_node(click_pos, entity_id_);
}

bool  hex_surface_node_class::create_hex_surface() {
//std::cout << "hex_surface_node_class:create_hex_surface 0000 \n";
if(!vw_scene) { std::cout << "hex_surface_node_class:create_hex_surface 000AAAA :: vw_scene == nullptr \n"; return false; }
if (!log_panel) { std::cout << "hex_surface_node_class:create_hex_surface 000AAAA :: log_panel == nullptr \n"; return false; }

    hex_surface = new hex_surface_object_class;

    if (!hex_surface) return false;

    hex_surface->log_panel = log_panel;

    int object_category_id = vw_scene->scene_entities_manager.get_objects_category_index(SCENE_CATEGORY_HEX_SURFACE);
    if (object_category_id == -1) {
//std::cout << "hex_surface_node_class:create_hex_surface :  object_category_id == -1  111111111111 \n";
        hex_surface->object_category_id = vw_scene->scene_entities_manager.define_new_entity_category(SCENE_CATEGORY_HEX_SURFACE);
    }
    else
        hex_surface->object_category_id = object_category_id;

    hex_surface->gizmo_display = node_gizmo_display_enum::none;
    hex_surface->axis_size = 100.0;

    hex_surface_parameters_widget_class* pw = new hex_surface_parameters_widget_class;
    hex_surface->parameter_widget = pw;

    hex_surface->parameter_widget->log_panel = log_panel;

    if (!vw_scene->scene_entities_manager.add_object(hex_surface, hex_surface->object_category_id))// This is where hex_surface->id is defined
        return false;

    hex_surface->object_type_id = ENTITY_TYPE_OBJECT;

//std::cout << "hex_surface_node_class:create_hex_surface :  vw_scene.scene_entities_manager.add_object  22222: " << hex_surface->id << " : " << hex_surface->object_category_id << std::endl;

    hex_surface->define_initial_shader_program();

    if (hex_surface->geometry->shader) {
        if (ofIsGLProgrammableRenderer()) {
//std::cout << "hex_surface_node_class:create_hex_surface: ofIsGLProgrammableRenderer() : 33333 " << std::endl;
            //shaders_loaded = hcp_voxel->geometry->shader->load("Shaders/passthru.vert", "Shaders/grid_fragment_shader.glsl");
            //shaders_loaded = hcp_voxel->geometry->shader->load("Assets/Shaders/Default/Voxel/default_gen_TRD_VS.glsl", "Assets/Shaders/Default/Voxel/default_gen_TRD_FS.glsl");
            //shaders_loaded = hcp_voxel->geometry->shader->load("Assets/Shaders/Default/Voxel/default_gen_TRD_VS.glsl", "Assets/Shaders/Default/Voxel/default_gen_TRD_FS.glsl", "Assets/Shaders/Default/Voxel/default_gen_TRD_PGS.glsl");
            //shaders_loaded = hcp_voxel->geometry->shader->load("Assets/Shaders/Default/Voxel/default_gen_TRD_VS.glsl", "Assets/Shaders/Default/Voxel/default_gen_TRD_FS.glsl", "Assets/Shaders/Default/Voxel/default_gen_TRD_GS.glsl");
        }
        else {
//std::cout << "hex_surface_node_class:create_hex_surface:!!!!ofIsGLProgrammableRenderer() : 44444" << std::endl;
            int i = vw_scene->scene_entities_manager.get_objects_category_index("Hex_Surface");// SCENE_CATEGORY_HEX_SURFACE does not work???
            if (i > -1) {
                vw_scene->scene_entities_manager.delete_object(hex_surface->id, vw_scene->scene_entities_manager.scene_objects[i].objects_category_id);
            }
            return false;
        }

        if (!hex_surface->geometry->shader->shader_compile_successful) {
//std::cout << "hex_surface_node_class:create_hex_surface: hex_surface Shaders not loaded !!!!! : 55555 " << std::endl;
//std::cout << hex_surface->geometry->shader->compile_log << std::endl;
//std::cout << "hex_surface_node_class:create_hex_surface: hex_surface Shaders not loaded !!!!! END : 66666" << std::endl;
        }
        else {
//cout << " Shaders loaded ^^^^^ : " << shader.getProgram() << " : " << std::endl;
            std::cout << "hex_surface_node_class:create_hex_surface: hex_surface Shaders loaded ^^^^^ : 77777" << std::endl;
//cout << entity_object03->geometry->shader->compile_log << std::endl;
        }
    }
    else
        std::cout << "hex_surface_node_class:create_hex_surface: hex surface Shader not created : 8888" << std::endl;

    return true;
}

bool hex_surface_node_class::create_hex_surface_timeline_link() {
    hex_surface_animation_object = new hex_surface_animation_object_class;

    if (!hex_surface || !hex_surface_animation_object) return false;

    hex_surface_animation_object->hex_surface_object = hex_surface;
    hex_surface_animation_object->log_panel = log_panel;


    // ================ define as a create timeline group function ===============
    if (!animation_timeline_tracks_widget->timeline_track_group_exists(node_id)) {
        //std::cout << "hcp_voxel_node_class::create_hcp_animation_link 000 : !animation_timeline_tracks_widget->timeline_track_group_exists(node_id)\n";
        int index = animation_timeline_tracks_widget->add_timeline_group(label, node_id, hex_surface_animation_object, TIMELINE_OBJECT_DATA_TYPE_ID_HEX_SURFACE);
        if (index != INVALID_ID && index < animation_timeline_tracks_widget->timeline_track_groups.size()) {

            timeline_interval_track_id = animation_timeline_tracks_widget->timeline_track_groups[index].add_interval_track("hex surface");
            if (timeline_interval_track_id == UINT_MAX) {
                animation_timeline_tracks_widget->delete_timeline_group(node_id);
                return false;
            }
            hex_surface_animation_object->hex_surface_interval_track_id = timeline_interval_track_id;

            timeline_interval_track_id = animation_timeline_tracks_widget->timeline_track_groups[index].add_interval_track("shader");
            if (timeline_interval_track_id == UINT_MAX) {
                animation_timeline_tracks_widget->delete_timeline_group(node_id);
                return false;
            }
            hex_surface_animation_object->hex_surface_shader_interval_track_id = timeline_interval_track_id;
        }
    }
    // =====================================================

    return true;
}


void hex_surface_node_class::delete_node_entity() {
    if (!hex_surface || !vw_scene) return;
//std::cout << "hcp_voxel_node_class:create_hcp_voxel: delete_node_entity : 0000" << std::endl;
    delete_hex_surface_timeline_link();
    vw_scene->scene_entities_manager.delete_object(node_entity_id, node_entity_category_id);
//std::cout << "hcp_voxel_node_class:create_hcp_voxel: delete_node_entity : 1111" << std::endl;
   // delete hex_surface; // Not to be used as this is called by the process of vw_scene->scene_entities_manager.delete_object(node_entity_id, node_entity_category_id);
}

void hex_surface_node_class::delete_hex_surface_timeline_link() {
    animation_timeline_tracks_widget->delete_timeline_group(node_id);
    delete hex_surface_animation_object;
    hex_surface_animation_object = nullptr;
    timeline_interval_track_id   = UINT_MAX;

    // If have more than one group 
    // : delete group animation object and initialise variables to unassigned values here
}

void hex_surface_node_class::editor_menu_options() {
    
    std::string menu_text;
    menu_text = "Timeline ... ###nvtimeline" + std::to_string(node_id);
    if (ImGui::BeginMenu(menu_text.c_str())) {

        menu_text = "Add timeline tracks###nvfs" + std::to_string(node_id);
        if (ImGui::MenuItem(menu_text.c_str())) {
            if (hex_surface_animation_object == nullptr) // Mist have no existing animation timeline object linked to this node
                create_hex_surface_timeline_link();
        }

        menu_text = "Delete ... ###nvtracks" + std::to_string(node_id);
        if (ImGui::BeginMenu(menu_text.c_str())) {
            menu_text = "Delete timeline group###nvfs" + std::to_string(node_id);
            if (ImGui::MenuItem(menu_text.c_str())) {
                delete_hex_surface_timeline_link();
            }

            // When more than one track in a group uncomment the following
            menu_text = "Delete generation track###nvfs" + std::to_string(node_id);
            if (ImGui::MenuItem(menu_text.c_str())) {
                if (hex_surface_animation_object) {// Must have a timeline animation object linked to this node
                    animation_timeline_tracks_widget->delete_timeline_group_track(node_id, hex_surface_animation_object->hex_surface_interval_track_id);
                    hex_surface_animation_object->hex_surface_interval_track_id = -1;
                }
            }

            menu_text = "Delete shader track###nvfs" + std::to_string(node_id);
            if (ImGui::MenuItem(menu_text.c_str())) {
                if (hex_surface_animation_object) {// Must have a timeline animation object linked to this node
                    animation_timeline_tracks_widget->delete_timeline_group_track(node_id, hex_surface_animation_object->hex_surface_shader_interval_track_id);
                    hex_surface_animation_object->hex_surface_shader_interval_track_id = -1;
                }
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }

    menu_text = "Export ...###nexport" + std::to_string(node_id);
    if (ImGui::BeginMenu(menu_text.c_str())) {
        if (hex_surface && vw_scene) { // Must have a defined voxel and a scene with voxel data in it.
            menu_text = "As Point Cloud###nvepc" + std::to_string(node_id);
            if (ImGui::MenuItem(menu_text.c_str())) {
                hex_suface_main_window_menu_functions.export_hex_surface_center_point_data(SELECTED_EXPORT, hex_surface->id, &vw_scene->scene_entities_manager, log_panel);
            }

            menu_text = "As Face Surface###nvefs" + std::to_string(node_id);
            if (ImGui::MenuItem(menu_text.c_str())) {
                hex_suface_main_window_menu_functions.export_hex_surface_surface_face_data(SELECTED_EXPORT, hex_surface->id, &vw_scene->scene_entities_manager, log_panel);
            }
        }
        ImGui::EndMenu();
    }  
}

bool hex_surface_node_class::define_ui() {
    if (!hex_surface) {
        std::cout << "hex_surface_node_class : display_ui Hex Surface 0000 : hcp_voxel == NULL \n";
        return false;
    }
std::cout << "hex_surface_node_class::define_ui() hex_surface 0000 " << node_id << " : " << hex_surface->id << " : " << hex_surface->object_category_id << " : " << hex_surface->object_type_id << std::endl;
    globalc::set_current_selected_data_context_id(ENTITY_DATA_CONTEXT);
    globalc::set_current_selected_entity_id(hex_surface->id);
    globalc::set_current_selected_entity_category_type_id(hex_surface->object_category_id);
    globalc::set_current_selected_entity_type_id(hex_surface->object_type_id);

    return true;
}
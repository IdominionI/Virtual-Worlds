#include "hcp_voxel_node.h"

#include <Universal_FW/ImNodes/Kernal/node_editor.h>>

#include <VW/Editor/Node_Editor/node_editor_extras.h>

#include "../Functions/vw_voxel_exports.h"
#include "../Editor/Widgets/hcp_voxel_parameters_widget.h"

#include <Universal_FW/Timeline/timeline_tracks.h>

bool hcp_voxel_node_class::define_node(ImVec2 click_pos, node_id_type entity_id_) {
    node_dimensions = { 60.0f,5.0f };

    // !!! CRITICAL : MUST HAVE NODE UI NODE TYPE DEFINITION DEFINED
    // !!! ELSE CANNOT LINK NODE PINS, PERFORM EVALUATIONS OR DISPLAY
    // !!! NODE PARAMETERS TO BE DISPLAYED AND MODIFIED AS WELL AS
    // !!! OTHER NODE AND NODE EDITOR FUCTIONALITY 

    ui_node_type.category          = NODE_CATEGORY_ID_GENERATOR;
    ui_node_type.node_type         = NODE_TYPE_ID_HCP_VOXEL;
    ui_node_type.node_data_context = ENTITY_DATA_CONTEXT;

//std::cout << "hcp_voxel_node_class :: define_node 111  : " << std::endl;

    if (!create_hcp_voxel()) {
        return false;
    }
//std::cout << "hcp_voxel_node_class :: define_node 222  : " << std::endl;

    // !!! CRITICAL : MUST HAVE node_entity_id : node_entity_category_id : node_entity_type_id
    // !!! DEFINED ELSE WILL NOT HAVE PARAMETER UI DISPLAYED

    node_entity_id          = hcp_voxel->id;
    node_entity_category_id = hcp_voxel->object_category_id;
    node_entity_type_id     = hcp_voxel->object_type_id;

    label = "HCP VOXEL " + std::to_string(hcp_voxel->id);
    hcp_voxel->name = "HCP_VOXEL_" + std::to_string(hcp_voxel->id);

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

bool hcp_voxel_node_class::define_import_node(ImVec2 click_pos,node_id_type entity_id_, void* node_misc_data) {
    node_misc_import_struct_type* node_misc_import_data = static_cast<node_misc_import_struct_type*>(node_misc_data);

    if (!node_misc_import_data) {
        std::cout << "ERROR : Editor scene and log data not defined. Aborting import of HCP Voxel node.\n";
        return false;
    }

    if (node_misc_import_data->log_panel == nullptr) {
        std::cout << "WARNING : Imported HCP Voxel node has no defined log panel to output messages to.\n";
    }

    if (node_misc_import_data->vw_scene == nullptr) {
        std::cout << "ERROR : Imported HCP Voxel node has no defined scene manager. Aborted import.\n" << std::endl;
        if (log_panel) log_panel->application_log.AddLog("ERROR :Imported HCP Voxel node has no defined scene manager. Aborted import.\n");
        return false;
    }

    log_panel = node_misc_import_data->log_panel;
    vw_scene  = node_misc_import_data->vw_scene;

    return define_node(click_pos,entity_id_);
}

bool  hcp_voxel_node_class::create_hcp_voxel() {
std::cout << "hcp_voxel_node_class:create_hcp_voxel 0000 \n";
if(!vw_scene) { std::cout << "hcp_voxel_node_class:create_hcp_voxel 000AAAA :: vw_scene == nullptr \n"; return false; }
if (!log_panel) { std::cout << "hcp_voxel_node_class:create_hcp_voxel 000AAAA :: log_panel == nullptr \n"; return false; }

    hcp_voxel = new voxel_hcp_object_class;

    if (!hcp_voxel) return false;

    hcp_voxel->log_panel = log_panel;

    int object_category_id = vw_scene->scene_entities_manager.get_objects_category_index(SCENE_CATEGORY_HCP_VOXEL);
    if (object_category_id == -1){
std::cout << "hcp_voxel_node_class:create_hcp_voxel :  object_category_id == -1 \n"; 
        hcp_voxel->object_category_id = vw_scene->scene_entities_manager.define_new_entity_category(SCENE_CATEGORY_HCP_VOXEL);
    } else
        hcp_voxel->object_category_id = object_category_id;

    hcp_voxel->gizmo_display = node_gizmo_display_enum::none;
    hcp_voxel->axis_size     = 100.0;

    voxel_hcp_parameters_widget_class *pw = new voxel_hcp_parameters_widget_class;
    hcp_voxel->parameter_widget = pw;

    hcp_voxel->parameter_widget->log_panel = log_panel;

    if (!vw_scene->scene_entities_manager.add_object(hcp_voxel, hcp_voxel->object_category_id))// This is where hcp_voxel->id is defined
        return false;

    hcp_voxel->object_type_id = ENTITY_TYPE_OBJECT;

std::cout << "hcp_voxel_node_class:create_hcp_voxel :  vw_scene.scene_entities_manager.add_object : " << hcp_voxel->id << " : " << hcp_voxel->object_category_id << std::endl;

        hcp_voxel->define_initial_shader_program();

        //hcp_voxel->geometry->create_shader();
        //hcp_voxel->geometry->init();
        if (hcp_voxel->geometry->shader) {
            if (ofIsGLProgrammableRenderer()) {
std::cout << "hcp_voxel_node_class:create_hcp_voxel: ofIsGLProgrammableRenderer() : " << std::endl;
                //shaders_loaded = hcp_voxel->geometry->shader->load("Shaders/passthru.vert", "Shaders/grid_fragment_shader.glsl");
                //shaders_loaded = hcp_voxel->geometry->shader->load("Assets/Shaders/Default/Voxel/default_gen_TRD_VS.glsl", "Assets/Shaders/Default/Voxel/default_gen_TRD_FS.glsl");
                //shaders_loaded = hcp_voxel->geometry->shader->load("Assets/Shaders/Default/Voxel/default_gen_TRD_VS.glsl", "Assets/Shaders/Default/Voxel/default_gen_TRD_FS.glsl", "Assets/Shaders/Default/Voxel/default_gen_TRD_PGS.glsl");
                //shaders_loaded = hcp_voxel->geometry->shader->load("Assets/Shaders/Default/Voxel/default_gen_TRD_VS.glsl", "Assets/Shaders/Default/Voxel/default_gen_TRD_FS.glsl", "Assets/Shaders/Default/Voxel/default_gen_TRD_GS.glsl");
            }
            else {
std::cout << "hcp_voxel_node_class:create_hcp_voxel:!!!!ofIsGLProgrammableRenderer() : " << std::endl;
                int i = vw_scene->scene_entities_manager.get_objects_category_index("HCP_Voxel");// SCENE_CATEGORY_HCP_VOXEL does not work ?????
                if (i > -1) {
                    vw_scene->scene_entities_manager.delete_object(hcp_voxel->id, vw_scene->scene_entities_manager.scene_objects[i].objects_category_id);
                }
                return false;
            }

            if (!hcp_voxel->geometry->shader->shader_compile_successful) {
std::cout << "hcp_voxel_node_class:create_hcp_voxel: hcp_voxel Shaders not loaded !!!!! : " << std::endl;
//std::string s = "jjjj\n";
//cout << s << std::endl;
std::cout << hcp_voxel->geometry->shader->compile_log << std::endl;
std::cout << "hcp_voxel_node_class:create_hcp_voxel: hcp_voxel Shaders not loaded !!!!! END : " << std::endl;
            }
            else {
//cout << " Shaders loaded ^^^^^ : " << shader.getProgram() << " : " << std::endl;
                std::cout << "hcp_voxel_node_class:create_hcp_voxel: hcp_voxel Shaders loaded ^^^^^ : " << std::endl;
//cout << entity_object03->geometry->shader->compile_log << std::endl;
            }
        }
        else
std::cout << "hcp_voxel_node_class:create_hcp_voxel: hcp_voxel Shader not created : " << std::endl;

    return true;
}

bool hcp_voxel_node_class::create_hcp_timeline_link() {
    hcp_animation_object = new hcp_animation_object_class;

    if (!hcp_voxel || !hcp_animation_object) return false;

    hcp_animation_object->voxel_hcp_object = hcp_voxel;
    hcp_animation_object->log_panel        = log_panel;


    // ================ define as a create timeline group function ===============
    if (!animation_timeline_tracks_widget->timeline_track_group_exists(node_id)) {
//std::cout << "hcp_voxel_node_class::create_hcp_animation_link 000 : !animation_timeline_tracks_widget->timeline_track_group_exists(node_id)\n";
        int index = animation_timeline_tracks_widget->add_timeline_group(label, node_id, hcp_animation_object, TIMELINE_OBJECT_DATA_TYPE_ID_VOXEL);
        if (index != INVALID_ID && index < animation_timeline_tracks_widget->timeline_track_groups.size()) {
            
            timeline_interval_track_id = animation_timeline_tracks_widget->timeline_track_groups[index].add_interval_track("hcp voxel");
            if (timeline_interval_track_id == UINT_MAX) {
                animation_timeline_tracks_widget->delete_timeline_group(node_id);
                return false;
            }
            hcp_animation_object->voxel_interval_track_id = timeline_interval_track_id;

            timeline_interval_track_id = animation_timeline_tracks_widget->timeline_track_groups[index].add_interval_track("shader");
            if (timeline_interval_track_id == UINT_MAX) {
                animation_timeline_tracks_widget->delete_timeline_group(node_id);
                return false;
            }
            hcp_animation_object->voxel_shader_interval_track_id = timeline_interval_track_id;
        }
    }
    // =====================================================

    return true;
}

void  hcp_voxel_node_class::delete_node_entity() {
    if (!hcp_voxel || !vw_scene) return;
//std::cout << "hcp_voxel_node_class:create_hcp_voxel: delete_node_entity : 0000" << std::endl;
    delete_hcp_timeline_link();// +++++++
    vw_scene->scene_entities_manager.delete_object(node_entity_id, node_entity_category_id);
//std::cout << "hcp_voxel_node_class:create_hcp_voxel: delete_node_entity : 1111" << std::endl;
   // delete hcp_voxel; // Not to be used as this is called by the process of vw_scene->scene_entities_manager.delete_object(node_entity_id, node_entity_category_id);
   

//std::cout << "hcp_voxel_node_class:create_hcp_voxel: delete_node_entity : 2222" << std::endl;
}

void  hcp_voxel_node_class::delete_hcp_timeline_link() {
    animation_timeline_tracks_widget->delete_timeline_group(node_id);
    delete hcp_animation_object;
    hcp_animation_object       = nullptr;
    timeline_interval_track_id = UINT_MAX;

    // If have more than one group 
    // : delete group animation object and initialise variables to unassigned values here
}

void hcp_voxel_node_class::editor_menu_options(){

    std::string menu_text = "Voxel ... ###nvoxel" + std::to_string(node_id);
    if (ImGui::BeginMenu(menu_text.c_str())) {
        menu_text = "Define Surface points###nvfs" + std::to_string(node_id);
        if (ImGui::MenuItem(menu_text.c_str())) {
            voxel_main_window_menu_functions.voxel_volume_to_voxel_surface(SELECTED_EXPORT, hcp_voxel->id, &vw_scene->scene_entities_manager, log_panel);
        }

        ImGui::EndMenu();
    }

    menu_text = "Timeline ... ###nvtimeline" + std::to_string(node_id);
    if (ImGui::BeginMenu(menu_text.c_str())) {

        menu_text = "Add timeline tracks###nvfs" + std::to_string(node_id);
        if (ImGui::MenuItem(menu_text.c_str())) {
            if (hcp_animation_object == nullptr) // Mist have no existing animation timeline object linked to this node
                create_hcp_timeline_link();
        }

        menu_text = "Delete ... ###nvtracks" + std::to_string(node_id);
        if (ImGui::BeginMenu(menu_text.c_str())) {
            menu_text = "Delete timeline group###nvfs" + std::to_string(node_id);
            if (ImGui::MenuItem(menu_text.c_str())) {
                delete_hcp_timeline_link();
            }

            // When more than one track in a group uncomment the following
            menu_text = "Delete generation track###nvfs" + std::to_string(node_id);
            if (ImGui::MenuItem(menu_text.c_str())) {
                if (hcp_animation_object) {// Must have a timeline animation object linked to this node
                    animation_timeline_tracks_widget->delete_timeline_group_track(node_id, hcp_animation_object->voxel_interval_track_id);
                    hcp_animation_object->voxel_interval_track_id = -1; 
                }
            }

            menu_text = "Delete shader track###nvfs" + std::to_string(node_id);
            if (ImGui::MenuItem(menu_text.c_str())) {
                if (hcp_animation_object) {// Must have a timeline animation object linked to this node
                    animation_timeline_tracks_widget->delete_timeline_group_track(node_id, hcp_animation_object->voxel_shader_interval_track_id);
                    hcp_animation_object->voxel_shader_interval_track_id = -1;
                }
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }

    menu_text = "Export ...###nexport" + std::to_string(node_id);
    if (ImGui::BeginMenu(menu_text.c_str())) {
        if (hcp_voxel && vw_scene) { // Must have a defined voxel and a scene with voxel data in it.
            menu_text = "As Point Cloud###nvepc" + std::to_string(node_id);
            if (ImGui::MenuItem(menu_text.c_str())) {
//std::cout << "hcp_voxel_node_class::editor_menu_options ^^^^^^^^ : " << vw_scene->scene_entities_manager.get_objects_of_category(SCENE_CATEGORY_HCP_VOXEL).category_objects.size() << ":" << globalc::get_current_selected_entity_id() << ":" << hcp_voxel->id << std::endl;
                voxel_main_window_menu_functions.export_voxels_center_point_data(SELECTED_EXPORT, hcp_voxel->id, &vw_scene->scene_entities_manager, log_panel);
            }

            menu_text = "As Point Surface###nveps" + std::to_string(node_id);
            if (ImGui::MenuItem(menu_text.c_str())) {
                voxel_main_window_menu_functions.export_voxels_point_surface_data(SELECTED_EXPORT, hcp_voxel->id, &vw_scene->scene_entities_manager, log_panel);
            }

            menu_text = "As Face Surface###nvefs" + std::to_string(node_id);
            if (ImGui::MenuItem(menu_text.c_str())) {
                voxel_main_window_menu_functions.export_voxels_surface_face_data(SELECTED_EXPORT, hcp_voxel->id, &vw_scene->scene_entities_manager, log_panel);
            }
        }
        ImGui::EndMenu();
    }    
}

bool hcp_voxel_node_class::define_ui() {
    if (!hcp_voxel) {
        std::cout << "hcp_voxel_node_class : display_ui HCP Voxel 0000 : hcp_voxel == NULL \n";
        return false;
    }
//std::cout << "hcp_voxel_node_class : display_ui HCP VOxel 0000 " << node_id << " : " << hcp_voxel->id << " : " << hcp_voxel->object_category_id << " : " << hcp_voxel->object_type_id << std::endl;
    globalc::set_current_selected_data_context_id(ENTITY_DATA_CONTEXT);
    globalc::set_current_selected_entity_id(hcp_voxel->id);
    globalc::set_current_selected_entity_category_type_id(hcp_voxel->object_category_id);
    globalc::set_current_selected_entity_type_id(hcp_voxel->object_type_id);

    return true; 
}

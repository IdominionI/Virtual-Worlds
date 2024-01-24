#include "hex_surface_main_menu_functions.h"

#include "../../Editor/Scene/hex_surface_scene_objects.h"

void hex_surface_main_menu_functions_class::export_hex_surface_center_point_data(int export_selection, id_type current_selected_entity_id, scene_entities_manager_class *scene_manager, log_panel_class *log_panel) {
    if (scene_manager == NULL) {
        if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export hex surface point geometry :: Cannot export hex surface point dat to file:: Scene Manager is undefined\n");
    }

    vw_scene_objects_category_class *hex_surface_scene_objects = get_hex_surface_scene_objects(scene_manager);
    
    if (hex_surface_scene_objects->category_objects.size() < 1) {
        if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Hex surface geometry :: Cannot perform hex surface function:: No entity data in scene to export\n");
        vwDialogs::message_box("ERROR : Export hex surface geometry", "Cannot export hex point surface to file::No entity data in scene to export");
        return;
    }

    //if (hex_surface_scene_objects == NULL) return;

    export_hex_surface_geometry.export_hex_surface_center_points_ply(*hex_surface_scene_objects, export_selection, current_selected_entity_id);
}

void hex_surface_main_menu_functions_class::export_hex_surface_surface_face_data(int export_selection, id_type current_selected_entity_id, scene_entities_manager_class *scene_manager, log_panel_class *log_panel) {
    if (scene_manager == NULL) {
        if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export hex surface surface geometry :: Cannot export hex surface surface dat to file:: Scene Manager is undefined\n");
    }

    vw_scene_objects_category_class *hex_surface_scene_objects = get_hex_surface_scene_objects(scene_manager);

    if (hex_surface_scene_objects->category_objects.size() < 1) {
        if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Hex surface geometry :: Cannot perform hex surface face function:: No entity data in scene to export\n");
        vwDialogs::message_box("ERROR : Export hex surface face geometry", "Cannot export hex surface face to file::No entity data in scene to export");
        return;
    }

   // if (hex_surface_scene_objects == NULL) return;
    export_hex_surface_geometry.export_hex_surface_faces_ply(*hex_surface_scene_objects, export_selection, current_selected_entity_id);
}

// -------------------------------------

vw_scene_objects_category_class* hex_surface_main_menu_functions_class::get_hex_surface_scene_objects(scene_entities_manager_class *scene_manager) {
    //int i = scene_manager->entities_manager.get_objects_of_category_index(ENTITY_CATEGORY_HEX_SURF);
    int i = scene_manager->get_category_index(SCENE_CATEGORY_HEX_SURFACE);
    if (i < 0) return NULL;

    vw_scene_objects_category_class* hex_surface_scene_objects = dynamic_cast<vw_scene_objects_category_class*>(&scene_manager->scene_objects[i]);
    return hex_surface_scene_objects;
}

//vw_scene_objects_category_class* voxel_hcp_entities = dynamic_cast<vw_scene_objects_category_class*>(&scene_manager->scene_objects[i]);
//return voxel_hcp_entities;
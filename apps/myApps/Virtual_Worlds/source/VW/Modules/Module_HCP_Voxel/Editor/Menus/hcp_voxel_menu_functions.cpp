#include "hcp_voxel_menu_functions.h"

#include "../../Render/voxel_hcp_render.h"
#include "../../Functions/vw_voxel_exports.h"

    void voxel_main_window_menu_functions_class::voxel_volume_to_voxel_surface(int selection, id_type current_selected_entity_id, scene_entities_manager_class *scene_manager,log_panel_class *log_panel) {
        if (scene_manager == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel surface geometry :: Cannot perform voxel volume to surface function:: Scene Manager is undefined\n");
            return;
        }

        vw_scene_objects_category_class *voxel_hcp_objects = get_voxel_hcp_scene_objects(scene_manager);

        if (voxel_hcp_objects->category_objects.size() < 1) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel surface geometry :: Cannot perform voxel volume to surface function:: No entity data in scene to export\n");
            vwDialogs::message_box("ERROR : Export voxel surface geometry", "Cannot export voxel point surface to file::No entity data in scene to export");
            return;
        }

        //voxel_object_data_class    voxel_object_surface_data;

        switch (selection) {
            case SELECTED_EXPORT: voxel_center_points_selected(*voxel_hcp_objects, current_selected_entity_id, log_panel); break;
            //case ACTIVE_EXPORT  : voxel_center_points_active(*voxel_hcp_objects, scene_manager, log_panel);  break;
            //case ALL_EXPORT     : voxel_center_points_all(*voxel_hcp_objects, scene_manager, log_panel);	   break;

        }
    }


    void voxel_main_window_menu_functions_class::export_voxels_center_point_data(int export_selection, id_type current_selected_entity_id, scene_entities_manager_class *scene_manager, log_panel_class *log_panel) {
        if (scene_manager == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point dat to file:: Scene Manager is undefined\n");
        }

        vw_scene_objects_category_class *voxel_hcp_entities_to_export = get_voxel_hcp_scene_objects(scene_manager);
        if (voxel_hcp_entities_to_export == NULL) return;
        export_voxel_geometry.export_voxel_center_points_ply(*voxel_hcp_entities_to_export, export_selection, current_selected_entity_id);
    }


    void  voxel_main_window_menu_functions_class::export_voxels_point_surface_data(int export_selection, id_type current_selected_entity_id, scene_entities_manager_class *scene_manager, log_panel_class *log_panel) {
        if (scene_manager == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point surface to file:: Scene Manager is undefined\n");
        }

        vw_scene_objects_category_class *voxel_hcp_entities_to_export = get_voxel_hcp_scene_objects(scene_manager);
        if (voxel_hcp_entities_to_export == NULL) return;

        export_voxel_geometry.export_voxel_point_surface_data_ply( voxel_hcp_entities_to_export, export_selection, current_selected_entity_id);
    }


    void  voxel_main_window_menu_functions_class::export_voxels_surface_face_data(int export_selection, id_type current_selected_entity_id, scene_entities_manager_class *scene_manager, log_panel_class *log_panel) {
        if (scene_manager == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point surface to file:: Scene Manager is undefined\n");
        }

        vw_scene_objects_category_class* voxel_hcp_entities_to_export = get_voxel_hcp_scene_objects(scene_manager);
        if (voxel_hcp_entities_to_export == NULL) return;

        export_voxel_geometry.export_voxel_surface_faces_data_ply( voxel_hcp_entities_to_export, export_selection, current_selected_entity_id);
    }

//--------------------------------------------
    
    void voxel_main_window_menu_functions_class::voxel_center_points_selected(vw_scene_objects_category_class &voxel_hcp_entities, id_type entity_id, log_panel_class* log_panel) {
    
        if (entity_id < 0) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point surface to file:: No entity selected to export\n");
            vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel point surface to file::No entity selected to export");
            return;
        }

        vw_object_base_class   *object           = voxel_hcp_entities.get_entity_object(entity_id);
        voxel_hcp_object_class *voxel_hcp_object = dynamic_cast<voxel_hcp_object_class *> (object);


        if (voxel_hcp_object == NULL) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point surface to file:: Could not find entity in scene data to export\n");
            vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel point surface to file::Could not find entity in scene data to export");
            return;
        }

        create_voxel_center_point_data(voxel_hcp_object, entity_id, log_panel);

//printf("voxel_center_points_ply_selected 44 %i\n", entity_id);
    }
/*
    void voxel_main_window_menu_functions_class::voxel_center_points_active(voxel_hcp_scene_objects_class& voxel_hcp_entities_to_export, scene_manager_class *scene_manager,  log_panel_class* log_panel) {
        for (index_type i = 0; i < voxel_hcp_entities_to_export.size(); i++) {
            voxel_hcp_object_class* voxel_hcp_object = voxel_hcp_entities_to_export[i];

            if (voxel_hcp_object == NULL) {
                if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point surface to file:: No voxel entities defined\n");
                vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel point surface to file::Could not find entity in scene data to export");
            }

            if (voxel_hcp_object->active_object) {
                if (voxel_hcp_object->voxel_object_data.voxel_matrix_data.size() > 0) {
                    create_voxel_center_point_data(voxel_hcp_object, voxel_hcp_object->object_id, scene_manager, log_panel);
                }
                else {
                    if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel volume to voxel surface :: Cannot convert voxel point surface for object:: %s \nNo voxel surface face mesh generated\n", voxel_hcp_object->object_name);
                }
            }
        }
    }

    void voxel_main_window_menu_functions_class::voxel_center_points_all(voxel_hcp_scene_objects_class& voxel_hcp_entities_to_export, scene_manager_class *scene_manager,log_panel_class* log_panel) {
        for (index_type i = 0; i < voxel_hcp_entities_to_export.size(); i++) {
            voxel_hcp_object_class* voxel_hcp_object = voxel_hcp_entities_to_export[i];

            if (voxel_hcp_object == NULL) {
                if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Export voxel surface geometry :: Cannot export voxel point surface to file:: No voxel entities defined\n");
                vwDialogs::message_box("Export voxel surface geometry", "Cannot export voxel point surface to file::Could not find entity in scene data to export");
            }

            if (voxel_hcp_object->voxel_object_data.voxel_matrix_data.size() > 0) {
                create_voxel_center_point_data(voxel_hcp_object, voxel_hcp_object->object_id,scene_manager, log_panel);
            }
            else {
                if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel volume to voxel surface :: Cannot convert voxel point surface for object:: %s \nNo voxel surface face mesh generated\n", voxel_hcp_object->object_name);
            }

        }
    }
*/

    void voxel_main_window_menu_functions_class::create_voxel_center_point_data(voxel_hcp_object_class* voxel_hcp_object, id_type entity_id, log_panel_class *log_panel) {
        voxel_hcp_render_class  voxel_hcp_render;
        voxel_surface_data_class voxel_surface_data;
        voxel_surface_data.cloud = voxel_hcp_object;
//printf("create_voxel_center_point_data 00\n");


        if (!voxel_surface_data.create_voxel_surface_point_data()) {
//printf("create_voxel_center_point_data 11\n");
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel volume to voxel surface :: Cannot convert voxel point surface for object:: %s \nNo voxel surface face mesh generated\n", voxel_hcp_object->name);
        }

        if (voxel_surface_data.surface_vertices.size() < 1) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel volume to voxel surface :: Cannot convert voxel point surface for object:: %s \nNo voxel vertex data\n", voxel_hcp_object->name);
            return;
        }

        if (voxel_surface_data.surface_vertices_normals.size() < 1) {
            if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel volume to voxel surface :: Cannot convert voxel point surface for object:: %s \nNo voxel normal data\n", voxel_hcp_object->name);
            return;
        }
//printf("create_voxel_center_point_data 22\n");
        voxel_object_data_class    voxel_object_surface_data;
        voxel_object_surface_data.create_empty_volume_cubic(voxel_hcp_object->voxel_object_data.matrix_dimension.x, voxel_hcp_object->voxel_object_data.matrix_dimension.y, voxel_hcp_object->voxel_object_data.matrix_dimension.z);

        for (int i = 0; i < voxel_surface_data.surface_vertices.size(); i++) {
            voxel_object_surface_data.voxel_matrix_data[voxel_surface_data.surface_vertices[i].voxel_index] = voxel_hcp_object->voxel_object_data.voxel_matrix_data[voxel_surface_data.surface_vertices[i].voxel_index];
        }

        voxel_hcp_object->voxel_object_data.voxel_matrix_data.clear();
        voxel_hcp_object->voxel_object_data.voxel_matrix_data.shrink_to_fit();

        voxel_hcp_object->voxel_object_data.voxel_matrix_data = voxel_object_surface_data.voxel_matrix_data;

        //voxel_hcp_object->define_vbo_vertices(MIN_VOXEL_VALUE, MAX_VOXEL_VALUE);// need to define values for min/max voxel value range or have incorrect to misleading display
        voxel_hcp_object->define_geometry_data();// need to define values for min/max voxel value range or have incorrect to misleading display

//printf("create_voxel_center_point_data 44\n");
    }

    vw_scene_objects_category_class *voxel_main_window_menu_functions_class::get_voxel_hcp_scene_objects(scene_entities_manager_class *scene_manager) {
        int i = scene_manager->get_category_index(SCENE_CATEGORY_HCP_VOXEL);
        if (i < 0) return NULL;

        vw_scene_objects_category_class* voxel_hcp_entities = dynamic_cast<vw_scene_objects_category_class *>(&scene_manager->scene_objects[i]);
        return voxel_hcp_entities;
    }
#pragma once


#include <VW/Editor/Main_Window/Panels/log_panel.h>

#include <VW_Framework/Scene/scene_entities_manager.h>

#include "../../Functions/hex_surface_exports.h"

class hex_surface_main_menu_functions_class {
public:
    void export_hex_surface_center_point_data(int export_selection, id_type current_selected_entity_id, scene_entities_manager_class *scene_manager, log_panel_class* log_panel);
    void export_hex_surface_surface_face_data(int export_selection, id_type current_selected_entity_id, scene_entities_manager_class *scene_manager, log_panel_class* log_panel);

private:
    export_hex_surface_geometry_class export_hex_surface_geometry;

    vw_scene_objects_category_class* get_hex_surface_scene_objects(scene_entities_manager_class *scene_manager);

};

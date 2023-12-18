#pragma once

void define_node_editor_nodes(node_editor_category_manager_class* node_editor_category_manager,
    log_panel_class* log_panel,
    scene_manager_class* scene_manager)

{
    //if (node_editor_category_manager == NULL) {
    //    log_panel->application_log.AddLog("ERROR : Could not define editor nodes for hco voxel entities.\n  Node editor category manager not defined\n");
    //    return;
    //}

    //if (scene_manager == NULL) {
    //    log_panel->application_log.AddLog("ERROR : Could not define editor nodes for hco voxel entities.\n No Scene manager defined.\n");
    //    return;
    //}

    // ********************************************************

    // Define voxel editor nodes db
    int index = node_editor_category_manager->add_editor_node_category(ENTITY_CATEGORY_HCP_VOXEL, log_panel);

    if (index < 0) {
        log_panel->application_log.AddLog("ERROR : Could not define editor nodes for hco voxel entities\n");
        return;
    }

    if (log_panel != NULL) log_panel->application_log.AddLog("Defined HCP voxel editor nodes category\n");

    hcp_voxel_editor_node_class* voxel_hcp_voxel_editor_node = new hcp_voxel_editor_node_class;

    if (voxel_hcp_voxel_editor_node == NULL) {
        if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not define HCP voxel entity node\n");
    }
    else {
        //printf("voxel_hcp_scene_objects != NULL\n");
        voxel_hcp_voxel_editor_node->editor_node_definition.category = ENTITY_CATEGORY_HCP_VOXEL;
        voxel_hcp_voxel_editor_node->editor_node_definition.context = HCP_CONTEXT_VOXEL_MATRIX;
        voxel_hcp_voxel_editor_node->editor_node_definition.function = HCP_VOXEL_FUNCTION_ENTITY;
        voxel_hcp_voxel_editor_node->editor_node_definition.function_category = FUNCTION_CATEGORY_GENERATOR;

        if (!node_editor_category_manager->add_editor_node(voxel_hcp_voxel_editor_node, scene_manager, log_panel)) {
            if (log_panel != NULL) log_panel->application_log.AddLog("APPLICATION PROBLEM :: Cound not add HCP voxel entity node\n");
        }
    }
}

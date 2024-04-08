#include "translation_node.h"

#include <Universal_FW/ImNodes/Kernal/node_editor.h>>
#include <Universal_FW/Timeline/timeline_tracks.h>

#include <VW/Editor/Node_Editor/node_editor_extras.h>
#include <VW/Editor/Import_Export/vw_import_export_parameters.h>

// ---  template to avoid circular referencing ------
// --- timeline animation class placed here that ----
// --- includes the node class and its functions ----
// --- to execute defined in the node class .h file -

#include "Animation/hcp_voxel_translation_animation.h"
hcp_voxel_translation_animation_class *hcp_animation_object = nullptr;
// -----------------

bool hcp_voxel_translation_node_class::define_node(ImVec2 click_pos,id_type entity_id_) {
    node_dimensions = { 60.0f,5.0f };

    // !!! CRITICAL : MUST HAVE NODE UI NODE TYPE DEFINITION DEFINED
    // !!! ELSE CANNOT LINK NODE PINS, PERFORM EVALUATIONS OR DISPLAY
    // !!! NODE PARAMETERS TO BE DISPLAYED AND MODIFIED AS WELL AS
    // !!! OTHER NODE AND NODE EDITOR FUCTIONALITY

    ui_node_type.category          = NODE_CATEGORY_ID_MODIFIER;
    ui_node_type.node_type         = NODE_TYPE_ID_HCP_VOXEL_TRANSLATION;
    ui_node_type.node_data_context = NODE_DATA_CONTEXT;

    label = "HCP Translation " + std::to_string(node_id);

    node_draw_data.node_header_color          = ImColor(175, 130, 15, 255);
    node_draw_data.node_header_hovered_color  = ImColor(180, 155, 80, 255);
    node_draw_data.node_header_selected_color = ImColor(200, 190, 145, 255);
    node_draw_data.node_header_label_color    = ImColor(255, 255, 255, 255);// ImGui does not have a color function that accepts its own ImColor type for textS

    node_draw_data.node_body_color            = NODE_HCP_DRAW_BODY_COLOR          ;
    node_draw_data.node_body_hovered_color    = NODE_HCP_DRAW_BODY_HOVERED_COLOR  ;
    node_draw_data.node_body_selected_color   = NODE_HCP_DRAW_BODY_SELECTED_COLOR ;
    node_draw_data.node_outline_color         = NODE_HCP_DRAW_OUTLINE_COLOR       ;

    graph->node_editor->SetNodeScreenSpacePos(node_id, click_pos);
   
    return true;
}

bool hcp_voxel_translation_node_class::define_import_node(ImVec2 click_pos, node_id_type entity_id_, void* node_misc_data) {
    node_misc_import_struct_type* node_misc_import_data = static_cast<node_misc_import_struct_type*>(node_misc_data);

    if (!node_misc_import_data) {
        std::cout << "ERROR : Editor scene and log data not defined. Aborting import of HCP translation node.\n";
        return false;
    }

    if (node_misc_import_data->log_panel == nullptr) {
        std::cout << "WARNING : Imported HCP translation node has no defined log panel to output messages to.\n";
    }

    if (node_misc_import_data->vw_scene == nullptr) {
        std::cout << "ERROR : Imported HCP traslation node has no defined scene manager. Aborted import.\n" << std::endl;
        if (log_panel) log_panel->application_log.AddLog("ERROR :Imported HCP translation node has no defined scene manager. Aborted import.\n");
        return false;
    }

    log_panel = node_misc_import_data->log_panel;
    vw_scene = node_misc_import_data->vw_scene;

    return define_node(click_pos, entity_id_);
}

bool hcp_voxel_translation_node_class::create_hcp_translation_timeline_link() {
       hcp_animation_object = new hcp_voxel_translation_animation_class;

    if (!hcp_animation_object) return false;

    hcp_animation_object->hcp_voxel_translation_node = this;

    // ================ define as a create timeline group function ===============
    if (!animation_timeline_tracks_widget->timeline_track_group_exists(node_id)) {
//std::cout << "hcp_voxel_node_class::create_hcp_animation_link 000 : !animation_timeline_tracks_widget->timeline_track_group_exists(node_id)\n";
        int index = animation_timeline_tracks_widget->add_timeline_group(label, node_id, hcp_animation_object, TIMELINE_OBJECT_DATA_TYPE_ID_VOXEL_TRANSLATION);
        if (index != INVALID_ID && index < animation_timeline_tracks_widget->timeline_track_groups.size()) {
            
            timeline_interval_track_id = animation_timeline_tracks_widget->timeline_track_groups[index].add_interval_track("hcp voxel\nTranslation");
            if (timeline_interval_track_id == UINT_MAX) {
                animation_timeline_tracks_widget->delete_timeline_group(node_id);
                return false;
            }
        }
    }
    // =====================================================

    return true;
}

void hcp_voxel_translation_node_class::delete_hcp_translation_timeline_link() {
    animation_timeline_tracks_widget->delete_timeline_group(node_id);
    delete hcp_animation_object;
    hcp_animation_object = nullptr;
    timeline_interval_track_id = UINT_MAX;

    // If have more tha one trck in a group
    // : delete track hcp_animation_object and initialise variables to unassigned values here
}

void hcp_voxel_translation_node_class::editor_menu_options() {
    std::string menu_text = "timeline###anvfs" + std::to_string(node_id);
    if (ImGui::BeginMenu(menu_text.c_str())) {
        menu_text = "Add timeline track###anvfs" + std::to_string(node_id);
        if (ImGui::MenuItem(menu_text.c_str())) {
            if (hcp_animation_object == nullptr)
                create_hcp_translation_timeline_link();
        }

        menu_text = "Delete timeline track###nvfs" + std::to_string(node_id);
        if (ImGui::MenuItem(menu_text.c_str())) {
            delete_hcp_translation_timeline_link();
        }

        // When more than one track in a group uncomment the following
        //menu_text = "Delete animation track###anvfs" + std::to_string(node_id);
        //if (ImGui::MenuItem(menu_text.c_str())) {
        //    animation_timeline_tracks_widget->delete_timeline_group_track(node_id, timeline_interval_track_id);
        //}

      ImGui::EndMenu();
 }
}


bool hcp_voxel_translation_node_class::define_ui() {
//std::cout << "hcp_voxel_automata_node_class::display_ui\n";

    globalc::set_current_selected_data_context_id(this->ui_node_type.node_data_context);
    globalc::set_current_selected_entity_id(INVALID_ID);
    globalc::set_current_selected_entity_category_type_id(INVALID_ID);
    globalc::set_current_selected_entity_type_id(INVALID_ID);

    globalc::set_current_selected_node(this);

    return true;
}

void hcp_voxel_translation_node_class::display_ui(id_type current_selected_object_id) {
//std::cout << "hcp_voxel_translation_node_class::display_ui\n";

    // Best to perform this code block only when connecting or diconnecting node input link
    if (!vw_scene ) {
        // error message
        return;
    }

    display_translation_ui(current_selected_object_id);
}

void hcp_voxel_translation_node_class::display_translation_ui(id_type current_selected_object_id) {
    float x_pos = 10.0f, y_pos = 60.0f;
    translation = { 0.0f,0.0f,0.0f };
//printf("hcp_voxel_translation_node_class::display_ui 00000 \n");
    text("HCP Voxel Matrix Translation", x_pos+100, y_pos);
    y_pos += 40;
    text("Coordinate     value       step active", x_pos+50, y_pos);

    //float_input_delta("X location", x_loc, 1.0f, x_pos+100, y_pos + 20, 50, "###tbu", "###tbu");
    std::string id_prefix = "###nwui"+ FW::stringtools::toString(node_id); std::string w_id_prefix = ""; std::string w_id = "";

    y_pos += 30;
    w_id_prefix = id_prefix+"x";
    text("X :     ", x_pos + 75, y_pos);
    ImGui::SameLine(); w_id = w_id_prefix + "2"; ImGui::SetNextItemWidth(75); ImGui::InputFloat(w_id.c_str(), &translation_step[0]);
    // ************************** Added increment/decrement buttons ********************************
    float t_height = ImGui::GetTextLineHeight();
    float b_height = ImGui::GetTextLineHeight() / 2.0f;
    float xpos = ImGui::GetCursorPosX(); float ypos = ImGui::GetCursorPosY();// These x,y positions seem to be strangely aligned with the diplayed widgets

    ImGui::SameLine(); w_id = w_id_prefix + "5"; ImGui::SetNextItemWidth(15);// ImGui::SetCursorPosY(-3);
    ImGui::SetCursorPosY(ypos - t_height - b_height - 5); // This seems to change the alignement and coordinates for all following widgets that use ImGui::SameLine()
                                                        // also  seems it needs to be defined after ImGui::SameLine() or does not take effect
    if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
        translation_step[0] += step[0];
    }

    ImGui::SameLine(); w_id = w_id_prefix + "6"; ImGui::SetNextItemWidth(15);
    ImGui::SetCursorPosY(ypos - b_height - 8);
    ImGui::SetCursorPosX(xpos + 226);

    if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
        translation_step[0] -= step[0];
    }

    ImGui::SameLine(); ImGui::SetCursorPosY(ypos - b_height - 18);
    w_id = w_id_prefix + "3"; ImGui::SetNextItemWidth(50); ImGui::InputFloat(w_id.c_str(), &step[0]);
    ImGui::SameLine(); w_id = w_id_prefix + "4"; ImGui::Checkbox(w_id.c_str(), &active[0]);

    // ************************** Added increment/decrement buttons ********************************
    t_height = ImGui::GetTextLineHeight();
    b_height = ImGui::GetTextLineHeight() / 2.0f;
    xpos = ImGui::GetCursorPosX(); ypos = ImGui::GetCursorPosY();// These x,y positions seem to be strangely aligned with the diplayed widgets

    ImGui::SameLine(); w_id = w_id_prefix + "7"; ImGui::SetNextItemWidth(15);// ImGui::SetCursorPosY(-3);
    ImGui::SetCursorPosY(ypos - t_height - b_height - 5); // This seems to change the alignement and coordinates for all following widgets that use ImGui::SameLine()
                                                        // also  seems it needs to be defined after ImGui::SameLine() or does not take effect
    if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
        translation[0] = step[0];
        action();
    }

    ImGui::SameLine(); w_id = w_id_prefix + "8"; ImGui::SetNextItemWidth(15);
    ImGui::SetCursorPosY(ypos - b_height - 8);
    ImGui::SetCursorPosX(xpos + 327);

    if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
        translation[0] = -step[0];
        action();
    }


    //----------------------------------------------------------------------------------------

    y_pos += 20;
    w_id_prefix = id_prefix + "y";
    text("Y :     ", x_pos + 75, y_pos);
    ImGui::SameLine(); w_id = w_id_prefix + "2"; ImGui::SetNextItemWidth(75); ImGui::InputFloat(w_id.c_str(), &translation_step[1]);
    // ************************** Added increment/decrement buttons ********************************
    t_height = ImGui::GetTextLineHeight();
    b_height = ImGui::GetTextLineHeight() / 2.0f;
    xpos = ImGui::GetCursorPosX(); ypos = ImGui::GetCursorPosY();// These x,y positions seem to be strangely aligned with the diplayed widgets

    ImGui::SameLine(); w_id = w_id_prefix + "5"; ImGui::SetNextItemWidth(15);// ImGui::SetCursorPosY(-3);
    ImGui::SetCursorPosY(ypos - t_height - b_height - 5); // This seems to change the alignement and coordinates for all following widgets that use ImGui::SameLine()
                                                        // also  seems it needs to be defined after ImGui::SameLine() or does not take effect
    if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
        translation_step[1] += step[1];
    }

    ImGui::SameLine(); w_id = w_id_prefix + "6"; ImGui::SetNextItemWidth(15);
    ImGui::SetCursorPosY(ypos - b_height - 8);
    ImGui::SetCursorPosX(xpos + 226);

    if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
        translation_step[1] -= step[1];
    }

    ImGui::SameLine(); ImGui::SetCursorPosY(ypos - b_height - 18);
    w_id = w_id_prefix + "3"; ImGui::SetNextItemWidth(50); ImGui::InputFloat(w_id.c_str(), &step[1]);
    ImGui::SameLine(); w_id = w_id_prefix + "4"; ImGui::Checkbox(w_id.c_str(), &active[1]);

    // ************************** Added increment/decrement buttons ********************************
    t_height = ImGui::GetTextLineHeight();
    b_height = ImGui::GetTextLineHeight() / 2.0f;
    xpos = ImGui::GetCursorPosX(); ypos = ImGui::GetCursorPosY();// These x,y positions seem to be strangely aligned with the diplayed widgets

    ImGui::SameLine(); w_id = w_id_prefix + "7"; ImGui::SetNextItemWidth(15);// ImGui::SetCursorPosY(-3);
    ImGui::SetCursorPosY(ypos - t_height - b_height - 5); // This seems to change the alignement and coordinates for all following widgets that use ImGui::SameLine()
                                                        // also  seems it needs to be defined after ImGui::SameLine() or does not take effect
    if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
        translation[1] = step[1];
        action();
    }

    ImGui::SameLine(); w_id = w_id_prefix + "8"; ImGui::SetNextItemWidth(15);
    ImGui::SetCursorPosY(ypos - b_height - 8);
    ImGui::SetCursorPosX(xpos + 327);

    if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
        translation[1] = -step[1];
        action();
    }
    //---------------------------------------------------------------------------------
 
    y_pos += 20;
    w_id_prefix = id_prefix + "z";
    text("Z :     ", x_pos + 75, y_pos);
    ImGui::SameLine(); w_id = w_id_prefix + "2"; ImGui::SetNextItemWidth(75); ImGui::InputFloat(w_id.c_str(), &translation_step[2]);
    // ************************** Added increment/decrement buttons ********************************
    t_height = ImGui::GetTextLineHeight();
    b_height = ImGui::GetTextLineHeight() / 2.0f;
    xpos = ImGui::GetCursorPosX(); ypos = ImGui::GetCursorPosY();// These x,y positions seem to be strangely aligned with the diplayed widgets

    ImGui::SameLine(); w_id = w_id_prefix + "5"; ImGui::SetNextItemWidth(15);// ImGui::SetCursorPosY(-3);
    ImGui::SetCursorPosY(ypos - t_height - b_height - 5); // This seems to change the alignement and coordinates for all following widgets that use ImGui::SameLine()
                                                        // also  seems it needs to be defined after ImGui::SameLine() or does not take effect
    if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
        translation_step[2] += step[2];;
    }

    ImGui::SameLine(); w_id = w_id_prefix + "6"; ImGui::SetNextItemWidth(15);
    ImGui::SetCursorPosY(ypos - b_height - 8);
    ImGui::SetCursorPosX(xpos + 226);

    if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
        translation_step[2] -= step[2];
    }

    ImGui::SameLine(); ImGui::SetCursorPosY(ypos - b_height - 18);
    w_id = w_id_prefix + "3"; ImGui::SetNextItemWidth(50); ImGui::InputFloat(w_id.c_str(), &step[2]);
    ImGui::SameLine(); w_id = w_id_prefix + "4"; ImGui::Checkbox(w_id.c_str(), &active[2]);

    // ************************** Added increment/decrement buttons ********************************
    t_height = ImGui::GetTextLineHeight();
    b_height = ImGui::GetTextLineHeight() / 2.0f;
    xpos = ImGui::GetCursorPosX(); ypos = ImGui::GetCursorPosY();// These x,y positions seem to be strangely aligned with the diplayed widgets

    ImGui::SameLine(); w_id = w_id_prefix + "7"; ImGui::SetNextItemWidth(15);// ImGui::SetCursorPosY(-3);
    ImGui::SetCursorPosY(ypos - t_height - b_height - 5); // This seems to change the alignement and coordinates for all following widgets that use ImGui::SameLine()
                                                        // also  seems it needs to be defined after ImGui::SameLine() or does not take effect
    if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
        translation[2] = step[2];
        action();
    }

    ImGui::SameLine(); w_id = w_id_prefix + "8"; ImGui::SetNextItemWidth(15);
    ImGui::SetCursorPosY(ypos - b_height - 8);
    ImGui::SetCursorPosX(xpos + 327);

    if (ImGui::ButtonEx(w_id.c_str(), ImVec2(15, b_height + 3))) {
        translation[2] = -step[2];
        action();
    }
    //---------------------------------------------------------------------------------

    y_pos += 40;

    if (ex_button(ICON_FA_STEP_BACKWARD "###nwuidt", x_pos + 145, y_pos, 50, 20))
        decrement_tanslation();


    if (ex_button(ICON_FA_STEP_FORWARD "###nwuiit", x_pos + 245, y_pos, 50, 20))
        increment_translation();

    y_pos += 40;

    if (ex_button("Relative###nwuirt", x_pos + 240, y_pos, 80, 20))
        coordinate_translation();
    y_pos += 30;
    if (ex_button("World###nwuiat", x_pos + 240, y_pos, 80, 20))
        coordinate_translation(false);

}

void hcp_voxel_translation_node_class::action() {
//printf("translation action 0000 %i \n", node_id);
    voxel_hcp_object_class *voxel_hcp_object = get_voxel_hcp_object_to_execute();

//printf("translation action 11111 %i \n", modify_node->node_id);

    if(!vw_scene) return;
//printf("translation action 33333 %i \n",modify_node->entity_id);

    if (voxel_hcp_object == NULL) return;
//printf("translation action 55555  \n");
    if(translate)
        voxel_hcp_object->voxel_object_data.matrix_origin += translation;
    else
        voxel_hcp_object->voxel_object_data.matrix_origin = translation;
//printf("translation action 66666 %f : %f : %f \n", voxel_hcp_object->voxel_object_data.matrix_origin.x, voxel_hcp_object->voxel_object_data.matrix_origin.y, voxel_hcp_object->voxel_object_data.matrix_origin.z);
//printf("translation action 66666AAA %f : %f: %f \n", location.x, location.y, location.z);
        
    // !!!!!!!! FOLLOWING UPDATE CODE NEEDS TO BE PUT INTO A GENERAL UPDATE VOXEL VERTICES FUNCTION !!!!!!!!!!!!!!

    voxel_hcp_object->set_voxel_value_range(MIN_VOXEL_VALUE, MAX_VOXEL_VALUE);// This is to be changed to a user defined input in a future update
    voxel_hcp_object->define_geometry_data();// need to define values for min/max voxel value range or have incorrect to misleading display

        shader_class shader;

//printf("translation action 77777 %s \n");

        int shader_id = voxel_hcp_object->point_cloud->shader->getProgram();

        if (shader_id < 0) {
            // error message
            return;
        }

        shader.set_vec3(shader_id, voxel_hcp_object->voxel_object_data.matrix_origin, "voxel_origin");
}

void hcp_voxel_translation_node_class::decrement_tanslation() {
    for (int i = 0; i < 3; i++) {
        if (active[i]) {
            translation[i] = -step[i];
        }
    }
    action();
}

void hcp_voxel_translation_node_class::increment_translation() {
    for (int i = 0; i < 3; i++) {
        if (active[i]) {
            translation[i] = step[i];
        }
    }
    action();
}

void hcp_voxel_translation_node_class::coordinate_translation(bool relative_translation) {
    for (int i = 0; i < 3; i++) {
        translation[i] = translation_step[i];
    }

    translate = relative_translation;
    action();
    translate = true;
}

void  hcp_voxel_translation_node_class::export_hcp_translation_node_parameter_data(std::fstream &stream) {
    stream << DATA_BLOCK_START << std::endl;

    stream << location.x << std::endl;
    stream << location.y << std::endl;
    stream << location.z << std::endl;
        
    stream << step.x << std::endl;
    stream << step.y << std::endl;
    stream << step.z << std::endl;
        
    stream << translation.x << std::endl;
    stream << translation.y << std::endl;
    stream << translation.z << std::endl;
        
    stream << translation_step.x << std::endl;
    stream << translation_step.y << std::endl;
    stream << translation_step.z << std::endl;
        
    stream << active.x << std::endl;
    stream << active.y << std::endl;
    stream << active.z << std::endl;
        
    stream << DATA_BLOCK_END << std::endl;
}

bool hcp_voxel_translation_node_class::import_node(std::vector<std::string> lines, int& line_number) {
    std::string line;
    line_number++;

	line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "hcp_voxel_translation_node_class::import_node AAAAAAA : " << line_number << ":" << lines[line_number] << std::endl;
// next line read must be a RULE_BLOCK_START flag
	if (!FW::stringtools::contains(lines[line_number], DATA_BLOCK_START)) {
//std::cout << "hcp_voxel_translation_node_class", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\nMissing RULE_BLOCK_START flag", //QMessageBox::Ok);
		return false;
	}

//std::cout << "import_export_byte_automata_rules_class::read_automata_byte_rule CCCCCC : " << line_number << ":" << lines[line_number] << std::endl;
    line_number++; line = lines[line_number];location.x = stof(line);
    line_number++; line = lines[line_number];location.y = stof(line);
    line_number++; line = lines[line_number];location.z = stof(line);
        
    line_number++; line = lines[line_number];step.x = stof(line);
    line_number++; line = lines[line_number];step.y = stof(line);
    line_number++; line = lines[line_number];step.z = stof(line);

    line_number++; line = lines[line_number];translation.x = stof(line);
    line_number++; line = lines[line_number];translation.y = stof(line);
    line_number++; line = lines[line_number];translation.z = stof(line);

    line_number++; line = lines[line_number];translation_step.x = stof(line);
    line_number++; line = lines[line_number];translation_step.y = stof(line);
    line_number++; line = lines[line_number];translation_step.z = stof(line);

    line_number++; line = lines[line_number];active.x = stof(line);
    line_number++; line = lines[line_number];active.y = stof(line);
    line_number++; line = lines[line_number];active.z = stof(line);

	// next line read must be a RULE_BLOCK_END flag
	line_number++;
	line = lines[line_number];
//std::cout << "hcp_voxel_translation_node_class::import_node 2222AA : " << line_number << ":" << lines[line_number] << std::endl;

	if (!FW::stringtools::contains(lines[line_number], DATA_BLOCK_END)) {
//std::cout <<"hcp_voxel_translation_node_class", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\nMissing Rule block end flag", //QMessageBox::Ok);
		return false;
	}

	return true;
}

voxel_hcp_object_class *hcp_voxel_translation_node_class::get_voxel_hcp_object_to_execute(){
    if (!vw_scene) {
        // error message
        return nullptr;
    }

    voxel_hcp_object_class* voxel_hcp_object_to_execute = nullptr;
    // find link to input pin
    link_struct_type input_link = graph->links.find_link_with_end_pin(inputs[0]);// Only have one input pin which is defined as default

    if (input_link.link_id != INVALID_ID) {
        pin_struct_type link_from_pin = graph->pins.find_pin(input_link.from);

        if (link_from_pin.node_id != INVALID_ID) {
            node_basis_class* node = graph->nodes.get_node(link_from_pin.node_id);

            if (node) {
                //vw_object_base_class* hcp_voxel_base_object = vw_scene->scene_entities_manager.get_object_of_category(node->node_entity_id, node->node_entity_category_id);
                //if (hcp_voxel_base_object) {
                //    voxel_hcp_object_to_execute = dynamic_cast<voxel_hcp_object_class*>(hcp_voxel_base_object);
                //}

                 voxel_hcp_object_to_execute = (voxel_hcp_object_class*) node->get_node_data();
            }
        }
    }

    return voxel_hcp_object_to_execute;
}

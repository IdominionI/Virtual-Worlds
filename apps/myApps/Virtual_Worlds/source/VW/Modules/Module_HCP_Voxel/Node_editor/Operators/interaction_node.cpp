#include "interaction_node.h"

#include <Universal_FW/ImNodes/Kernal/node_editor.h>>
#include <Universal_FW/Timeline/timeline_tracks.h>

#include <VW/Editor/Node_Editor/node_editor_extras.h>
#include <VW/Editor/Import_Export/vw_import_export_parameters.h>

// ---  template to avoid circular referencing ------
// --- timeline animation class placed here that ----
// --- includes the node class and its functions ----
// --- to execute defined in the node class .h file -

#include "Animation/hcp_voxel_interaction_animation.h"
hcp_voxel_interaction_animation_class *hcp_animation_object = nullptr;
// -----------------

bool hcp_voxel_interaction_node_class::define_node(ImVec2 click_pos, id_type entity_id_) {
    node_dimensions = { 60.0f,5.0f };

    // !!! CRITICAL : MUST HAVE NODE UI NODE TYPE DEFINITION DEFINED
    // !!! ELSE CANNOT LINK NODE PINS, PERFORM EVALUATIONS OR DISPLAY
    // !!! NODE PARAMETERS TO BE DISPLAYED AND MODIFIED AS WELL AS
    // !!! OTHER NODE AND NODE EDITOR FUCTIONALITY

    ui_node_type.category          = NODE_CATEGORY_ID_OPERATOR;
    ui_node_type.node_type         = NODE_TYPE_ID_HCP_VOXEL_INTERACTION;
    ui_node_type.node_data_context = NODE_DATA_CONTEXT;

    label = "HCP Interaction " + std::to_string(node_id);
    //label = "HCP Interaction " + std::to_string(node_id) + ":"+std::to_string(interaction_object->id);

    node_header_color = ImColor(0, 139, 139, 255);
    node_header_hovered_color = ImColor(0, 175, 175, 255);
    node_header_selected_color = ImColor(0, 255, 255, 255);
    node_header_label_color = ImColor(255, 255, 255, 255);// ImGui does not have a color function that accepts its own ImColor type for textS

    node_body_color = ImColor(75, 75, 75, 255);
    node_body_hovered_color = ImColor(100, 100, 100, 255);
    node_body_selected_color = ImColor(150, 150, 150, 255);
    node_outline_color = ImColor(50, 50, 50, 255);

    graph->node_editor->SetNodeScreenSpacePos(node_id, click_pos);
   
    return true;
}

bool hcp_voxel_interaction_node_class::define_import_node(ImVec2 click_pos, node_id_type entity_id_, void* node_misc_data) {
    node_misc_import_struct_type* node_misc_import_data = static_cast<node_misc_import_struct_type*>(node_misc_data);

    if (!node_misc_import_data) {
        std::cout << "ERROR : Editor scene and log data not defined. Aborting import of HCP Interaction node.\n";
        return false;
    }

    if (node_misc_import_data->log_panel == nullptr) {
        std::cout << "WARNING : Imported HCP Interaction node has no defined log panel to output messages to.\n";
    }

    if (node_misc_import_data->vw_scene == nullptr) {
        std::cout << "ERROR : Imported HCP Interaction node has no defined scene manager. Aborted import.\n" << std::endl;
        if (log_panel) log_panel->application_log.AddLog("ERROR :Imported HCP Interaction node has no defined scene manager. Aborted import.\n");
        return false;
    }

    log_panel = node_misc_import_data->log_panel;
    vw_scene = node_misc_import_data->vw_scene;

    return define_node(click_pos, entity_id_);
}

bool hcp_voxel_interaction_node_class::create_hcp_interaction_timeline_link(){
    hcp_animation_object = new hcp_voxel_interaction_animation_class;

    if (!hcp_animation_object) return false;

    hcp_animation_object->hcp_voxel_interaction_node = this;

    // ================ define as a create timeline group function ===============
    if (!animation_timeline_tracks_widget->timeline_track_group_exists(node_id)) {
//std::cout << "hcp_voxel_interaction_node_class::create_hcp_animation_link 000 : !animation_timeline_tracks_widget->timeline_track_group_exists(node_id)\n";
        int index = animation_timeline_tracks_widget->add_timeline_group(label, node_id, hcp_animation_object, TIMELINE_OBJECT_DATA_TYPE_ID_VOXEL_INTERACTION);
        if (index != INVALID_ID && index < animation_timeline_tracks_widget->timeline_track_groups.size()) {
            
            timeline_interval_track_id = animation_timeline_tracks_widget->timeline_track_groups[index].add_interval_track("hcp voxel\nInteraction");
            if (timeline_interval_track_id == UINT_MAX) {
                animation_timeline_tracks_widget->delete_timeline_group(node_id);
                return false;
            }
        }
    }
    // =====================================================

    return true;
}

void hcp_voxel_interaction_node_class::delete_hcp_interaction_timeline_link(){
    animation_timeline_tracks_widget->delete_timeline_group(node_id);
    delete hcp_animation_object;
    hcp_animation_object = nullptr;
    timeline_interval_track_id = UINT_MAX;

    // If have more tha one track in a group
    // : delete track hcp_animation_object and initialise variables to unassigned values here
}
// +++++++++++++++++++

void hcp_voxel_interaction_node_class::editor_menu_options() {
    std::string menu_text = "timeline###anvfs" + std::to_string(node_id);
    if (ImGui::BeginMenu(menu_text.c_str())) {
        menu_text = "Add timeline track###anvfs" + std::to_string(node_id);
        if (ImGui::MenuItem(menu_text.c_str())) {
            if (hcp_animation_object == nullptr)
                create_hcp_interaction_timeline_link();
        }

        menu_text = "Delete timeline track###nvfs" + std::to_string(node_id);
        if (ImGui::MenuItem(menu_text.c_str())) {
            delete_hcp_interaction_timeline_link();
        }

        // When more than one track in a group uncomment the following
        //menu_text = "Delete animation track###anvfs" + std::to_string(node_id);
        //if (ImGui::MenuItem(menu_text.c_str())) {
        //    animation_timeline_tracks_widget->delete_timeline_group_track(node_id, timeline_interval_track_id);
        //}

      ImGui::EndMenu();
 }
}

bool hcp_voxel_interaction_node_class::define_ui() {
//std::cout << "hcp_voxel_interaction_node_class::display_ui\n";

    globalc::set_current_selected_data_context_id(this->ui_node_type.node_data_context);
    globalc::set_current_selected_entity_id(INVALID_ID);
    globalc::set_current_selected_entity_category_type_id(INVALID_ID);
    globalc::set_current_selected_entity_type_id(INVALID_ID);

    globalc::set_current_selected_node(this);

    return true;
}

void hcp_voxel_interaction_node_class::display_ui(node_id_type current_selected_object_id) {
//std::cout << "hcp_voxel_interaction_node_class::display_ui\n";

    // Best to perform this code block only when connecting or diconnecting node input link
    if (!vw_scene ) {
        // error message
        return;
    }

    display_interaction_ui(current_selected_object_id);
}

void hcp_voxel_interaction_node_class::display_interaction_ui(id_type current_selected_object_id) {
       float x_pos = 10.0f, y_pos = 70.0f;

        text("HCP Voxel Interactions", x_pos + 70, y_pos);

        y_pos += 30;
        ImGui::SetCursorPosX(x_pos + 70);
        ImGui::SetCursorPosY(y_pos);
        (ImGui::RadioButton("None###hcpvitsn", &interaction_type, VOXEL_INTERACTIONS_NONE));

        y_pos += 30;
        text("Superposition", x_pos + 90, y_pos);

        y_pos += 30;
        ImGui::SetCursorPosX(x_pos + 70);
        ImGui::SetCursorPosY(y_pos);
        (ImGui::RadioButton("Addition###hcpvitsa", &interaction_type, VOXEL_INTERACTIONS_SUPERPOSITION_ADD));

        y_pos += 30;
        ImGui::SetCursorPosX(x_pos + 70);
        ImGui::SetCursorPosY(y_pos);
        (ImGui::RadioButton("Subtraction###hcpvitss", &interaction_type, VOXEL_INTERACTIONS_SUPERPOSITION_SUB));

        y_pos += 30;
        text("Logical", x_pos + 120, y_pos);

        y_pos += 30;
        ImGui::SetCursorPosX(x_pos + 70);
        ImGui::SetCursorPosY(y_pos);
        (ImGui::RadioButton("And###hcpvitsi", &interaction_type, VOXEL_INTERACTIONS_AND));

        y_pos += 30;
        ImGui::SetCursorPosX(x_pos + 70);
        ImGui::SetCursorPosY(y_pos);
        (ImGui::RadioButton("Or###hcpvitso", &interaction_type, VOXEL_INTERACTIONS_OR));

        y_pos += 30;
        ImGui::SetCursorPosX(x_pos + 70);
        ImGui::SetCursorPosY(y_pos);
        (ImGui::RadioButton("Exclusive Or###hcpvitse", &interaction_type, VOXEL_INTERACTIONS_EXOR));

        y_pos += 30;
        ImGui::SetCursorPosX(x_pos + 70);
        ImGui::SetCursorPosY(y_pos);
        (ImGui::RadioButton("Complement###hcpvitsc", &interaction_type, VOXEL_INTERACTIONS_COMPLEMENT));

        y_pos += 30;
        if (ex_button("Perform interaction###cpvitsp", x_pos + 100, y_pos,150, 20))
            action();
}

void hcp_voxel_interaction_node_class::action() {
//std::cout << "hcp_voxel_interaction_node_class:action 0000 : " << interaction_type << std::endl;
        if (interaction_type < 1 || interaction_type >6) return;
        if(!vw_scene) return;

        voxel_interaction.scene_manager = vw_scene; // ****** future enhancement to have this function as a static
        voxel_interaction.log_panel     = log_panel;     // ****** future enhancement to have this function as a static

//std::cout << "hcp_voxel_interaction_node_class:action 1111 : " << interaction_type << std::endl;
        voxel_hcp_object_class* voxel_hcp_object_A = get_voxel_hcp_object_to_execute(0);
        voxel_hcp_object_class* voxel_hcp_object_B = get_voxel_hcp_object_to_execute(1);

        if(voxel_hcp_object_A == NULL || voxel_hcp_object_B == NULL) return;

//std::cout << "hcp_voxel_interaction_node_class:action 2222 A: " << voxel_hcp_object_A->voxel_object_data.voxel_size << " : " << voxel_hcp_object_B->voxel_object_data.voxel_size << std::endl;

        // Voxel matrices must be of the same size
        if (voxel_hcp_object_A->voxel_object_data.voxel_size != voxel_hcp_object_B->voxel_object_data.voxel_size) {
            // error message
            return;
        }

//std::cout << "hcp_voxel_interaction_node_class:action 3333\n";
        if(interaction_object){
            vw_scene->scene_entities_manager.delete_object(interaction_object->id, interaction_object->object_category_id);
        }

        interaction_object = voxel_interaction.create_interaction_object(voxel_hcp_object_A, voxel_hcp_object_B); // ****** future enhancement to have this function as a static

        if (!interaction_object) { 
            node_entity_id          = INVALID_ID;
            node_entity_category_id = INVALID_ID;
            return;
        }

        node_entity_id          = interaction_object->id;
        node_entity_category_id = interaction_object->object_category_id;
        node_entity_type_id     = interaction_object->object_type_id;
        interaction_object->name = "HCP_VOXEL_" + std::to_string(interaction_object->id);
//std::cout << "hcp_voxel_interaction_node_class:action 4444 : " <<interaction_object->id << std::endl;

        if(!perform_interaction(voxel_hcp_object_A, voxel_hcp_object_B, interaction_type)) return;

//std::cout << "hcp_voxel_interaction_node_class:action 5555\n";
        voxel_interaction.update_voxel_verticies(interaction_object); // ****** future enhancement to have this function as a static
//std::cout << "hcp_voxel_interaction_node_class:action 6666\n";

}

bool hcp_voxel_interaction_node_class::perform_interaction(voxel_hcp_object_class* voxel_hcp_object_A, voxel_hcp_object_class* voxel_hcp_object_B, int interaction_type) {
//printf("hcp_interaction_node_class:interaction_and 0000 \n");
    glm::ivec3 a_start, b_start;

    voxel_interaction.deterime_interaction_matrix_start_values(voxel_hcp_object_A, voxel_hcp_object_B, interaction_object, a_start, b_start);
//printf("hcp_interaction_node_class:interaction_and 11111 %i : %i : %i\n", a_start.x, a_start.y, a_start.z);
//printf("hcp_interaction_node_class:interaction_and 22222 %i : %i : %i\n", b_start.x, b_start.y, b_start.z);
//printf("hcp_interaction_node_class:interaction_and 33333 %f : %f : % f\n", interaction_object->voxel_object_data.matrix_origin.x, interaction_object->voxel_object_data.matrix_origin.y, interaction_object->voxel_object_data.matrix_origin.z);


    // Assign interaction values to interaction matrix
    int i_x_dim = interaction_object->voxel_object_data.matrix_dimension.x;
    int i_y_dim = interaction_object->voxel_object_data.matrix_dimension.y;
    int i_z_dim = interaction_object->voxel_object_data.matrix_dimension.z;

//printf("hcp_interaction_node_class:interaction_and 44444 %i : %i : % i\n",i_x_dim, i_y_dim, i_z_dim);
    for (int xi = 0; xi < i_x_dim; xi++) {
        for (int yi = 0; yi < i_y_dim; yi++) {
            for (int zi = 0; zi < i_z_dim; zi++) {

//printf("hcp_interaction_node_class:interaction_and 4444 %\n");
                // Get coresponding voxel data entry in voxel matrix A that is at interaction matrix index coord xi,yi,zi
                voxel_data_type a_value = DEFAULT_INACTIVE_VALUE, b_value = DEFAULT_INACTIVE_VALUE;
                glm::ivec3 matrix_coord_A = { a_start.x + xi,a_start.y + yi ,a_start.z + zi };
                index_data_type a_index = voxel_hcp_object_A->voxel_object_data.get_voxel_matrix_data_index(matrix_coord_A);
                if (a_index >= 0 && a_index < voxel_hcp_object_A->voxel_object_data.voxel_matrix_data.size()) {
                    a_value = voxel_hcp_object_A->voxel_object_data.voxel_matrix_data[a_index];
                }

                // Get coresponding voxel data entry in voxel matrix A that is at interaction matrix index coord xi,yi,zi
                glm::ivec3 matrix_coord_B = { b_start.x + xi,b_start.y + yi ,b_start.z + zi };
                index_data_type b_index = voxel_hcp_object_B->voxel_object_data.get_voxel_matrix_data_index(matrix_coord_B);
                if (b_index >= 0 && b_index < voxel_hcp_object_B->voxel_object_data.voxel_matrix_data.size()) {
                    b_value = voxel_hcp_object_B->voxel_object_data.voxel_matrix_data[b_index];
                }

                switch (interaction_type) {
                    case VOXEL_INTERACTIONS_SUPERPOSITION_ADD: assign_voxel(xi, yi, zi, a_value + b_value);break;
                    case VOXEL_INTERACTIONS_SUPERPOSITION_SUB: assign_voxel(xi, yi, zi, abs(a_value - b_value)); break;
                    case VOXEL_INTERACTIONS_AND              : {if (a_value != DEFAULT_INACTIVE_VALUE && b_value != DEFAULT_INACTIVE_VALUE) 
                                                                    assign_voxel(xi, yi, zi, DEFAULT_ACTIVE_VALUE);
                                                                    break;}
                    case VOXEL_INTERACTIONS_OR               : {if (a_value != DEFAULT_INACTIVE_VALUE || b_value != DEFAULT_INACTIVE_VALUE) 
                                                                    assign_voxel(xi, yi, zi, DEFAULT_ACTIVE_VALUE);
                                                                    break;}
                    case VOXEL_INTERACTIONS_EXOR             : {if ((a_value != DEFAULT_INACTIVE_VALUE) != (b_value != DEFAULT_INACTIVE_VALUE)) 
                                                                    assign_voxel(xi, yi, zi, DEFAULT_ACTIVE_VALUE);
                                                                    break;}
                    case VOXEL_INTERACTIONS_COMPLEMENT       : {if (!((a_value != DEFAULT_INACTIVE_VALUE) == (b_value != DEFAULT_INACTIVE_VALUE))) 
                                                                    assign_voxel(xi, yi, zi, DEFAULT_ACTIVE_VALUE);
                                                                    break; }
                }
            }
        }
    }

    return true;
}

void hcp_voxel_interaction_node_class::assign_voxel(int xi, int yi, int zi,int value) {
    glm::ivec3 matrix_coord_C = { xi,yi ,zi };
    index_data_type c_index = interaction_object->voxel_object_data.get_voxel_matrix_data_index(matrix_coord_C);
    // Future consideration is that this value is assigned to some other value
    if (c_index >= 0 && c_index < interaction_object->voxel_object_data.voxel_matrix_data.size()) {
        interaction_object->voxel_object_data.voxel_matrix_data[c_index] = value;
    }
}


void hcp_voxel_interaction_node_class::export_hcp_interaction_node_parameter_data(std::fstream& stream) {
    stream << DATA_BLOCK_START << std::endl;

    stream << interaction_type << std::endl;
        
    stream << DATA_BLOCK_END << std::endl;
}

bool hcp_voxel_interaction_node_class::import_node(std::vector<std::string> lines, int& line_number) {
       std::string line;
    line_number++;

	line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "hcp_voxel_interaction_node_class::import_node AAAAAAA : " << line_number << ":" << lines[line_number] << std::endl;
// next line read must be a RULE_BLOCK_START flag
	if (!FW::stringtools::contains(lines[line_number], DATA_BLOCK_START)) {
//std::cout << "hcp_voxel_translation_node_class", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\nMissing RULE_BLOCK_START flag", //QMessageBox::Ok);
		return false;
	}

//std::cout << "import_export_byte_automata_rules_class::read_automata_byte_rule CCCCCC : " << line_number << ":" << lines[line_number] << std::endl;
    line_number++; line = lines[line_number];interaction_type = stoi(line);

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



voxel_hcp_object_class *hcp_voxel_interaction_node_class::get_voxel_hcp_object_to_execute(int input_pin_index){
    if (!vw_scene || input_pin_index <0 || input_pin_index > inputs.size()) {
        // error message
        return nullptr;
    }

    voxel_hcp_object_class* voxel_hcp_object_to_execute = nullptr;
    // find link to input pin
    link_struct_type input_link = graph->links.find_link_with_end_pin(inputs[input_pin_index]);// Only have one input pin which is defined as default

    if (input_link.link_id != INVALID_ID) {
        pin_struct_type link_from_pin = graph->pins.find_pin(input_link.from);

        if (link_from_pin.node_id != INVALID_ID) {
            node_basis_class* node = graph->nodes.get_node(link_from_pin.node_id);

            if (node) {
                 voxel_hcp_object_to_execute = (voxel_hcp_object_class*) node->get_node_data();
            }
        }
    }

    return voxel_hcp_object_to_execute;
}
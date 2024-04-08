#include "interaction_node.h"


#include <Universal_FW/ImNodes/Kernal/node_editor.h>>
#include <Universal_FW/Timeline/timeline_tracks.h>

#include <VW/Editor/Node_Editor/node_editor_extras.h>
#include <VW/Editor/Import_Export/vw_import_export_parameters.h>

//#include <Source/Editor/Main_Window/Widgets/imgui_widgets.h>
// ---  template to avoid circular referencing ------
// --- timeline animation class placed here that ----
// --- includes the node class and its functions ----
// --- to execute defined in the node class .h file -

#include "Animation/hex_surface_interaction_animation.h"
hex_surface_interaction_animation_class *hex_animation_object = nullptr;
// -----------------

bool hex_surface_interaction_node_class::define_node(ImVec2 click_pos,id_type  entity_id) {
    node_dimensions = { 60.0f,5.0f };

    // !!! CRITICAL : MUST HAVE NODE UI NODE TYPE DEFINITION DEFINED
    // !!! ELSE CANNOT LINK NODE PINS, PERFORM EVALUATIONS OR DISPLAY
    // !!! NODE PARAMETERS TO BE DISPLAYED AND MODIFIED AS WELL AS
    // !!! OTHER NODE AND NODE EDITOR FUCTIONALITY

    ui_node_type.category          = NODE_CATEGORY_ID_OPERATOR;
    ui_node_type.node_type         = NODE_TYPE_ID_HEX_SURFACE_INTERACTION;
    ui_node_type.node_data_context = NODE_DATA_CONTEXT;

    label = "Hex Interaction " + std::to_string(node_id);

    node_draw_data.node_header_color          = ImColor(110, 155, 10, 255);
    node_draw_data.node_header_hovered_color  = ImColor(155, 180, 90, 255);
    node_draw_data.node_header_selected_color = ImColor(145, 165, 130, 255);
    node_draw_data.node_header_label_color    = ImColor(255, 255, 255, 255);// ImGui does not have a color function that accepts its own ImColor type for textS

    node_draw_data.node_body_color          = NODE_HEX_DRAW_BODY_COLOR         ;
    node_draw_data.node_body_hovered_color  = NODE_HEX_DRAW_BODY_HOVERED_COLOR ;
    node_draw_data.node_body_selected_color = NODE_HEX_DRAW_BODY_SELECTED_COLOR;
    node_draw_data.node_outline_color       = NODE_HEX_DRAW_OUTLINE_COLOR      ;

    graph->node_editor->SetNodeScreenSpacePos(node_id, click_pos);
   
    return true;
}

bool hex_surface_interaction_node_class::define_import_node(ImVec2 click_pos, node_id_type entity_id_, void* node_misc_data) {
    node_misc_import_struct_type* node_misc_import_data = static_cast<node_misc_import_struct_type*>(node_misc_data);

    if (!node_misc_import_data) {
        std::cout << "ERROR : Editor scene and log data not defined. Aborting import of Hex surface Interaction node.\n";
        return false;
    }

    if (node_misc_import_data->log_panel == nullptr) {
        std::cout << "WARNING : Imported Hex surface Interaction node has no defined log panel to output messages to.\n";
    }

    if (node_misc_import_data->vw_scene == nullptr) {
        std::cout << "ERROR : Imported Hex surface Interaction node has no defined scene manager. Aborted import.\n" << std::endl;
        if (log_panel) log_panel->application_log.AddLog("ERROR :Imported Hex surface Interaction node has no defined scene manager. Aborted import.\n");
        return false;
    }

    log_panel = node_misc_import_data->log_panel;
    vw_scene = node_misc_import_data->vw_scene;

    return define_node(click_pos, entity_id_);
}

    bool hex_surface_interaction_node_class::define_inputs(pins_class& pins) {
        pin_struct_type pin;

        pin.label     = "Grid A";
        pin.context   = CONTEXT_ID_HEX_SURFACE_GRID;
        pin.data_type = DATA_VALUE_TYPE_HEX_SURFACE_GRID;
        pin.pin_type  = pin_type_enum::single_input;

        pin.pin_color          = IM_COL32(120, 30, 30, 255);
        pin.pin_hovered_color  = IM_COL32(200, 85, 85, 255);
        pin.pin_selected_color = IM_COL32(200, 120, 120, 255);
        pin.pin_shape          = ImNodesPinShape_::ImNodesPinShape_QuadFilled;

        if (add_node_input_pin(pin, pins) == INVALID_ID) return false;

        pin.label     = "Grid B";
        pin.context   = CONTEXT_ID_HEX_SURFACE_GRID;
        pin.data_type = DATA_VALUE_TYPE_HEX_SURFACE_GRID;
        pin.pin_type  = pin_type_enum::single_input;

        pin.pin_color          = IM_COL32(120, 30, 30, 255);
        pin.pin_hovered_color  = IM_COL32(200, 85, 85, 255);
        pin.pin_selected_color = IM_COL32(200, 120, 120, 255);
        pin.pin_shape          = ImNodesPinShape_::ImNodesPinShape_QuadFilled;

        if (add_node_input_pin(pin, pins) == INVALID_ID) return false;

        return true;
    }

    bool hex_surface_interaction_node_class::define_outputs(pins_class& pins) {
        pin_struct_type pin; 

        pin.label     = "Grid";
        pin.context   = CONTEXT_ID_HEX_SURFACE_GRID;
        pin.data_type = DATA_VALUE_TYPE_HEX_SURFACE_GRID;
        pin.pin_type  = pin_type_enum::single_output;

        pin.pin_color          = IM_COL32(120, 30, 30, 255);
        pin.pin_hovered_color  = IM_COL32(200, 85, 85, 255);
        pin.pin_selected_color = IM_COL32(200, 120, 120, 255);
        pin.pin_shape          = ImNodesPinShape_::ImNodesPinShape_QuadFilled;

        if (add_node_output_pin(pin, pins) == INVALID_ID) return false;

        return true;
    }

    void hex_surface_interaction_node_class::delete_node_entity() {
        delete_hex_surface_interaction_timeline_link();
        delete_hex_grid();
    }

    void hex_surface_interaction_node_class::delete_hex_grid() { // So as to be able to delete interaction hex data without deleting the node
        if (!interaction_object || !vw_scene) return;
        if (node_entity_id != INVALID_ID && node_entity_category_id != INVALID_ID) {
            vw_scene->scene_entities_manager.delete_object(node_entity_id, node_entity_category_id);
            node_entity_id          = INVALID_ID;
            node_entity_category_id = INVALID_ID;
            interaction_object      = nullptr;
        }
    }

// +++++++++++++++++++
bool hex_surface_interaction_node_class::create_hex_surface_interaction_timeline_link(){
    hex_animation_object = new hex_surface_interaction_animation_class;

    if (!hex_animation_object) return false;

    hex_animation_object->hex_surface_interaction_node = this;

    // ================ define as a create timeline group function ===============
    if (!animation_timeline_tracks_widget->timeline_track_group_exists(node_id)) {
//std::cout << "hcp_voxel_node_class::create_hcp_animation_link 000 : !animation_timeline_tracks_widget->timeline_track_group_exists(node_id)\n";
        int index = animation_timeline_tracks_widget->add_timeline_group(label, node_id, hex_animation_object, TIMELINE_OBJECT_DATA_TYPE_ID_HEX_SURFACE_INTERACTION);
        if (index != INVALID_ID && index < animation_timeline_tracks_widget->timeline_track_groups.size()) {
            
            timeline_interval_track_id = animation_timeline_tracks_widget->timeline_track_groups[index].add_interval_track("hex surface\nInteraction");
            if (timeline_interval_track_id == UINT_MAX) {
                animation_timeline_tracks_widget->delete_timeline_group(node_id);
                return false;
            }
        }
    }
    // =====================================================

    return true;

    return false;
}

void hex_surface_interaction_node_class::delete_hex_surface_interaction_timeline_link(){
    animation_timeline_tracks_widget->delete_timeline_group(node_id);
    delete hex_animation_object;
    hex_animation_object = nullptr;
    timeline_interval_track_id = UINT_MAX;

    // If have more tha one trck in a group
    // : delete track hcp_animation_object and initialise variables to unassigned values here
}
// +++++++++++++++++++

void hex_surface_interaction_node_class::editor_menu_options() {
    std::string menu_text = "timeline###ahvfs" + std::to_string(node_id);
    if (ImGui::BeginMenu(menu_text.c_str())) {
        menu_text = "Add timeline track###ahvfs" + std::to_string(node_id);
        if (ImGui::MenuItem(menu_text.c_str())) {
            if (hex_animation_object == nullptr)
                create_hex_surface_interaction_timeline_link();
        }

        menu_text = "Delete timeline track###nhfs" + std::to_string(node_id);
        if (ImGui::MenuItem(menu_text.c_str())) {
            delete_hex_surface_interaction_timeline_link();
        }

        // When more than one track in a group uncomment the following
        //menu_text = "Delete animation track###anvfs" + std::to_string(node_id);
        //if (ImGui::MenuItem(menu_text.c_str())) {
        //    animation_timeline_tracks_widget->delete_timeline_group_track(node_id, timeline_interval_track_id);
        //}

      ImGui::EndMenu();
 }
}

bool hex_surface_interaction_node_class::define_ui() {
//std::cout << "hcp_voxel_interaction_node_class::display_ui\n";

    globalc::set_current_selected_data_context_id(this->ui_node_type.node_data_context);
    globalc::set_current_selected_entity_id(INVALID_ID);
    globalc::set_current_selected_entity_category_type_id(INVALID_ID);
    globalc::set_current_selected_entity_type_id(INVALID_ID);

    globalc::set_current_selected_node(this);

    return true;
}

void hex_surface_interaction_node_class::display_ui(node_id_type current_selected_object_id) {
//std::cout << "hcp_voxel_interaction_node_class::display_ui\n";

    // Best to perform this code block only when connecting or diconnecting node input link
    if (!vw_scene ) {
        // error message
        return;
    }

    display_interaction_ui(current_selected_object_id);
}

void hex_surface_interaction_node_class::display_interaction_ui(id_type current_selected_object_id) {
    float x_pos = 10.0f, y_pos = 70.0f;

    text("HCP Voxel Interactions", x_pos + 70, y_pos);

    y_pos += 30;
    ImGui::SetCursorPosX(x_pos + 70);
    ImGui::SetCursorPosY(y_pos);
    (ImGui::RadioButton("None###hcpvitsn", &interaction_type, HEX_SURFACE_INTERACTIONS_NONE));

    y_pos += 30;
    text("Superposition", x_pos + 90, y_pos);

    y_pos += 30;
    ImGui::SetCursorPosX(x_pos + 70);
    ImGui::SetCursorPosY(y_pos);
    (ImGui::RadioButton("Addition###hcpvitsa", &interaction_type, HEX_SURFACE_INTERACTIONS_SUPERPOSITION_ADD));

    y_pos += 30;
    ImGui::SetCursorPosX(x_pos + 70);
    ImGui::SetCursorPosY(y_pos);
    (ImGui::RadioButton("Subtraction###hcpvitss", &interaction_type, HEX_SURFACE_INTERACTIONS_SUPERPOSITION_SUB));

    y_pos += 30;
    text("Logical", x_pos + 120, y_pos);

    y_pos += 30;
    ImGui::SetCursorPosX(x_pos + 70);
    ImGui::SetCursorPosY(y_pos);
    (ImGui::RadioButton("And###hcpvitsi", &interaction_type, HEX_SURFACE_INTERACTIONS_AND));

    y_pos += 30;
    ImGui::SetCursorPosX(x_pos + 70);
    ImGui::SetCursorPosY(y_pos);
    (ImGui::RadioButton("Or###hcpvitso", &interaction_type, HEX_SURFACE_INTERACTIONS_OR));

    y_pos += 30;
    ImGui::SetCursorPosX(x_pos + 70);
    ImGui::SetCursorPosY(y_pos);
    (ImGui::RadioButton("Exclusive Or###hcpvitse", &interaction_type, HEX_SURFACE_INTERACTIONS_EXOR));

    y_pos += 30;
    ImGui::SetCursorPosX(x_pos + 70);
    ImGui::SetCursorPosY(y_pos);
    (ImGui::RadioButton("Complement###hcpvitsc", &interaction_type, HEX_SURFACE_INTERACTIONS_COMPLEMENT));

    y_pos += 30;
    if (ex_button("Perform interaction###cpvitsp", x_pos + 100, y_pos,150, 20))
        action();

}

void hex_surface_interaction_node_class::action() {
    if (interaction_type < 1 || interaction_type >6) return;
    if (!vw_scene) return;

    hex_surface_interaction.scene_manager = vw_scene; // ****** future enhancement to have this function as a static
    hex_surface_interaction.log_panel = log_panel;     // ****** future enhancement to have this function as a static

    hex_surface_object_class *hex_surface_object_A = get_hex_surface_object_to_execute(0);
    hex_surface_object_class *hex_surface_object_B = get_hex_surface_object_to_execute(1);

    if (!hex_surface_object_A || !hex_surface_object_B) {
        // error message
        return;
    }

    // Voxel matrices must be of the same size
    if (hex_surface_object_A->hex_surface_object_data.hex_size != hex_surface_object_B->hex_surface_object_data.hex_size) {
        // error message 
        return;
    }

//printf("hcp_interaction_node_class:action 0000 \n");
    interaction_object = hex_surface_interaction.create_interaction_object(hex_surface_object_A, hex_surface_object_B); // ****** future enhancement to have this function as a static
    if (!interaction_object) {
        // error message
        node_entity_id          = INVALID_ID;
        node_entity_category_id = INVALID_ID;
        return;
    }
//std::cout << "hcp_interaction_node_class:action 1111 : " << interaction_object->hex_surface_object_data.grid_dimension.x << " : " << interaction_object->hex_surface_object_data.grid_dimension.y << std::endl;
    node_entity_id          = interaction_object->id;
    node_entity_category_id = interaction_object->object_category_id;
    node_entity_type_id     = interaction_object->object_type_id;
    interaction_object->name = "HEX_SURFACE_" + std::to_string(interaction_object->id);

    if(!peform_interaction(hex_surface_object_A, hex_surface_object_B, interaction_type)) return;

//printf("hcp_interaction_node_class action 3333\n");
    hex_surface_interaction.update_voxel_verticies(interaction_object); // ****** future enhancement to have this function as a static
//printf("hcp_interaction_node_class action 4444\n");
}

bool hex_surface_interaction_node_class::peform_interaction(hex_surface_object_class* hex_surface_object_A, hex_surface_object_class* hex_surface_object_B, int interaction_type) {

//printf("hcp_interaction_node_class:peform_interaction 0000 \n");
    glm::ivec2 a_start, b_start;

    hex_surface_interaction.deterime_interaction_matrix_start_values(hex_surface_object_A, hex_surface_object_B, interaction_object, a_start, b_start);
//printf("hcp_interaction_node_class:peform_interaction 11111 %i : %i : %i\n", a_start.x, a_start.y);
//printf("hcp_interaction_node_class:interaction_and 22222 %i : %i : %i\n", b_start.x, b_start.y, b_start.z);
//printf("hcp_interaction_node_class:interaction_and 33333 %f : %f : % f\n", interaction_object->hex_surface_object_data.matrix_origin.x, interaction_object->hex_surface_object_data.matrix_origin.y, interaction_object->hex_surface_object_data.matrix_origin.z);


    // Assign interaction values to interaction matrix
    int i_x_dim = interaction_object->hex_surface_object_data.grid_dimension.x;
    int i_y_dim = interaction_object->hex_surface_object_data.grid_dimension.y;

//printf("hcp_interaction_node_class:peform_interaction 44444 %i : %i \n",i_x_dim, i_y_dim);
    for (int xi = 0; xi < i_x_dim; xi++) {
        for (int yi = 0; yi < i_y_dim; yi++) {         
//printf("hcp_interaction_node_class:peform_interaction 5555 %\n");
            // Get coresponding voxel data entry in voxel matrix A that is at interaction matrix index coord xi,yi,zi
            hex_surface_data_type a_value = DEFAULT_INACTIVE_VALUE, b_value = DEFAULT_INACTIVE_VALUE;
            hex_index_vector matrix_coord_A = { a_start.x + xi,a_start.y + yi };
            index_data_type a_index = hex_surface_object_A->hex_surface_object_data.get_hex_surface_matrix_data_index(matrix_coord_A);
            if (a_index >= 0 && a_index < hex_surface_object_A->hex_surface_object_data.hex_surface_matrix_data.size()) {
                a_value = hex_surface_object_A->hex_surface_object_data.hex_surface_matrix_data[a_index];
            }

            // Get coresponding voxel data entry in voxel matrix A that is at interaction matrix index coord xi,yi,zi
            hex_index_vector matrix_coord_B = { b_start.x + xi,b_start.y + yi  };
            index_data_type b_index = hex_surface_object_B->hex_surface_object_data.get_hex_surface_matrix_data_index(matrix_coord_B);
            if (b_index >= 0 && b_index < hex_surface_object_B->hex_surface_object_data.hex_surface_matrix_data.size()) {
                b_value = hex_surface_object_B->hex_surface_object_data.hex_surface_matrix_data[b_index];
            }
//printf("hcp_interaction_node_class:peform_interaction 66666 %\n");
            switch (interaction_type) {
                case HEX_SURFACE_INTERACTIONS_SUPERPOSITION_ADD: assign_voxel(xi, yi, a_value + b_value);break;
                case HEX_SURFACE_INTERACTIONS_SUPERPOSITION_SUB: assign_voxel(xi, yi, abs(a_value - b_value)); break;
                case HEX_SURFACE_INTERACTIONS_AND              : {if (a_value != DEFAULT_INACTIVE_VALUE && b_value != DEFAULT_INACTIVE_VALUE) 
                                                                assign_voxel(xi, yi, DEFAULT_ACTIVE_VALUE);
                                                                break;}
                case HEX_SURFACE_INTERACTIONS_OR               : {if (a_value != DEFAULT_INACTIVE_VALUE || b_value != DEFAULT_INACTIVE_VALUE) 
                                                                assign_voxel(xi, yi, DEFAULT_ACTIVE_VALUE);
                                                                break;}
                case HEX_SURFACE_INTERACTIONS_EXOR             : {if ((a_value != DEFAULT_INACTIVE_VALUE) != (b_value != DEFAULT_INACTIVE_VALUE)) 
                                                                assign_voxel(xi, yi, DEFAULT_ACTIVE_VALUE);
                                                                break;}
                case HEX_SURFACE_INTERACTIONS_COMPLEMENT       : {if (!((a_value != DEFAULT_INACTIVE_VALUE) == (b_value != DEFAULT_INACTIVE_VALUE))) 
                                                                assign_voxel(xi, yi, DEFAULT_ACTIVE_VALUE);
                                                                break; }
            }
            }
        }
//printf("hcp_interaction_node_class:peform_interaction 7777 %\n");

    return true;

    //return false;
}

void hex_surface_interaction_node_class::assign_voxel(int xi, int yi, hex_surface_data_type value) {
    hex_index_vector matrix_coord_C = { xi,yi };
    index_data_type c_index = interaction_object->hex_surface_object_data.get_hex_surface_matrix_data_index(matrix_coord_C);
    // Future consideration is that this value is assigned to some other value
    if (c_index >= 0 && c_index < interaction_object->hex_surface_object_data.hex_surface_matrix_data.size()) {
        interaction_object->hex_surface_object_data.hex_surface_matrix_data[c_index] = value;
    }
}


void hex_surface_interaction_node_class::export_hex_surface_interaction_node_parameter_data(std::fstream& stream) {
    stream << DATA_BLOCK_START << std::endl;

    stream << interaction_type << std::endl;
        
    stream << DATA_BLOCK_END << std::endl;
}

bool hex_surface_interaction_node_class::import_node(std::vector<std::string> lines, int& line_number) {
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




hex_surface_object_class *hex_surface_interaction_node_class::get_hex_surface_object_to_execute(int input_pin_index){
    if (!vw_scene) {
        // error message
        return nullptr;
    }

    hex_surface_object_class* hex_surface_object_to_execute = nullptr;
    // find link to input pin
    link_struct_type input_link = graph->links.find_link_with_end_pin(inputs[input_pin_index]);// Only have one input pin which is defined as default

    if (input_link.link_id != INVALID_ID) {
        pin_struct_type link_from_pin = graph->pins.find_pin(input_link.from);

        if (link_from_pin.node_id != INVALID_ID) {
            node_basis_class* node = graph->nodes.get_node(link_from_pin.node_id);

            if (node) {
                 hex_surface_object_to_execute = (hex_surface_object_class*) node->get_node_data();
            }
        }
    }

    return hex_surface_object_to_execute;
}
#include "hex_generation_selection_widget.h"

#include <imgui-docking/imgui.h>

#include <VW_framework/Types/vw_type_definitions.h>
#include <FrameWork/Kernels/ImGuiEx/imgui_widgets.h>

	void hex_surface_generation_selection_widget_class::display() {
		if (hex_surface_object_to_execute == NULL) {
			return;
		}

		float x_pos = 10.0f, y_pos = 70.0f;

		text("Hex Surface Generation", x_pos+60, y_pos);
		
		y_pos += 25;
		text(" Matrix\nFunction", x_pos + 50, y_pos);
		text("Cartesian\nCoordinate", x_pos + 170, y_pos);
		y_pos += 40;

		ImGui::SetCursorPosX(x_pos + 70);
		ImGui::SetCursorPosY(y_pos);
		if (ImGui::RadioButton("###hexvgtm", &hcp_generation_type, 0)) {
			display_cart_to_hex_widget(hex_surface_object_to_execute, current_selected_entity_id);
		}

		ImGui::SetCursorPosX(x_pos + 190);
		ImGui::SetCursorPosY(y_pos);
		if(ImGui::RadioButton("###hexvgtc", &hcp_generation_type, 1)) {
			display_hex_surface_generation_widget(hex_surface_object_to_execute, current_selected_entity_id);
		}

		y_pos += 30;
		ImGui::SetCursorPosX(x_pos + 120);
		ImGui::SetCursorPosY(y_pos);
		if (ImGui::Checkbox("Display as points###hgdisplpts", &display_as_points)) {
			change_hex_display();
		}

		y_pos += 30;
		text("Hex Scale", x_pos + 130, y_pos);

		y_pos += 20;

		if (float_min_max_slider("v", x_pos, y_pos, 200.0f, min_vscale, max_vscale, hex_scale_value, 50.0f)) {
			update_hex_size();
		}

		if(hcp_generation_type == 0) 
			display_hex_surface_generation_widget(hex_surface_object_to_execute, current_selected_entity_id);
		else
			display_cart_to_hex_widget(hex_surface_object_to_execute, current_selected_entity_id);
	}

	void hex_surface_generation_selection_widget_class::change_hex_display() {
		if (hex_surface_object_to_execute != NULL) {

			hex_surface_object_to_execute->hex_surface_object_data.hex_surface_shader_parameters.use_point_shader = display_as_points;
			hex_surface_object_to_execute->log_panel = log_panel;

			hex_surface_object_to_execute->update_shader();

			shader_class shader;
			shader.set_b1(hex_surface_object_to_execute->geometry->shader->getProgram(), display_as_points, "display_as_points");

			update_hex_size();
		}
	}

	void hex_surface_generation_selection_widget_class::update_hex_size() {
		if (hex_surface_object_to_execute != NULL) {
			float vox_size = hex_surface_object_to_execute->hex_surface_object_data.hex_size * hex_scale_value;

			shader_class shader;

			if (hex_surface_object_to_execute->geometry == NULL) return;
			if (hex_surface_object_to_execute->geometry->shader == NULL) return;

			shader.set_f1(hex_surface_object_to_execute->geometry->shader->getProgram(), vox_size, "hexSize");
		}
	}

//------------------------------------------------

	void hex_surface_generation_selection_widget_class::display_cart_to_hex_widget(hex_surface_object_class *hex_surface_object_to_execute, id_type current_selected_object_id) {
		cart_to_hex_widget.hex_surface_object_to_execute = hex_surface_object_to_execute;

		if (cart_to_hex_widget.hex_surface_object_to_execute == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hex suface object defined to perform hex generation procedure on.\n");
			return;
		}
//printf("voxel_hcp_generation_widget.voxel_hcp_object_to_execute != NULL ^^^^\n");
		cart_to_hex_widget.log_panel                  = log_panel;
		cart_to_hex_widget.current_selected_entity_id = current_selected_object_id;
//printf("parameter_panel_class :: display_voxel_hcp_generation_widget22\n");
		cart_to_hex_widget.display();

	}

	void hex_surface_generation_selection_widget_class::display_hex_surface_generation_widget(hex_surface_object_class *hex_surface_object_to_execute, id_type current_selected_object_id) {

		hex_surface_generation_widget.hex_surface_object_to_execute = hex_surface_object_to_execute;

		if (hex_surface_generation_widget.hex_surface_object_to_execute == NULL) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hex suface object defined to perform hex generation procedure on.\n");
			return;
		}

		hex_surface_generation_widget.log_panel                  = log_panel;
		hex_surface_generation_widget.current_selected_entity_id = current_selected_object_id;
		hex_surface_generation_widget.display();
	}
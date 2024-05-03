#include "hex_surface_generation_editor_widget.h"

#include <FrameWork/Kernels/imgui-docking/imgui.h>

#include <VW_FrameWork/Types/vw_type_definitions.h>

#include <FrameWork/Kernels/ImGuiEx/imgui_widgets.h>

#include <VW/Editor/Main_Window/Panels/log_panel.h>

#include <Universal_FW/Tools/dialogs.h>

#include "../../Object/DataTypes/dt_hex_surface_generator.h"
#include "../../Kernal/hex_surface_function_import_export.h"
#include "../../Object/hex_surface_object.h"

	void hex_surface_generation_editor_widget_class::display() {
		if (hex_surface_object_to_execute == NULL) {
			return;
		}

		float x_pos = 10.0f, y_pos = 70.0f;

		text("Hex Surface Editor Generation", x_pos+60, y_pos);

		y_pos += 30;
		ImGui::SetCursorPosX(x_pos + 120);
		ImGui::SetCursorPosY(y_pos);
		if (ImGui::Checkbox("Display as points###gdisplpts", &display_as_points)) {
			change_voxels_display();
		}

		y_pos += 30;
		text("Hex Scale", x_pos + 130, y_pos);

		y_pos += 20;
		if (float_min_max_slider("h", x_pos, y_pos, 200.0f, min_vscale, max_vscale, hex_scale_value, 50.0f)) {
			hex_surface_generation_widget.voxel_scale_value = hex_scale_value;
			update_hex_size();
		}

		display_hex_surface_generation_widget(hex_surface_object_to_execute, current_selected_entity_id);
	}

	void hex_surface_generation_editor_widget_class::change_voxels_display() {
			if (hex_surface_object_to_execute != NULL) {
				hex_surface_object_to_execute->hex_surface_object_data.hex_surface_shader_parameters.use_point_shader = display_as_points;

//printf("hex_surface_generation_editor_widget_class :: change_voxels_display 000 : %i : %i\n", display_as_points, hex_surface_object_to_execute->hex_surface_object_data.shader_parameters.use_point_shader);
				hex_surface_object_to_execute->log_panel = log_panel;
				
				hex_surface_object_to_execute->update_shader();

				shader_class shader;
				shader.set_b1(hex_surface_object_to_execute->geometry->shader->getProgram(), display_as_points, "display_as_points");
				hex_surface_generation_widget.hex_analysis_shader->update_editor_shader_uniforms();

				update_hex_size();
			}
	}

	void hex_surface_generation_editor_widget_class::update_hex_size() {
		if (hex_surface_object_to_execute != NULL) {
//printf("hex_surface_generation_editor_widget_class :: update_voxel_size 000 : %i : %i\n", display_as_points, scene_voxel_object->scene_graph_object.scene_object_class.shader_material->use_point_shader);

			float vox_size = hex_surface_object_to_execute->hex_surface_object_data.hex_size * hex_scale_value;

			shader_class shader;

			if (hex_surface_object_to_execute->geometry == NULL) return;
			if (hex_surface_object_to_execute->geometry->shader == NULL) return;

			shader.set_f1(hex_surface_object_to_execute->geometry->shader->getProgram(), vox_size, "hexSize");

			//float voxel_hcp_z_increment = hex_surface_object_to_execute->hex_surface_object_data.hex_size * 2.0f * sqrt(6.0f) / 3.0f;
			//shader.set_f1(hex_surface_object_to_execute->geometry->shader->getProgram(), voxel_hcp_z_increment, "voxel_hcp_z_increment");
		}

	}

//-------------------------------------
	void hex_surface_generation_editor_widget_class::display_hex_surface_generation_widget(hex_surface_object_class *hex_surface_object_to_execute, id_type current_selected_object_id) {
		hex_surface_generation_widget.hex_surface_object_to_execute = hex_surface_object_to_execute;

		if (hex_surface_generation_widget.hex_surface_object_to_execute == NULL) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hcp voxel object defined to perform voxel generation procedure on.\n");
			return;
		}
//printf("voxel_hcp_generation_widget.hex_surface_object_to_execute != NULL ^^^^\n");
		hex_surface_generation_widget.log_panel                  = log_panel;
		hex_surface_generation_widget.current_selected_entity_id = current_selected_object_id;
//printf("parameter_panel_class :: display_voxel_hcp_generation_widget22\n");
		hex_surface_generation_widget.display();

	}

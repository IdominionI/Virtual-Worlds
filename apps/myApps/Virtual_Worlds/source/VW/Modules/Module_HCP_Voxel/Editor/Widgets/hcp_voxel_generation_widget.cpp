#include "hcp_voxel_generation_widget.h"

#include <FrameWork/Kernels/imgui-docking/imgui.h>

#include <VW_FrameWork/Types/vw_type_definitions.h>

#include <VW/Editor/Main_Window/Widgets/imgui_widgets.h>
#include <VW/Editor/Tools/dialogs.h>
#include <VW/Editor/Main_Window/Panels/log_panel.h>

#include "../../Object/DataTypes/dt_voxel_generator.h"
#include "../../Kernal/voxel_function_import_export.h"
#include "../../Object/voxel_hcp_object.h"

	void hcp_voxel_generation_widget_class::display() {
		if (voxel_hcp_object_to_execute == NULL) {
			return;
		}

		float x_pos = 10.0f, y_pos = 70.0f;

		text("HCP Voxel Generation", x_pos+60, y_pos);

		y_pos += 30;
		ImGui::SetCursorPosX(x_pos + 120);
		ImGui::SetCursorPosY(y_pos);
		if (ImGui::Checkbox("Display as points###gdisplpts", &display_as_points)) {
			change_voxels_display();
		}

		y_pos += 30;
		text("Voxel Scale", x_pos + 130, y_pos);

		y_pos += 20;
		if (float_min_max_slider("v", x_pos, y_pos, 200.0f, min_vscale, max_vscale, voxel_scale_value, 50.0f)) {
			voxel_hcp_generation_widget.voxel_scale_value = voxel_scale_value;
			update_voxel_size();
		}

		display_voxel_generation_widget(voxel_hcp_object_to_execute, current_selected_entity_id);
	}

	void hcp_voxel_generation_widget_class::change_voxels_display() {
			if (voxel_hcp_object_to_execute != NULL) {
				voxel_hcp_object_to_execute->voxel_object_data.shader_parameters.use_point_shader = display_as_points;

//printf("hcp_voxel_generation_widget_class :: change_voxels_display 000 : %i : %i\n", display_as_points, scene_voxel_object->scene_graph_object.scene_object_class.shader_material->use_point_shader);
				voxel_hcp_object_to_execute->log_panel = log_panel;
				voxel_hcp_object_to_execute->update_shader();

				update_voxel_size();
			}
	}

	void hcp_voxel_generation_widget_class::update_voxel_size() {
		if (voxel_hcp_object_to_execute != NULL) {
//printf("hcp_voxel_generation_widget_class :: update_voxel_size 000 : %i : %i\n", display_as_points, scene_voxel_object->scene_graph_object.scene_object_class.shader_material->use_point_shader);

			float vox_size = voxel_hcp_object_to_execute->voxel_object_data.voxel_size * voxel_scale_value;

			shader_class shader;

			if (voxel_hcp_object_to_execute->geometry == NULL) return;
			if (voxel_hcp_object_to_execute->geometry->shader == NULL) return;

			shader.set_f1(voxel_hcp_object_to_execute->geometry->shader->getProgram(), vox_size, "voxSize");

			float voxel_hcp_z_increment = voxel_hcp_object_to_execute->voxel_object_data.voxel_size * 2.0f * sqrt(6.0f) / 3.0f;
			shader.set_f1(voxel_hcp_object_to_execute->geometry->shader->getProgram(), voxel_hcp_z_increment, "voxel_hcp_z_increment");
		}

	}

//-------------------------------------
	void hcp_voxel_generation_widget_class::display_voxel_generation_widget(voxel_hcp_object_class *voxel_hcp_object_to_execute, id_type current_selected_object_id) {
		voxel_hcp_generation_widget.voxel_hcp_object_to_execute = voxel_hcp_object_to_execute;

		if (voxel_hcp_generation_widget.voxel_hcp_object_to_execute == NULL) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No hcp voxel object defined to perform voxel generation procedure on.\n");
			return;
		}
//printf("voxel_hcp_generation_widget.voxel_hcp_object_to_execute != NULL ^^^^\n");
		voxel_hcp_generation_widget.log_panel                  = log_panel;
		voxel_hcp_generation_widget.current_selected_entity_id = current_selected_object_id;
//printf("parameter_panel_class :: display_voxel_hcp_generation_widget22\n");
		voxel_hcp_generation_widget.display();

	}

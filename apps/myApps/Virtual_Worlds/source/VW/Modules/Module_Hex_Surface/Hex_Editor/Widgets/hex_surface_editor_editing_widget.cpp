#include "hex_surface_editor_editing_widget.h"

#include <FrameWork/Kernels/imgui-docking/imgui.h>

#include <VW_FrameWork/Types/vw_type_definitions.h>
#include <VW_FrameWork/App/global.h>

#include <FrameWork/Kernels/ImGuiEx/imgui_widgets.h>

#include <VW/Editor/Main_Window/Panels/log_panel.h>

#include <Universal_FW/Tools/dialogs.h>

#include "../../Object/DataTypes/dt_hex_surface_generator.h"
#include "../../Kernal/hex_surface_function_import_export.h"

void hex_surface_editor_editing_widget_class::display() {
	if (!hex_surface_object_to_execute) {
		return;
	}

	if(!hex_analysis_shader) {
std::cout << "hex_surface_editor_editing_widget_class : !hex_analysis_shader\n";
		return;
	}

	hex_analysis_shader->current_selected_entity_id = current_selected_entity_id;
	hex_analysis_shader->hex_surface_object_to_execute = hex_surface_object_to_execute;

	x_pos = 10.0f, y_pos = 70.0f;

	text("Hex Editor", x_pos + 90, y_pos);

	y_pos += 30;
	hcp_voxel_analysis();
	
	y_pos += 20;

	checkbox("###hgdec", "Enable\nEdit", x_pos + 98, y_pos, 55.0f,5.0f, enable_edit);

	if (enable_edit) {
		int s_index = hcp_voxel_selection();

		if (brush_changed) {
			define_test_brush();
			brush_changed = false;
		}

		y_pos += 45;
		if (integer_input("Brush Value##hhgbv", test_brush.hex_brush.brush_hex_value, x_pos + 80, y_pos, 40.0f)) {
			if (test_brush.hex_brush.brush_hex_value > 255) test_brush.hex_brush.brush_hex_value = 255;
			if (test_brush.hex_brush.brush_hex_value < 0)   test_brush.hex_brush.brush_hex_value = 0;
		}

		y_pos += 30;
		text("Brush Radius", x_pos + 110, y_pos);
		y_pos += 20;
		text("X             Y", x_pos + 90, y_pos);

		y_pos += 20;
		ImGui::SetCursorPosX(50);
		ImGui::SetNextItemWidth(100);
		if (ImGui::DragFloat("###hgbx", &test_brush.hex_brush.radius_2d.x, 0.1f, 0.1f,1000.0f , "%.1f", ImGuiSliderFlags_None)) {
			//test_brush->update_shader_brush_data(); // testing only : no longer needed
		}

		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		if (ImGui::DragFloat("###hgbmy", &test_brush.hex_brush.radius_2d.y, 0.1f, 0.1f,1000.0f , "%.1f", ImGuiSliderFlags_None)) {
			//test_brush->update_shader_brush_data(); // testing only : no longer needed
		}

		if (s_index > -1) {
std::cout << "hex_surface_editor_editing_widget_class::display :: Brush draw initiated " << s_index << std::endl;

			//define_test_brush();

			test_brush.apply_brush(hex_surface_object_to_execute,s_index);
		}
	}


}

void hex_surface_editor_editing_widget_class::hcp_voxel_analysis() {

	// **************** //CoolLibs/imgui_gradient widget tool https://github.com/CoolLibs/imgui_gradient

	text("Hex Value Gradient", x_pos + 85, y_pos);
	y_pos += 30;

	ImGG::interpolation_mode_widget("Interpolation Mode", &hex_analysis_shader->gradient_widget.gradient().interpolation_mode());
	y_pos += 15;

	text("0                                  255", x_pos + 10, y_pos);
	y_pos += 30;

	hex_analysis_shader->gradient_settings.gradient_width    = 256.f;
	hex_analysis_shader->gradient_settings.gradient_height   = 30.f;
	hex_analysis_shader->gradient_settings.horizontal_margin = 10.f;
	hex_analysis_shader->gradient_settings.should_use_a_random_color_for_the_new_marks = false;

	hex_analysis_shader->gradient_settings.flags = ImGG::Flag::ImGuiGradientFlag::NoBorder | ImGG::Flag::ImGuiGradientFlag::NoLabel | ImGG::Flag::ImGuiGradientFlag::NoDragDownToDelete;

	ImGui::SetCursorPosX(15);
	if (hex_analysis_shader->gradient_widget.widget("My Gradient", hex_analysis_shader->gradient_settings)) {
		hex_analysis_shader->update_editor_shader_gradient();
	}

	// ************************************************************************************

	y_pos += 100;
	text("Hex Value Display Range", x_pos + 55, y_pos);
	y_pos += 30;

	hex_analysis_shader->imgui_int_range_widget.offset.x = 20;
	hex_analysis_shader->imgui_int_range_widget.min_text_offset = { -5.0f,5.0f };
	hex_analysis_shader->imgui_int_range_widget.max_text_offset = { -20.0f,5.0f };
	if (hex_analysis_shader->imgui_int_range_widget.display_int_range_widget(256, 0, 255)) {
		hex_analysis_shader->update_editor_shader_range();
	}

	if (checkbox("###hgdrc", "Enable\nRange", x_pos + 290, y_pos - 10, 55.0f, 5.0f, hex_analysis_shader->enable_range)) {
		hex_analysis_shader->update_editor_shader_range();
	}
	// -----------------------------------------------------------------------------
	y_pos += 45;
	if (integer_input("Hex Value##hvewv", hex_analysis_shader->selected_value, x_pos + 80, y_pos, 40.0f)) {
		if (hex_analysis_shader->selected_value > 255) hex_analysis_shader->selected_value = 255;
		if (hex_analysis_shader->selected_value < 0)   hex_analysis_shader->selected_value = 0;
		hex_analysis_shader->update_editor_shader_value();
	}

	ImGui::SetCursorPosX(30);
	ImGui::SetNextItemWidth(255);
	ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0.0f,0.0f,0.0f,1.0f });
	if (ImGui::SliderInt("##hvewv", &hex_analysis_shader->selected_value, 0, 255)) {
		if (hex_analysis_shader->selected_value > 255) hex_analysis_shader->selected_value = 255;
		if (hex_analysis_shader->selected_value < 0)   hex_analysis_shader->selected_value = 0;
		hex_analysis_shader->update_editor_shader_value();
	}

	ImGui::PopStyleColor(1);// Otherwise all widgets after this will have background color black

	if (checkbox("###hgdvc", "Enable\nValue", x_pos + 290, y_pos + 20, 55.0f, 0.0f, hex_analysis_shader->enable_value)) {
		hex_analysis_shader->update_editor_shader_value();
	}
	// -----------------------------------------------------------------------------
	y_pos += 55;

	text("Hex Grid Display Range", x_pos + 70, y_pos);
	y_pos += 30;
	text("Min             Max", x_pos + 75, y_pos);

	y_pos += 30;
	ImGui::SetCursorPosX(50);
	ImGui::SetNextItemWidth(100);
	if (ImGui::DragInt("###hgminx", &hex_analysis_shader->min_matrix_x, 1, 0, hex_surface_object_to_execute->hex_surface_object_data.grid_dimension.x - 1, "%d", ImGuiSliderFlags_AlwaysClamp)) {
		if (hex_analysis_shader->min_matrix_x >= hex_analysis_shader->max_matrix_x) hex_analysis_shader->min_matrix_x =hex_analysis_shader->max_matrix_x - 1;
		hex_analysis_shader->update_editor_shader_matrix_range();
	}
	
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100);
	if (ImGui::DragInt("X###hgmanx", &hex_analysis_shader->max_matrix_x, 1, 1, hex_surface_object_to_execute->hex_surface_object_data.grid_dimension.x, "%d", ImGuiSliderFlags_AlwaysClamp)) {
		if (hex_analysis_shader->max_matrix_x <= hex_analysis_shader->min_matrix_x) hex_analysis_shader->max_matrix_x = hex_analysis_shader->min_matrix_x + 1;
		hex_analysis_shader->update_editor_shader_matrix_range();
	}
	
	y_pos += 30;
	ImGui::SetCursorPosX(50);
	ImGui::SetNextItemWidth(100);
	if (ImGui::DragInt("###hgminy", &hex_analysis_shader->min_matrix_y, 1, 0, hex_surface_object_to_execute->hex_surface_object_data.grid_dimension.y - 1, "%d", ImGuiSliderFlags_AlwaysClamp)) {
		if (hex_analysis_shader->min_matrix_y >= hex_analysis_shader->max_matrix_y) hex_analysis_shader->min_matrix_y =hex_analysis_shader->max_matrix_y - 1;
		hex_analysis_shader->update_editor_shader_matrix_range();
	}
	
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100);
	if (ImGui::DragInt("Y###hgmany", &hex_analysis_shader->max_matrix_y, 1, 1, hex_surface_object_to_execute->hex_surface_object_data.grid_dimension.y, "%d", ImGuiSliderFlags_AlwaysClamp)) {
		if (hex_analysis_shader->max_matrix_y <= hex_analysis_shader->min_matrix_y) hex_analysis_shader->max_matrix_y = hex_analysis_shader->min_matrix_y + 1;
		hex_analysis_shader->update_editor_shader_matrix_range();
	}
	// -----------------------------------------------------------------------------

}

// Adaptation of openframework pointPickerexample to pick and edit hcp voxel matrix values

int hex_surface_editor_editing_widget_class::hcp_voxel_selection() {
	int selected_index = -1;

	if (hex_surface_object_to_execute) {
		cam = globalc::get_current_selected_camera();
		glm::vec2 mouseXY = globalc::get_current_mouseXY();

		if (!cam) {
std::cout << "hex_surface_editor_editing_widget_class:display !cam \n" << std::endl;
		}
//std::cout << "hex_surface_editor_editing_widget_class:display 00000  " << cam->getGlobalPosition().x << " : " << cam->getGlobalPosition().y << " : " << cam->getGlobalPosition().z << std::endl;

		std::vector <hex_surface_data_type> &voxel_matrix_data = hex_surface_object_to_execute->hex_surface_object_data.hex_surface_matrix_data;
		int n = voxel_matrix_data.size();

		float nearestDistance = 0;
		glm::vec2 nearestVertex;
		int nearestIndex = 0;
		
		glm::vec3 mouse(mouseXY.x,mouseXY.y, 0);
		for (int i = 0; i < n; i++) {
			int voxel_value = voxel_matrix_data[i];
			glm::vec3  voxel_cart_coord   = hex_surface_object_to_execute->hex_surface_object_data.get_hex_surface_world_cartesian_coordinate(i);
			glm::vec3 cur = cam->worldToScreen(voxel_cart_coord);

//std::cout << "hex_surface_editor_editing_widget_class:display 11111  " << cur.x << " : " << cur.y << " : " << cur.z << std::endl;
			float distance = glm::distance(cur, mouse);
			if (i == 0 || distance < nearestDistance) {
				nearestDistance = distance;
				nearestVertex = cur;
				nearestIndex = i;
			}
		}

		ofSetColor(ofColor::gray);
		ofDrawLine(nearestVertex, mouse);

		ofNoFill();
		ofSetColor(ofColor::yellow);
		ofSetLineWidth(2);
		ofDrawCircle(nearestVertex, 4);
		ofSetLineWidth(1);

		glm::vec2 offset(10, -10);
		ofDrawBitmapStringHighlight(ofToString(nearestIndex), mouse + offset);

		if (ImGui::IsKeyReleased(ImGuiKey_Space))// Selected index of the hcp voxel matrix data to perform functions on
			selected_index = nearestIndex;
	}

	return selected_index;
}

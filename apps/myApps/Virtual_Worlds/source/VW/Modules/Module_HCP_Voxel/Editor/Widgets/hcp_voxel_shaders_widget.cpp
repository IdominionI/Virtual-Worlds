#include "hcp_voxel_shaders_widget.h"

#include <FrameWork/Kernels/imgui-docking/imgui.h>

#include <Universal_FW/Interface/IconsFontAwesome.h>
#include<Universal_FW/Tools/dialogs.h>

#include <FrameWork/Kernels/ImGuiEx/imgui_widgets.h>

#include <VW/Editor/Main_Window/Panels/log_panel.h>

#include "../../Object/voxel_hcp_object.h"

void voxel_shaders_widget_class::display() {
//std::cout <<"voxel_shaders_widget_class::display() 0000 :current_selected_enity_id < 0 %i\n", current_selected_enity_id);

	initialise_parameters();

	float x_pos = 10.0f, y_pos = 70.0f;

	title("Voxel  Shaders ");

	y_pos += 30;
	text("Display Bounding Volume : ", x_pos + 80, y_pos);

	ImGui::SetCursorPosX(x_pos + 273);
	ImGui::SetCursorPosY(y_pos);
	if (ImGui::Checkbox("###sdisplbv", &display_bounding_volume)) {
		//if (scene_manager != NULL) {
			// Commented out code for possible dissable of function if hcp voxel is not displayed.

			//scene_node_class <render_object_class>* bv_render_object = scene_manager->get_scene_entity_render_object(current_selected_entity_id + BOUNDING_GEOMETRY_OFFSET);


			//if (bv_render_object != NULL)
			//	bv_render_object->scene_graph_object.scene_object_class.visible = display_bounding_volume;
		//}
	}

	y_pos += 20;
	text("Shader Files", x_pos + 100, y_pos);

	y_pos += 20;
	text("Vertex  :", x_pos, y_pos);

	if (ex_button(voxel_shader_parameters->vertex_shader_file_pathname.filename().string().c_str(), x_pos + 80, y_pos, 190, 20))
		select_vertex_shader_file();

	y_pos += 23;
	text("Geometry:", x_pos, y_pos);

	if (ex_button(voxel_shader_parameters->geometry_shader_file_pathname.filename().string().c_str(), x_pos + 80, y_pos, 190, 20))
		select_geometry_shader_file();

	y_pos += 23;
	text("Fragment:", x_pos, y_pos);

	if (ex_button(voxel_shader_parameters->fragment_shader_file_pathname.filename().string().c_str(), x_pos + 80, y_pos, 190, 20))
		select_fragment_shader_file();

	y_pos += 23;
	text("Shader Variables", x_pos + 80, y_pos);

	ImGui::BeginChild("shader_variables", ImVec2(320, 300.0f), true);

	if (shader_variables_widget.variables_manager(voxel_shader_parameters)) update_shader_variables(); // probably inefficient but simple

	ImGui::EndChild();


	y_pos += 330;

	if (ex_button(ICON_FA_STEP_BACKWARD "###vsd", x_pos + 50, y_pos, 50, 20))
		perform_decrement_variables();

	if (ex_button(ICON_FA_EJECT "###vgu", x_pos + 110, y_pos, 80, 20))
		update_voxel_shaders();


	if (ex_button(ICON_FA_STEP_FORWARD "###vsi", x_pos + 200, y_pos, 50, 20))
		perform_increment_variables();

	y_pos += 30;

	if (ex_button("Clear Variables###vgc", x_pos + 85, y_pos, 140, 20))
		clear_variables();

	y_pos += 30;

	if (ex_button(ICON_FA_UPLOAD " Save###vss", x_pos + 35, y_pos, 100, 20))
		save_shader_parameters();

	if (ex_button(ICON_FA_DOWNLOAD " Load" "###vsdl", x_pos + 165, y_pos, 100, 20))
		load_shader_parameters();

}

// NOTE :: to remove slider min max limits when performing increment and decrement funcions
//         comment out the code between 
//			if (<variable>.active_variable_step) {
//		   and
//         <variable>.value +-= <variable>.variable_step; 
//          
void voxel_shaders_widget_class::perform_decrement_variables(float frame_interval) {
//std::cout <<"perform_decrement_variables button clicked\n");// replace with decrement step
	for (shader_parameter_variable_struct_type& variable : voxel_shader_parameters->variables) {
		float var_step = variable.variable_step * frame_interval;
		if (variable.active_variable_step) {
			if (variable.variable_step > 0.0 && variable.value - var_step < variable.slider_min)
				variable.value = variable.slider_min;
			else
				if (variable.variable_step < 0.0 && variable.value - var_step > variable.slider_max)
					variable.value = variable.slider_max;
				else
					variable.value -= var_step;
		}
	}

	for (shader_parameter_int_variable_struct_type& int_variable : voxel_shader_parameters->int_variables) {
		int var_step = int(float(int_variable.variable_step) * frame_interval);
		if (int_variable.active_variable_step) {
			if (int_variable.variable_step > 0 && int_variable.value - var_step < int_variable.slider_min)
				int_variable.value = int_variable.slider_min;
			else
				if (int_variable.variable_step < 0 && int_variable.value - var_step > int_variable.slider_max)
					int_variable.value = int_variable.slider_max;
				else
					int_variable.value -= var_step;
		}
	}

	update_shader_variables();
}

void voxel_shaders_widget_class::perform_increment_variables(float frame_interval) {
//std::cout <<"perform_increment_variables button clicked\n");// replace with decrement step
	for (shader_parameter_variable_struct_type& variable : voxel_shader_parameters->variables) {
		float var_step = variable.variable_step * frame_interval;
		if (variable.active_variable_step) {
			if (variable.variable_step < 0.0 && variable.value + var_step < variable.slider_min)
				variable.value = variable.slider_min;
			else
				if (variable.variable_step > 0.0 && variable.value + var_step > variable.slider_max)
					variable.value = variable.slider_max;
				else
					variable.value += var_step;
		}
	}

	for (shader_parameter_int_variable_struct_type& int_variable : voxel_shader_parameters->int_variables) {
		int var_step = int(float(int_variable.variable_step) * frame_interval);
		if (int_variable.active_variable_step) {
			if (int_variable.variable_step < 0 && int_variable.value + var_step < int_variable.slider_min)
				int_variable.value = int_variable.slider_min;
			else
				if (int_variable.variable_step > 0 && int_variable.value + var_step > int_variable.slider_max)
					int_variable.value = int_variable.slider_max;
				else
					int_variable.value += var_step;
		}
	}

	update_shader_variables();
}

void voxel_shaders_widget_class::update_shader_variables() { // not sure this is needed as render node does this task
//std::cout <<"voxel_shaders_widget_class::update voxel shaders clicked\n");// replace with update variables
	// Get the scene render object that stores the object render properties
	//scene_node_class <render_object_class>* entity_render_object = scene_manager->get_render_object(current_selected_entity_id);
	//if (entity_render_object == NULL) {
	//	if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find object render object node. Cannot update voxel shaders\n");
	//	//std::cout <<"voxel_shaders_widget_class::update_shader_variables :entity_render_object == NULL\n");
	//	return;
	//}
//std::cout <<"voxel_shaders_widget_class::update_shader_variables 111\n");

	//material_struct_type* shader_material = dynamic_cast<material_struct_type*>(entity_render_object->scene_graph_object.scene_object_class.shader_material); // Get the pointer to the shader properties fpr the render process

//std::cout <<"voxel_shaders_widget_class::update_shader_variables 222\n");

	//shader_material->variables      = voxel_shader_parameters->variables;
	//shader_material->int_variables  = voxel_shader_parameters->int_variables;
	//shader_material->bool_variables = voxel_shader_parameters->bool_variables;


//std::cout <<"voxel_shaders_widget_class::update_shader_variables 333\n");
}

void voxel_shaders_widget_class::update_voxel_shaders(bool notification) {
//std::cout << "execute_shader_function button clicked\n";
//std::cout << "voxel_shaders_widget_class::update_voxel_shaders000\n";

	if (voxel_shader_parameters == NULL) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel shader parameter data is undefined. Cannot update voxel shaders\n");
		std::cout <<"voxel_shaders_widget_class :: initialise_parameters : voxel_shader_parameters == NULL\n";
		return;
	}

	if (current_selected_entity_id < 0) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : There is no defined current selected entity. Cannot update voxel shaders\n");
		std::cout <<"voxel_shaders_widget_class::initialise_parameters :current_selected_enity_id < 0 : " << current_selected_entity_id << std::endl;
		return;
	}

//std::cout << "voxel_shaders_widget_class::update_voxel_shaders111\n";

	material_struct_type *shader_material = dynamic_cast<material_struct_type*>(&voxel_hcp_object_to_execute->voxel_object_data.shader_parameters); // Get the pointer to the shader properties fpr the render process

	// ######## ERROR : voxel_shader_parameters some how retains its values and is not cleard from memory when object is deleted and when an import is 
	// ########         performed the parameter data is somehow merged with the imported shader paramter data. Mystery as this does not occure for hex surface class
	// ########         which uses identical data class and code.

	shader_material->vertex_shader_file_pathname   = voxel_shader_parameters->vertex_shader_file_pathname;
	shader_material->geometry_shader_file_pathname = voxel_shader_parameters->geometry_shader_file_pathname;
	shader_material->fragment_shader_file_pathname = voxel_shader_parameters->fragment_shader_file_pathname;

//std::cout << "voxel_shaders_widget_class::update_voxel_shaders 111 " << voxel_hcp_object_to_execute->voxel_object_data.shader_parameters.geometry_shader_file_pathname << "\n";

	shader_material->animate_shaders = voxel_shader_parameters->animate_shaders;

	shader_material->use_default_vertex_shader   = false;
	shader_material->use_default_point_shader    = false;
	shader_material->use_default_geometry_shader = false;
	shader_material->use_default_fragment_shader = false;

	shader_material->variables      = voxel_shader_parameters->variables;
	shader_material->int_variables  = voxel_shader_parameters->int_variables;
	shader_material->bool_variables = voxel_shader_parameters->bool_variables;

	shader_material->define_shader_uniforms();

//std::cout <<"voxel_shaders_widget_class : update_shader_variables :Vertex glsl file  %i : %s\n" , shader_material->use_default_vertex_shader, shader_material->glsl_vertex_shader_file_pathname.c_str() );
//std::cout <<"voxel_shaders_widget_class : update_shader_variables :Geometry glsl file %i :: %i : %s\n" , shader_material->use_default_geometry_shader, shader_material->use_default_point_shader, shader_material->glsl_geometry_shader_file_pathname.c_str());
//std::cout <<"voxel_shaders_widget_class :update_shader_variables :Fragment glsl file %i : %s\n" , shader_material->use_default_fragment_shader, shader_material->glsl_fragment_shader_file_pathname.c_str());


//std::cout <<"voxel_shaders_widget_class::initialise_parameters : XXXXX %s:\n", voxel_shader_parameters->default_point_shader_file_pathname.c_str());
//std::cout <<"voxel_shaders_widget_class::initialise_parameters : YYYYY %i:\n", voxel_shader_parameters->variables.size());

	// Following required as generating a new shader program also needs to up date default shader variables
	// that are defined in the hcp Generation widget
	hcp_voxel_generation_widget->change_voxels_display();
}

bool voxel_shaders_widget_class::initialise_parameters() {
	if (voxel_hcp_object_to_execute == NULL) {
//std::cout <<"voxel_shaders_widget_class :: initialise_parameters :voxel_hcp_object_to_execute == NULL\n");
		return false;
	}

	voxel_shader_parameters = &voxel_hcp_object_to_execute->voxel_object_data.shader_parameters;

//std::cout <<"voxel_shaders_widget_class :: initialise_parameters : AAAAA !!! %s :\n", voxel_shader_parameters->vertex_shader_file_name.c_str());

	if ((voxel_shader_parameters->vertex_shader_file_pathname == ""))
		voxel_shader_parameters->vertex_shader_file_pathname = "...###vsf";

	if ((voxel_shader_parameters->geometry_shader_file_pathname == ""))
		voxel_shader_parameters->geometry_shader_file_pathname = "...###vsf";

	if ((voxel_shader_parameters->fragment_shader_file_pathname == ""))
		voxel_shader_parameters->fragment_shader_file_pathname = "...###vsf";

	return true;
}
//--------------------------------

	void voxel_shaders_widget_class::save_shader_parameters() {

		if (!voxel_shader_parameters) {
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Cannot export shader parameters. No shader parameter defined to export from\n");
			return;
		}

//std::cout <<"save button clicked\n");// replace with clear variables
		char const* patterns[] = { "*.twm" };
		char const* file_pathname = vwDialogs::save_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No compute expresion file defined to save voxel generation parameter data to\n");
			return;
		}
		else
			std::cout <<"save_generation_parameters != NULL  \n";

		voxel_texture_import_export_data.shader_parameters = *voxel_shader_parameters;
		voxel_texture_import_export_data.save_working_generated_function(file_pathname);

		if (log_panel != NULL) log_panel->application_log.AddLog("INFO :Compute expresion voxel generation parameter data saved to file\n %s\n", file_pathname);

	}

	void voxel_shaders_widget_class::load_shader_parameters() {

//std::cout <<"load button clicked\n");// replace with clear variables
		if (!voxel_shader_parameters) {
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Cannot import shader parameters. No shader parameter defined to import into\n");
			return;
		}

		clear_variables();

		char const* patterns[] = { "*.twm" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No compute expresion file defined to import voxel generation parameter data from.\n");
			return;
		}
		else
			std::cout <<"load_generation_parameters != NULL  \n";

		voxel_texture_import_export_data.import_voxel_genereated_function(voxel_shader_parameters, file_pathname);

		if (log_panel != NULL) log_panel->application_log.AddLog("INFO :Compute expresion voxel generation parameter data imported from file\n %s\n", file_pathname);

	}

	void voxel_shaders_widget_class::select_vertex_shader_file() {
//std::cout <<"select_vertex_shader_file button pressed.\n";// replace with get file pathname tool
		char const* patterns[] = { "*_VS.glsl" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No vertex shader file selected.\n");
			return;
		}

//std::cout <<"vertex_shader_file_pathname != NULL %s:\n", file_pathname);

		//change directory deliminator from \ to / for consistency
		std::string s = FW::stringtools::replace(file_pathname, "\\", "/");
		voxel_shader_parameters->vertex_shader_file_pathname = s;
//std::cout <<"select_vertex_shader_file 111111111 : %s:\n",s.c_str());
	}

	void voxel_shaders_widget_class::select_geometry_shader_file() {
//std::cout <<"select_geometry_shader_file button pressed.\n");// replace with get file pathname tool
		char const* patterns[] = { "*_GS.glsl" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No geometry shader file selected.\n");
			return;
		}
//else
//std::cout <<"geometry_shader_file_pathname != NULL %s:\n", file_pathname);

		std::string s = FW::stringtools::replace(file_pathname, "\\", "/");
		voxel_shader_parameters->geometry_shader_file_pathname = s;
	}

	void voxel_shaders_widget_class::select_fragment_shader_file() {
//std::cout <<"select_fragment_shader_file button pressed.\n");// replace with get file pathname tool
		char const* patterns[] = { "*_FS.glsl" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No fragment shader file selected.\n");
			return;
		}
//else
//std::cout <<"vertex_fragment_file_pathname != NULL %s:\n", file_pathname);

		std::string s = FW::stringtools::replace(file_pathname, "\\", "/");
		voxel_shader_parameters->fragment_shader_file_pathname = s;
	}

	void voxel_shaders_widget_class::clear_variables() {
//std::cout <<"Clear Variables clicked";
		voxel_shader_parameters->variables.clear();
		voxel_shader_parameters->int_variables.clear();
		voxel_shader_parameters->bool_variables.clear();
	}

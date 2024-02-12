#include "hex_shaders_widget.h"

#include <FrameWork/Kernels/imgui-docking/imgui.h>

#include <Universal_FW/Interface/IconsFontAwesome.h>

#include <FrameWork/Kernels/ImGuiEx/imgui_widgets.h>
#include <VW/Editor/Tools/dialogs.h>
#include <VW/Editor/Main_Window/Panels/log_panel.h>

	void hex_surface_shaders_widget_class::display() {
//printf("voxel_shaders_widget_class::display() 0000 :current_selected_enity_id < 0 %i\n", current_selected_enity_id);
		initialise_parameters();

		float x_pos = 10.0f, y_pos = 70.0f;

		title("Voxel  Shaders ");

		y_pos += 30;
		text("Display Bounding Area : ", x_pos + 80, y_pos);

		y_pos += 20;
		text("Shader Files", x_pos + 100, y_pos);

		y_pos += 20;
		text("Vertex  :", x_pos, y_pos);

//printf("voxel_shaders_widget_class 0000 %s.\n", hex_surface_shader_parameters.vertex_shader_file_name.c_str());// replace with get file pathname tool;
		if (ex_button(hex_surface_shader_parameters->vertex_shader_file_pathname.filename().string().c_str(), x_pos + 80, y_pos, 190, 20))
			select_vertex_shader_file();

		y_pos += 23;
		text("Geometry :", x_pos, y_pos);

		if (ex_button(hex_surface_shader_parameters->geometry_shader_file_pathname.filename().string().c_str(), x_pos + 80, y_pos, 190, 20))
			select_point_geometry_shader_file();

		y_pos += 23;
		text("Fragment:", x_pos, y_pos);

		if (ex_button(hex_surface_shader_parameters->fragment_shader_file_pathname.filename().string().c_str(), x_pos + 80, y_pos, 190, 20))
			select_fragment_shader_file();

		y_pos += 23;
		text("Shader Variables", x_pos + 80, y_pos);

		ImGui::BeginChild("shader_variables", ImVec2(320, 300.0f), true);

		if (shader_variables_widget.variables_manager(hex_surface_shader_parameters)) update_shader_variables(); // probably inefficient but simple

		ImGui::EndChild();


		y_pos += 330;

		if (ex_button(ICON_FA_STEP_BACKWARD "###hsd", x_pos + 50, y_pos, 50, 20))
			perform_decrement_variables();

		if (ex_button(ICON_FA_EJECT "###hgu", x_pos + 110, y_pos, 80, 20))
			update_hex_shaders();
			

		if (ex_button(ICON_FA_STEP_FORWARD "###hsi", x_pos + 200, y_pos, 50, 20))
			perform_increment_variables();

		y_pos += 30;

		if (ex_button("Clear Variables###hgc", x_pos + 85, y_pos, 140, 20))
			clear_variables();

		y_pos += 30;

		if (ex_button(ICON_FA_UPLOAD " Save###hss", x_pos + 35, y_pos, 100, 20))
			save_shader_parameters();

		if (ex_button(ICON_FA_DOWNLOAD " Load" "###hsdl", x_pos + 165, y_pos, 100, 20))
			load_shader_parameters();

	}

	// NOTE :: to remove slider min max limits when performing increment and decrement funcions
	//         comment out the code between 
	//			if (<variable>.active_variable_step) {
	//		   and
	//         <variable>.value +-= <variable>.variable_step; 
	//          
	void hex_surface_shaders_widget_class::perform_decrement_variables(float frame_interval) {
//printf("perform_decrement_variables button clicked\n");// replace with decrement step
		for (shader_parameter_variable_struct_type &variable : hex_surface_shader_parameters->variables) {
			float var_step = variable.variable_step * frame_interval;
			if (variable.active_variable_step) {
				//if (variable.variable_step > 0.0 && variable.value - variable.variable_step < variable.slider_min)
				if (variable.variable_step > 0.0 && variable.value - var_step < variable.slider_min)
					variable.value = variable.slider_min;
				else
					//if (variable.variable_step < 0.0 && variable.value - variable.variable_step > variable.slider_max)
					if (variable.variable_step < 0.0 && variable.value - var_step > variable.slider_max)
						variable.value = variable.slider_max;
					else
						//variable.value -= variable.variable_step * frame_interval;
						variable.value -= var_step;
			}
		}

		for (shader_parameter_int_variable_struct_type &int_variable : hex_surface_shader_parameters->int_variables) {
			int var_step = int(float(int_variable.variable_step) * frame_interval);
			if (int_variable.active_variable_step) {
				//if (int_variable.variable_step > 0 && int_variable.value - int_variable.variable_step < int_variable.slider_min)
				if (int_variable.variable_step > 0 && int_variable.value - var_step < int_variable.slider_min)
					int_variable.value = int_variable.slider_min;
				else
					//if (int_variable.variable_step < 0 && int_variable.value - int_variable.variable_step > int_variable.slider_max)
					if (int_variable.variable_step < 0 && int_variable.value - var_step > int_variable.slider_max)
						int_variable.value = int_variable.slider_max;
					else
						//int_variable.value -= int_variable.variable_step;
						int_variable.value -= var_step;
			}
		}

		update_shader_variables();
	}

	void hex_surface_shaders_widget_class::perform_increment_variables(float frame_interval) {
//printf("perform_increment_variables button clicked\n");// replace with decrement step
		for (shader_parameter_variable_struct_type &variable : hex_surface_shader_parameters->variables) {
			float var_step = variable.variable_step * frame_interval;
			if (variable.active_variable_step) {
					//if (variable.variable_step < 0.0 && variable.value + variable.variable_step < variable.slider_min)
					if (variable.variable_step < 0.0 && variable.value + var_step < variable.slider_min)
						variable.value = variable.slider_min;
					else
						//if (variable.variable_step > 0.0 && variable.value + variable.variable_step > variable.slider_max)
						if (variable.variable_step > 0.0 && variable.value + var_step > variable.slider_max)
							variable.value = variable.slider_max;
						else
							//variable.value += variable.variable_step * frame_interval;
							variable.value += var_step;
			}
		}

		for (shader_parameter_int_variable_struct_type &int_variable : hex_surface_shader_parameters->int_variables) {
			int var_step = int(float(int_variable.variable_step) * frame_interval);
			if (int_variable.active_variable_step) {
				//if (int_variable.variable_step < 0 && int_variable.value + int_variable.variable_step < int_variable.slider_min)
				if (int_variable.variable_step < 0 && int_variable.value + var_step < int_variable.slider_min)
					int_variable.value = int_variable.slider_min;
				else
					//if (int_variable.variable_step > 0 && int_variable.value + int_variable.variable_step > int_variable.slider_max)
					if (int_variable.variable_step > 0 && int_variable.value + var_step > int_variable.slider_max)
						int_variable.value = int_variable.slider_max;
					else
						//int_variable.value += int_variable.variable_step;
						int_variable.value += var_step;
			}
		}

		update_shader_variables();
	}

	void hex_surface_shaders_widget_class::update_shader_variables() { // not sure this is needed as render node does this task
//printf("voxel_shaders_widget_class::update voxel shaders clicked\n");// replace with update variables
//		// Get the scene render object that stores the object render properties
//		scene_node_class <render_object_class> *entity_render_object = scene_manager->get_render_object(current_selected_entity_id);
//		if (entity_render_object == NULL) {
//			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not find object render object node. Cannot update voxel shaders\n");
////printf("voxel_shaders_widget_class::update_shader_variables :entity_render_object == NULL\n");
//			return;
//		}
////printf("voxel_shaders_widget_class::update_shader_variables 111\n");
//		material_struct_type  *shader_material = dynamic_cast<material_struct_type*>(entity_render_object->scene_graph_object.scene_object_class.shader_material); // Get the pointer to the shader properties fpr the render process
//		
////printf("voxel_shaders_widget_class::update_shader_variables 222\n");
//		shader_material->variables      = hex_surface_shader_parameters->variables;
//		shader_material->int_variables  = hex_surface_shader_parameters->int_variables;
//		shader_material->bool_variables = hex_surface_shader_parameters->bool_variables;
//printf("voxel_shaders_widget_class::update_shader_variables 333\n");
	}

void hex_surface_shaders_widget_class::update_hex_shaders(bool notification) {
//printf("execute_shader_function button clicked\n");

	if (hex_surface_shader_parameters == NULL) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel shader parameter data is undefined. Cannot update voxel shaders\n");
//printf("voxel_shaders_widget_class :: initialise_parameters : hex_surface_shader_parameters == NULL\n");
		return;
	}

	if (current_selected_entity_id < 0) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : There is no defined current selected entity. Cannot update voxel shaders\n");
//printf("voxel_shaders_widget_class::initialise_parameters :current_selected_enity_id < 0 %i\n", current_selected_enity_id);
		return;
	}

	material_struct_type* shader_material = dynamic_cast<material_struct_type*>(&hex_surface_object_to_execute->hex_surface_object_data.hex_surface_shader_parameters); // Get the pointer to the shader properties fpr the render process
															 
//printf("voxel_shaders_widget_class::initialise_parameters : XXXXX %s:\n", hex_surface_shader_parameters->default_point_shader_file_pathname.c_str());
//printf("voxel_shaders_widget_class::initialise_parameters : YYYYY %s:\n", hex_surface_shader_parameters->point_shader_file_pathname.c_str());

	shader_material->vertex_shader_file_pathname = hex_surface_shader_parameters->vertex_shader_file_pathname;

	shader_material->point_shader_file_pathname    = hex_surface_shader_parameters->point_shader_file_pathname;
	shader_material->geometry_shader_file_pathname = hex_surface_shader_parameters->geometry_shader_file_pathname;
	shader_material->fragment_shader_file_pathname = hex_surface_shader_parameters->fragment_shader_file_pathname;

	shader_material->animate_shaders  = hex_surface_shader_parameters->animate_shaders;

	//shader_material->use_default_vertex_shader   = hex_surface_shader_parameters->use_default_vertex_shader;
	//shader_material->use_default_point_shader    = hex_surface_shader_parameters->use_default_point_shader;
	//shader_material->use_default_geometry_shader = hex_surface_shader_parameters->use_default_geometry_shader;
	//shader_material->use_default_fragment_shader = hex_surface_shader_parameters->use_default_fragment_shader;

	shader_material->use_default_vertex_shader   = false;
	shader_material->use_default_point_shader    = false;
	shader_material->use_default_geometry_shader = false;
	shader_material->use_default_fragment_shader = false;

	shader_material->variables      = hex_surface_shader_parameters->variables;
	shader_material->int_variables  = hex_surface_shader_parameters->int_variables;
	shader_material->bool_variables = hex_surface_shader_parameters->bool_variables;

	shader_material->define_shader_uniforms();

	// Following required as generating a new shader program also needs to up date default shader variables
	// that are defined in the Hex Generation widget
	hex_surface_generation_widget->change_hex_display();
}

//bool initialise_parameters(shader_parameters_struct_type *hex_surface_shader_parameters) {
bool hex_surface_shaders_widget_class::initialise_parameters() {
	if (hex_surface_object_to_execute == NULL) {
//printf("voxel_shaders_widget_class :: initialise_parameters :hex_surface_object_to_execute == NULL\n");
		return false;
	}

	hex_surface_shader_parameters = &hex_surface_object_to_execute->hex_surface_object_data.hex_surface_shader_parameters;

//printf("voxel_shaders_widget_class :: initialise_parameters : AAAAA !!! %s :\n", hex_surface_shader_parameters->vertex_shader_file_name.c_str());

	if((hex_surface_shader_parameters->vertex_shader_file_pathname == "")) 
		hex_surface_shader_parameters->vertex_shader_file_pathname = "...###hsf";

	if ((hex_surface_shader_parameters->point_shader_file_pathname == ""))
		hex_surface_shader_parameters->point_shader_file_pathname = "...###hsf1";

	if ((hex_surface_shader_parameters->geometry_shader_file_pathname == ""))
		hex_surface_shader_parameters->geometry_shader_file_pathname = "...###hsf2";

	if ((hex_surface_shader_parameters->fragment_shader_file_pathname == ""))
		hex_surface_shader_parameters->fragment_shader_file_pathname = "...###hsf3";
		
	return true;
}

//-----------------------------------------------

	void hex_surface_shaders_widget_class::save_shader_parameters() {
//printf("save button clicked\n");// replace with clear variables
		char const* patterns[] = {"*.twm"};
		char const* file_pathname = vwDialogs::save_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No compute expresion file defined to save voxel generation parameter data to\n");
			return;
		}
		else
			//printf("save_generation_parameters != NULL %s \n", file_pathname);
			printf("save_generation_parameters != NULL  \n");

		hex_surface_texture_import_export_data.shader_parameters = *hex_surface_shader_parameters;
		hex_surface_texture_import_export_data.save_working_generated_function(file_pathname);

		if (log_panel != NULL) log_panel->application_log.AddLog("INFO :Compute expresion voxel generation parameter data saved to file\n %s\n", file_pathname);
	}

	void hex_surface_shaders_widget_class::load_shader_parameters() {
//printf("load button clicked\n");// replace with clear variables
		clear_variables();

		char const* patterns[] = { "*.twm" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No compute expresion file defined to import voxel generation parameter data from.\n");
			return;
		}
//else
//printf("save_generation_parameters != NULL %s \n", file_pathname);
//printf("load_generation_parameters != NULL  \n");

		hex_surface_texture_import_export_data.import_voxel_genereated_function(hex_surface_shader_parameters, file_pathname);

//printf("hex_surface_shaders_widget_class : load_shader_parameters 000 %s \n",hex_surface_shader_parameters->vertex_shader_file_pathname.filename().string().c_str());
//printf("hex_surface_shaders_widget_class : load_shader_parameters 111 %s \n",hex_surface_shader_parameters->vertex_shader_file_pathname.string().c_str());

		if (log_panel != NULL) log_panel->application_log.AddLog("INFO :Compute expresion voxel generation parameter data imported from file\n %s\n", file_pathname);
	}

	void hex_surface_shaders_widget_class::select_vertex_shader_file() {
		//if (log_panel != NULL) log_panel->application_log.AddLog("INFO : select_vertex_shader_file button pressed.");
//printf("select_vertex_shader_file button pressed.\n");// replace with get file pathname tool
		char const* patterns[] = {"*_VS.glsl"};
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No vertex shader file selected.\n");
			return;
		}

//printf("vertex_shader_file_pathname != NULL %s:\n", file_pathname);

		//change directory deliminator from \ to / for consistency
		std::string s                                        = FW::stringtools::replace(file_pathname, "\\", "/");
		hex_surface_shader_parameters->vertex_shader_file_pathname = s;
//printf("select_vertex_shader_file 111111111 : %s:\n",s.c_str());
		//hex_surface_shader_parameters->vertex_shader_file_name     = vwDialogs::get_filename(s, "/");
	}

	void hex_surface_shaders_widget_class::select_point_geometry_shader_file() {
		//if (log_panel != NULL) log_panel->application_log.AddLog("INFO : select_vertex_shader_file button pressed.");

//printf("select_point_geometry_shader_file button pressed.\n");// replace with get file pathname tool
		char const* patterns[] = { "*_GS.glsl" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No point geometry shader file selected.\n");
			return;
		}

//printf("hex_surface_shaders_widget_class::select_point_geometry_shader_file : 00000 point_geometry_shader_file_pathname != NULL %s:\n", file_pathname);

		//hex_surface_shader_parameters->point_shader_file_pathname = file_pathname;
		std::string s                                            = FW::stringtools::replace(std::string(file_pathname), "\\", "/");
		//hex_surface_shader_parameters->point_shader_file_pathname      = s;
//printf("hex_surface_shaders_widget_class::select_point_geometry_shader_file : 11111 %s:\n", s.c_str());
		hex_surface_shader_parameters->geometry_shader_file_pathname = s;
//printf("hex_surface_shaders_widget_class::select_point_geometry_shader_file : 22222 %s:\n", s.c_str());

	}

	void hex_surface_shaders_widget_class::select_fragment_shader_file() {
		//if (log_panel != NULL) log_panel->application_log.AddLog("INFO : select_vertex_shader_file button pressed.");

//printf("select_fragment_shader_file button pressed.\n");// replace with get file pathname tool
		char const* patterns[] = { "*_FS.glsl" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No fragment shader file selected.\n");
			return;
		}
		//		else
//printf("vertex_fragment_file_pathname != NULL %s:\n", file_pathname);

		std::string s                                          = FW::stringtools::replace(file_pathname, "\\", "/");
		hex_surface_shader_parameters->fragment_shader_file_pathname = s;
	}

	void hex_surface_shaders_widget_class::clear_variables() {
		printf("Clear Variables clicked");// replace with clear variables
		hex_surface_shader_parameters->variables.clear();
		hex_surface_shader_parameters->int_variables.clear();
		hex_surface_shader_parameters->bool_variables.clear();
	}
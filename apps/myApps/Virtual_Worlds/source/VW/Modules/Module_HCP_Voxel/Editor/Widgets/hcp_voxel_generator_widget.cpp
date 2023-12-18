#include "hcp_voxel_generator_widget.h"

#include <FrameWork/Kernels/imgui-docking/imgui.h>

#include <VW_framework/Types/vw_type_definitions.h>
#include <VW/Editor/Main_Window/Widgets/imgui_widgets.h>
#include <VW/Editor/Tools/dialogs.h>
#include <VW/Editor/Interface/IconsFontAwesome.h> //*****

#include "../../Object/voxel_hcp_object.h"

	void voxel_hcp_generation_widget_class::display() {
		if (voxel_hcp_object_to_execute == NULL) {
			return;
		}

		const char* invocation_items[] = { "1", "32", "64", "128", "256", "512", "1024" };// This cannot be defined outside this function	
		//float x_pos = 10.0f, y_pos = 260.0f;
		float x_pos = 10.0f, y_pos = 180.0f;

		text("Expression : ", x_pos, y_pos);
		if (ex_button(voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.expression_file_name.c_str(), x_pos + 120, y_pos, 150, 20))
			get_expression_file();

		y_pos += 25;
		text("Start             End", x_pos + 80, y_pos);

		x_pos = 20; y_pos += 20;

		text("X-axis", x_pos, y_pos);
		float_input("###gxstart", voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.x_start, x_pos + 60, y_pos, 75.0f);
		float_input("###gxend", voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.x_end, x_pos + 190.0f, y_pos, 75.0f);

		y_pos += 20;
		text("Y-axis", x_pos, y_pos);
		float_input("###gystart", voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.y_start, x_pos + 60, y_pos, 75.0f);
		float_input("###gyend", voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.y_end, x_pos + 190.0f, y_pos, 75.0f);

		y_pos += 20;
		text("Z-axis", x_pos, y_pos);
		float_input("###gzstart", voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.z_start, x_pos + 60, y_pos, 75.0f);
		float_input("###gzend", voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.z_end, x_pos + 190.0f, y_pos, 75.0f);

		y_pos += 30;
		text("Compute Invocation :", x_pos, y_pos);

		ImGui::SetCursorPosX(x_pos + 170);
		ImGui::SetCursorPosY(y_pos);
		ImGui::SetNextItemWidth(75);
		if (ImGui::Combo("###invocation", &invocation, &Funcs::ItemGetter, invocation_items, IM_ARRAYSIZE(invocation_items))) {
			define_voxel_generation_parameters();
		}

		y_pos += 30;
		if (ex_button("Execute Function", x_pos + 70, y_pos, 150, 20))
			execute_voxel_function();


		y_pos += 30;
		text("Min\nSurface\nValue", x_pos, y_pos);
		integer_input("###gminsv", voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.min_surface_value, x_pos + 80, y_pos + 15, 50.0f);

		text("Max\nSurface\nValue", x_pos + 150, y_pos);
		integer_input("###gmaxsv", voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.max_surface_value, x_pos + 220, y_pos + 15, 50.0f);

		y_pos += 60;
		text("Resolution Step", x_pos + 30, y_pos);
		float_input("###gresstep", voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.resolution_step, x_pos + 160, y_pos, 75.0f);

		y_pos += 25;
		text("Treshold", x_pos + 30, y_pos);
		float_input("###gthres", voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.generation_threshold, x_pos + 160, y_pos, 75.0f);
/*
		y_pos += 30;
		ImGui::SetCursorPosX(x_pos + 120);
		ImGui::SetCursorPosY(y_pos);
		if(ImGui::Checkbox("Display as points###gdisplpts", &display_as_points)) {
			change_voxels_display();
		}
		
		y_pos += 30;
		text("Voxel Scale", x_pos+130, y_pos);
		
		y_pos += 20;
		if (float_min_max_slider("v", x_pos, y_pos, 200.0f, min_vscale, max_vscale, voxel_scale_value, 50.0f)) {
			update_voxel_size();
		}
*/
		y_pos += 20;
		text("Voxel Generattion Variables", x_pos + 45, y_pos);
		y_pos += 20;

		ImGui::BeginChild("outer_child", ImVec2(320,300.0f), true);

			generator_variables_widget_class.variables_manager(voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters);

		ImGui::EndChild();

		y_pos += 330;

		if (ex_button(ICON_FA_STEP_BACKWARD "###gd", x_pos + 50, y_pos, 50, 20))
			perform_decrement_variables();

		//if (ex_button(ICON_FA_EJECT "###gu", x_pos + 110, y_pos, 80, 20)) // Not needed but just in case
		//	printf("update button clicked");// replace with update variables

		if (ex_button(ICON_FA_STEP_FORWARD "###gi", x_pos + 200, y_pos, 50, 20))
			perform_increment_variables();

		y_pos += 30;

		if (ex_button("Clear Variables###gc", x_pos + 85, y_pos, 140, 20))
			clear_variables();

		y_pos += 30;

		if (ex_button(ICON_FA_UPLOAD " Save###gs", x_pos + 35, y_pos, 100, 20))
			save_generation_parameters();

		if (ex_button(ICON_FA_DOWNLOAD " Load" "###gdl", x_pos + 165, y_pos, 100, 20))
			load_generation_parameters();

	}

	void voxel_hcp_generation_widget_class::perform_decrement_variables() {
//printf("perform_decrement_variables button clicked\n");// replace with decrement step
		for (voxel_generator_parameter_variable_struct_type &variable : voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.variables) {
			if (variable.active_variable_step) variable.value -= variable.variable_step;
		}

		for (voxel_generator_parameter_int_variable_struct_type &int_variable : voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.int_variables) {
			if (int_variable.active_variable_step) int_variable.value -= int_variable.variable_step;
		}

		execute_voxel_function();
	}

	void voxel_hcp_generation_widget_class::perform_increment_variables() {
//printf("perform_increment_variables button clicked\n");// replace with decrement step
		for (voxel_generator_parameter_variable_struct_type &variable : voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.variables) {
			if (variable.active_variable_step) variable.value += variable.variable_step; 
		}

		for (voxel_generator_parameter_int_variable_struct_type &int_variable : voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.int_variables) {
			if (int_variable.active_variable_step) int_variable.value += int_variable.variable_step;
		}

		execute_voxel_function();
	}

	void voxel_hcp_generation_widget_class::execute_voxel_function(bool notification) {
//printf("Execute Function button clicked\n");

		//####### GET  OBJECT DATA THAT HAS PARAMETER DATA AND UPDATE #######

//editor_logging_panel.add_log_message("INFO","Execute Buttton 000 : " + voxel_generation_item.current_selected_component_name);
		if (current_selected_entity_id   == INVALID_ID){
//printf("voxel_hcp_generation_widget_class :: current_selected_entity_id   == -1 #####\n");
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not perform voxel generation. No entity selected.\n");
			return;
		}


		if (voxel_hcp_object_to_execute == NULL) { 
//printf("voxel_hcp_generation_widget_class :: voxel_hcp_object_to_execute == NULL #####\n");
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not perform voxel generation. No entity selected.\n");
			return;
		}
//printf("voxel_hcp_generation_widget_class :: execute_voxel_function 1111\n");
		define_voxel_generation_parameters(); // Only need to define invocation values
//editor_logging_panel.add_log_message("INFO","Execute Buttton 111 : " + voxel_generation_item.current_selected_component_name);
		// set voxel_generation object id to execute funtion data on
		
		//voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters = voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters;
		
		voxel_generator.cloud      = voxel_hcp_object_to_execute;
		voxel_generator.cloud_data = &voxel_hcp_object_to_execute->voxel_object_data;
		voxel_generator.log_panel  = log_panel;
//printf("voxel_hcp_generation_widget_class :: execute_voxel_function %s 2222\n", voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.expression_file_pathname.c_str());



		if (!voxel_generator.generate_voxel_function()) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Create Voxel function of %s failed.\n", voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.expression_file_name.c_str());
			return;
		}

if (log_panel != NULL) log_panel->application_log.AddLog("INFO : After  voxel_generator.generate_voxel_function() ::: %i \n", voxel_hcp_object_to_execute->voxel_object_data.voxel_matrix_data.size());

		// need to define values for min/max voxel value range or have incorrect to misleading display
		voxel_hcp_object_to_execute->set_voxel_value_range(MIN_VOXEL_VALUE, MAX_VOXEL_VALUE);// This is to be changed to a user defined input in a future update
		voxel_hcp_object_to_execute->define_geometry_data();// need to define values for min/max voxel value range or have incorrect to misleading display

if (log_panel != NULL) log_panel->application_log.AddLog("INFO : After  voxel_hcp_object_to_execute->define_geometry_data() !!!! %i \n", voxel_hcp_object_to_execute->geometry->get_number_of_vertices());
//printf("voxel_hcp_generation_widget_class :: execute_voxel_function 4444\n");

		//####### GET RENDER OBJECT THAT HAS GEOMETRY DATA AND UPDATE #######
			shader_class shader;
// **** Update voxel hcp shader variable values define  to be used in all voxel hcp shaders as default
			float vox_size = voxel_hcp_object_to_execute->voxel_object_data.voxel_size * voxel_scale_value;

			shader.set_f1(shader.getProgram(), vox_size, "voxSize");

			shader.set_ivec3(shader.getProgram(), voxel_hcp_object_to_execute->voxel_object_data.matrix_dimension, "voxel_matrix_dimension");

			shader.set_vec3(shader.getProgram(), voxel_hcp_object_to_execute->voxel_object_data.matrix_origin, "voxel_origin");

			float voxel_hcp_z_increment = voxel_hcp_object_to_execute->voxel_object_data.voxel_size * 2.0f * sqrt(6.0f) / 3.0f;
			shader.set_f1(shader.getProgram(), voxel_hcp_z_increment, "voxel_hcp_z_increment");

			//voxel surface display data
			float v_min = (float)voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.min_surface_value;
			shader.set_f1(shader.getProgram(), v_min, "voxel_min_surface_display_value");

			float v_max = (float)voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.max_surface_value;
			shader.set_f1(shader.getProgram(), v_max, "voxel_max_surface_display_value");

//printf("voxel_hcp_generation_widget_class :: execute_voxel_function 77777\n");
		if (notification)
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Voxel Creation From Function Completed.\n");
	}


	void voxel_hcp_generation_widget_class::initialise_parameters() {
		initialise_invocation();
	}

	void voxel_hcp_generation_widget_class::initialise_invocation() {
		switch (voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.invocation) {
			case 1    : invocation = 0; break;
			case 32   : invocation = 1; break;
			case 64   : invocation = 2; break;
			case 128  : invocation = 3; break;
			case 256  : invocation = 4; break;
			case 512  : invocation = 5; break;
			case 1024 : invocation = 6; break;
			default   : invocation = 7; break;
		}
	}

	// test_hex_hexagon_cell_coord_from_cartesian for testing only. Delete when complete
	void voxel_hcp_generation_widget_class::test_voxel_cell_coord_from_cartesian(voxel_object_data_class voxel_object_data) {
		printf("test_hex_hexagon_cell_coord_from_cartesian()000\n");
		glm::vec3 points[10];
/*		// even level test
		points[0] = { 7.05,1.0,0.0 };
		points[1] = { 7.05,1.14,0.0 };
		points[2] = { 7.05,1.2,0.0 };
		points[3] = { 6.95,1.0,0.0 };
		points[4] = { 6.95,1.14,0.0 };
		points[5] = { 6.95,1.2,0.0 };
//		points[6] = { 7.05,1.5,0.0 };
//		points[7] = { 7.05,2.0,0.0 };
//		points[8] = { 0.24,0.0 ,0.0 };
//		points[9] = { 0.35,1.5 ,0.0 };
		points[6] = { 6.95,-1.0,0.0 };
		points[7] = { 6.95,-2.0,0.0 };
		points[8] = { -0.24,0.0 ,0.0 };
		points[9] = { -1.35,1.5 ,0.0 };
*/
/*		// odd level test
		points[0] = { 7.55,0.53,1.5 };
		points[1] = { 7.55,1.153,1.5 };
		points[2] = { 7.55,1.6,1.5 };
		points[3] = { 7.45,0.53,1.5 };
		points[4] = { 7.45,1.153,1.5 };
		points[5] = { 7.45,1.6,1.5 };
//		points[6] = { 7.05,1.5 + 0.577,1.5 };
//		points[7] = { 7.05,2.0 + 0.577,1.5 };
//		points[8] = { 0.24,0.0 + 0.577 ,1.5 };
//		points[9] = { 0.35,1.5 + 0.577 ,1.5 };
		points[6] = { 6.95,-1.0,1.5 };
		points[7] = { 6.95,-2.0,1.5 };
		points[8] = { -0.24,0.0 ,1.5 };
		points[9] = { -1.35,1.5 ,1.5 };
*/
/*		// even to odd level test
		points[0] = { 7.05,0.0 , 0.5 };
		points[1] = { 7.05,0.0 , 1.0 };
		points[2] = { 7.05,0.0, 1.3 };
		points[3] = { 7.00,0.05, 0.5 };
		points[4] = { 7.00,0.05 , 1.0 };
		points[5] = { 7.00,0.05 , 1.3 };
		points[6] = { 6.95,0.0 , 0.5 };
		points[7] = { 6.95,0.0 , 1.0 };
		points[8] = { 6.95,0.0, 1.3 };
		points[9] = { 6.95,0.0 , -1.3 };
*/
		// odd to even level test
		points[0] = { 7.55,0.0 , 2.0 };
		points[1] = { 7.55,0.0 , 2.5 };
		points[2] = { 7.55,0.0, 2.6 };
		points[3] = { 7.45,0.0, 2.0 };
		points[4] = { 7.45,0.0 , 2.5 };
		points[5] = { 7.45,0.0 , 2.6 };
		points[6] = { 7.5,0.55 ,2.0 };
		points[7] = { 7.5,0.55 , 2.5 };
		points[8] = { 7.5,0.55, 2.6 };
		points[9] = { 6.95,0.0 , -1.3 };

		printf("test_voxel_cell_coord_from_cartesian()111\n");
		printf("Origin : x %f : y %f : z %f\n", voxel_object_data.matrix_origin.x, voxel_object_data.matrix_origin.y, voxel_object_data.matrix_origin.z);
		printf("grid dim : x %i : y %i : y %i \n", voxel_object_data.matrix_dimension.x, voxel_object_data.matrix_dimension.y, voxel_object_data.matrix_dimension.z);
		printf("voxel size : %f\n", voxel_object_data.voxel_size);

		glm::ivec3 iv;
		for (int i = 0; i < 10; i++) {
			iv= voxel_object_data.hcp_voxel_cell_coord_from_cartesian(points[i].x, points[i].y, points[i].z);
			printf("point %i x : %f y : %f z : %f :: voxel_coord_from_cartesian  x : %i y : %i z %i\n",i, points[i].x, points[i].y, points[i].z, iv.x, iv.y,iv.z);
		}
	}

//-----------------------------------------------

	void voxel_hcp_generation_widget_class::define_voxel_generation_parameters() {
		switch (invocation) {
			case 0 : voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.invocation = 1;    break;
			case 1 : voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.invocation = 32;   break;
			case 2 : voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.invocation = 64;   break;
			case 3 : voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.invocation = 128;  break;
			case 4 : voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.invocation = 256;  break;
			case 5 : voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.invocation = 512;  break;
			case 6 : voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.invocation = 1024; break;
			default :voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.invocation = 256;  break;
		}
	}

	void voxel_hcp_generation_widget_class::get_expression_file() {
//printf("Expression file button clicked");// replace with get file pathname tool
		char const* patterns[] = { "*_EXPR.txt" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No compute expresion file selected to perform voxel generation\n");
			return;
		}else
			printf("expression_file_pathname != NULL %s \n", file_pathname);

		voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.expression_file_pathname = file_pathname;
		voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.expression_file_name     = vwDialogs::get_filename(file_pathname,"\\");
	}
	
	void voxel_hcp_generation_widget_class::save_generation_parameters() {
//printf("save button clicked\n");// replace with clear variables
		char const* patterns[] = { "*.vgp" };
		char const* file_pathname = vwDialogs::save_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No compute expresion file defined to save voxel generation parameter data to\n");
			return;
		} 
		//else
			//printf("save_generation_parameters != NULL %s \n", file_pathname);
			//printf("save_generation_parameters != NULL  \n");

		switch (invocation) {
			case 0 : voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.invocation = 1;    break;
			case 1 : voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.invocation = 32;   break;
			case 2 : voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.invocation = 64;   break;
			case 3 : voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.invocation = 128;  break;
			case 4 : voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.invocation = 256;  break;
			case 5 : voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.invocation = 512;  break;
			case 6 : voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.invocation = 1024; break;
			default :voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.invocation = 256;  break;
		}

		voxel_function_import_export.voxel_generator_parameters = voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters;

		voxel_function_import_export.save_voxel_generated_function(file_pathname);
		if (log_panel != NULL) log_panel->application_log.AddLog("INFO :Compute expresion voxel generation parameter data saved to file\n %s\n", file_pathname);
	}

	void voxel_hcp_generation_widget_class::load_generation_parameters() {
//printf("load button clicked\n");// replace with clear variables
		clear_variables();

		char const* patterns[] = { "*.vgp" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No compute expresion file defined to import voxel generation parameter data from.\n");
			return;
		}
		//else
		//	printf("load_generation_parameters != NULL  \n");

		voxel_function_import_export.import_voxel_generated_function(voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters, file_pathname);

		switch (voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.invocation) {
			case 1    : invocation = 0; break;
			case 32   : invocation = 1; break;
			case 64   : invocation = 2; break;
			case 128  : invocation = 3; break;
			case 256  : invocation = 4; break;
			case 512  : invocation = 5; break;
			case 1024 : invocation = 6; break;
			default   : invocation = 7; break;
		}
		if (log_panel != NULL) log_panel->application_log.AddLog("INFO :Compute expresion voxel generation parameter data imported from file\n %s\n", file_pathname);
	}

	void voxel_hcp_generation_widget_class::clear_variables() {
		//printf("Clear Variables clicked");// replace with clear variables
		voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.variables.clear();
		voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.int_variables.clear();
		voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.bool_variables.clear();
	}

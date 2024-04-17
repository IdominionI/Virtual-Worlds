#include "hcp_voxel_editor_generator_widget.h"

#include <FrameWork/Kernels/imgui-docking/imgui.h>

#include <VW_framework/Types/vw_type_definitions.h>
#include <FrameWork/Kernels/ImGuiEx/imgui_widgets.h>

#include <Universal_FW/Interface/IconsFontAwesome.h>
#include <Universal_FW/Tools/dialogs.h>

#include "../../Object/voxel_hcp_object.h"

	void voxel_hcp_editor_generation_widget_class::display() {
		if (voxel_hcp_object_to_execute == NULL) {
			return;
		}

		float x_pos = 10.0f, y_pos = 180.0f;

		text("Expression : ", x_pos, y_pos);
		if (ex_button(voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.expression_file_name.c_str(), x_pos + 120, y_pos, 150, 20))
			get_voxel_volume_file();

		y_pos += 25;
		text("Start             End", x_pos + 80, y_pos);

		x_pos = 20; y_pos += 20;

		text("X-axis", x_pos, y_pos);
		float_input("###vexstart", voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.x_start, x_pos + 60, y_pos, 75.0f);
		float_input("###vexend", voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.x_end, x_pos + 190.0f, y_pos, 75.0f);

		y_pos += 20;
		text("Y-axis", x_pos, y_pos);
		float_input("###veystart", voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.y_start, x_pos + 60, y_pos, 75.0f);
		float_input("###veyend", voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.y_end, x_pos + 190.0f, y_pos, 75.0f);

		y_pos += 20;
		text("Z-axis", x_pos, y_pos);
		float_input("###vezstart", voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.z_start, x_pos + 60, y_pos, 75.0f);
		float_input("###vezend", voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.z_end, x_pos + 190.0f, y_pos, 75.0f);

		y_pos += 30;
		if (ex_button("Limits of Voxel Volume###vevl", x_pos + 70, y_pos, 165, 20))
			get_voxel_volume_limits();

		y_pos += 30;
		if (ex_button("Import Voxel Volume###veiv", x_pos + 85, y_pos, 140, 20))
			import_voxel_volume();


		y_pos += 30;
		text("Min\nSurface\nValue", x_pos, y_pos);
		integer_input("###veminsv", voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.min_surface_value, x_pos + 80, y_pos + 15, 50.0f);

		text("Max\nSurface\nValue", x_pos + 150, y_pos);
		integer_input("###vemaxsv", voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.max_surface_value, x_pos + 220, y_pos + 15, 50.0f);

		//invocation is the default surface value used to simplify save/load of paramter file using existing save/load code
		y_pos += 45;
		text("Default\nSurface\nValue", x_pos+80, y_pos);
		integer_input("###vedefsv", voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.invocation, x_pos + 160, y_pos + 15, 50.0f);

		y_pos += 50;
		text("Resolution Step", x_pos + 30, y_pos);
		float_input("###veresstep", voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.resolution_step, x_pos + 160, y_pos, 75.0f);

		y_pos += 30;
		if (ex_button("Create New###venv", x_pos + 85, y_pos, 140, 20))
			create_new_voxel_volume();

		y_pos += 30;
		if (ex_button("Save Expression###vesvv", x_pos + 85, y_pos, 140, 20))
			save_voxel_volume();

		y_pos += 30;

		if (ex_button(ICON_FA_UPLOAD " Save###ves", x_pos + 35, y_pos, 100, 20))
			save_generation_parameters();

		if (ex_button(ICON_FA_DOWNLOAD " Load" "###vedl", x_pos + 165, y_pos, 100, 20))
			load_generation_parameters();

	}

	void voxel_hcp_editor_generation_widget_class::create_new_voxel_volume(bool notification) {
//std::cout <<"create_new_voxel_volume button clicked\n";
		if (voxel_hcp_object_to_execute == NULL) { 
//printf("voxel_hcp_editor_generation_widget_class :: voxel_hcp_object_to_execute == NULL #####\n");
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not perform voxel generation. No entity selected.\n");
			return;
		}

		voxel_hcp_object_to_execute->voxel_object_data.voxel_size = voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.resolution_step;// +++++

//printf("voxel_hcp_editor_generation_widget_class :: execute_voxel_function 1111\n");

		if (! voxel_hcp_object_to_execute->voxel_object_data.create_voxel_matrix(voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.invocation)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Create Voxel volume failed.\n");
			return;
		}

if (log_panel != NULL) log_panel->application_log.AddLog("INFO : After  voxel_generator.generate_voxel_function() ::: %i \n", voxel_hcp_object_to_execute->voxel_object_data.voxel_matrix_data.size());

		// need to define values for min/max voxel value range or have incorrect to misleading display
		voxel_hcp_object_to_execute->set_voxel_value_range(voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.min_surface_value, voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.max_surface_value);// This is to be changed to a user defined input in a future update
		voxel_hcp_object_to_execute->define_geometry_data();// need to define values for min/max voxel value range or have incorrect to misleading display

if (log_panel != NULL) log_panel->application_log.AddLog("INFO : After  voxel_hcp_object_to_execute->define_geometry_data() !!!! %i \n", voxel_hcp_object_to_execute->geometry->get_number_of_vertices());
//printf("voxel_hcp_editor_generation_widget_class :: execute_voxel_function 4444\n");

		//####### GET RENDER OBJECT THAT HAS GEOMETRY DATA AND UPDATE #######
		if(generate_shader_program){
			update_voxel_shaders();
			generate_shader_program = false;
		}

		voxel_hcp_object_to_execute->object_changed = true;
		voxel_hcp_object_to_execute->min_voxel_value = 0;
		voxel_hcp_object_to_execute->max_voxel_value = 255;
		hcp_analysis_shader->update_editor_shader_uniforms();

//printf("voxel_hcp_editor_generation_widget_class :: execute_voxel_function 77777\n");
		if (notification)
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Voxel Creation From Function Completed.\n");

	}

	void voxel_hcp_editor_generation_widget_class::get_voxel_volume_file() {
//printf("Expression file button clicked");// replace with get file pathname tool
		char const* patterns[] = { "*.vvf" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No voxel volume file selected to import\n");
			return;
		}else
			printf("expression_file_pathname != NULL %s \n", file_pathname);

		voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.expression_file_pathname = file_pathname;
		voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.expression_file_name     = vwDialogs::get_filename(file_pathname,"\\");
	}
	
	void voxel_hcp_editor_generation_widget_class::save_generation_parameters() {
//printf("save button clicked\n");// replace with clear variables
		char const* patterns[] = { "*.vep" };
		char const* file_pathname = vwDialogs::save_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No voxel volume file defined to save voxel volume generation parameter data to\n");
			return;
		} 
//else
//printf("save_generation_parameters != NULL %s \n", file_pathname);
//printf("save_generation_parameters != NULL  \n");

		voxel_function_import_export.voxel_generator_parameters = voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters;

		voxel_function_import_export.save_voxel_generated_function(file_pathname);
		if (log_panel != NULL) log_panel->application_log.AddLog("INFO :Voxel volume generation parameter data saved to file\n %s\n", file_pathname);
	}

	void voxel_hcp_editor_generation_widget_class::load_generation_parameters() {
//printf("load button clicked\n");// replace with clear variables
		clear_variables();

		char const* patterns[] = { "*.vep" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No Voxel volume file defined to import voxel volume generation parameter data from.\n");
			return;
		}
//else
//	printf("load_generation_parameters != NULL  \n");

		voxel_function_import_export.import_voxel_generated_function(voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters, file_pathname);

		if (log_panel != NULL) log_panel->application_log.AddLog("INFO :Compute expresion voxel generation parameter data imported from file\n %s\n", file_pathname);
	}

	void voxel_hcp_editor_generation_widget_class::clear_variables() {
//printf("Clear Variables clicked");// replace with clear variables
		voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.variables.clear();
		voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.int_variables.clear();
		voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.bool_variables.clear();
	}

	void voxel_hcp_editor_generation_widget_class::get_voxel_volume_limits() {
		if (voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.expression_file_pathname.size() == 0 ||
			voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.expression_file_name == "...") {

			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Cannot get voxel volume limits: invalid file pathname\n");
			return;
		}
			
		if (!hcp_voxel_editor_import_export.get_voxel_volume_limits(voxel_hcp_object_to_execute->voxel_object_data)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Cannot get voxel volume limits: Potential corrupted file\n");
			return;
		}

		voxel_object_data_class                voxel_object_data = voxel_hcp_object_to_execute->voxel_object_data;
		voxel_generator_parameters_struct_type voxel_generator_parameters = voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters;

		voxel_generator_parameters.resolution_step = voxel_object_data.voxel_size;

		voxel_generator_parameters.x_start = voxel_object_data.matrix_origin.x;
		voxel_generator_parameters.y_start = voxel_object_data.matrix_origin.y;
		voxel_generator_parameters.z_start = voxel_object_data.matrix_origin.z;

		voxel_generator_parameters.x_end = voxel_object_data.matrix_origin.x + voxel_object_data.voxel_size * float(voxel_object_data.matrix_dimension.x)* 2.0f;
		voxel_generator_parameters.y_end = voxel_object_data.matrix_origin.y + voxel_object_data.voxel_size * float(voxel_object_data.matrix_dimension.y)* (3.0f / sqrt(3.0f));
		voxel_generator_parameters.z_end = voxel_object_data.matrix_origin.z + voxel_object_data.voxel_size * float(voxel_object_data.matrix_dimension.z)* 2.0f * sqrt(6.0f) / 3.0f;

		voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters = voxel_generator_parameters;
	}
	
	void voxel_hcp_editor_generation_widget_class::import_voxel_volume() {
		if (voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.expression_file_pathname.size() == 0 ||
			voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.expression_file_name == "...") {

			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Cannot get voxel volume limits: invalid file pathname\n");
			return;
		}

		if (!hcp_voxel_editor_import_export.import_voxel_volume_data(voxel_hcp_object_to_execute->voxel_object_data)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Cannot get voxel volume data: Potential corrupted file\n");
			return;
		}

		voxel_hcp_object_to_execute->set_voxel_value_range(voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.min_surface_value, voxel_hcp_object_to_execute->voxel_object_data.voxel_generator_parameters.max_surface_value);// This is to be changed to a user defined input in a future update
//printf("voxel_hcp_editor_generation_widget_class :: import_voxel_volume 111111 ###\n");
		voxel_hcp_object_to_execute->define_geometry_data();// need to define values for min/max voxel value range or have incorrect to misleading display
//printf("voxel_hcp_editor_generation_widget_class :: import_voxel_volume 222222 ###\n");
//if (log_panel != NULL) log_panel->application_log.AddLog("INFO : After  voxel_hcp_object_to_execute->define_geometry_data() !!!! %i \n", voxel_hcp_object_to_execute->geometry->get_number_of_vertices());
//printf("voxel_hcp_editor_generation_widget_class :: import_voxel_volume 333333 ###\n");

		if(generate_shader_program){
			update_voxel_shaders();
			generate_shader_program = false;
		}

		voxel_hcp_object_to_execute->object_changed = true;
		voxel_hcp_object_to_execute->min_voxel_value = 0;
		voxel_hcp_object_to_execute->max_voxel_value = 255;
		hcp_analysis_shader->update_editor_shader_uniforms();
	}
	
	void voxel_hcp_editor_generation_widget_class::save_voxel_volume() {
		char const* patterns[] = { "*.vvf" };
		char const* file_pathname = vwDialogs::save_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No voxel volume file selected to export\n");
			return;
		}else
			printf("Voxel volume file pathname != NULL %s \n", file_pathname);


		hcp_voxel_editor_import_export.export_voxel_volume(file_pathname, voxel_hcp_object_to_execute->voxel_object_data);
	}

	// +++++++++++++++++++++++++++++++++++++++++++++++
void voxel_hcp_editor_generation_widget_class::update_voxel_shaders() {
//std::cout << "execute_shader_function button clicked\n";
//std::cout << "voxel_shaders_widget_class::update_voxel_shaders000\n";

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

	shader_material->vertex_shader_file_pathname   = (shader_file_directory+hcp_editor_vertex_shader).c_str();
	shader_material->geometry_shader_file_pathname = (shader_file_directory+hcp_editor_geometry_shader).c_str();
	shader_material->fragment_shader_file_pathname = (shader_file_directory+hcp_editor_fragment_shader).c_str();

	shader_material->use_default_vertex_shader   = false;
	shader_material->use_default_point_shader    = false;// No longer used delete ????
	shader_material->use_default_geometry_shader = false;
	shader_material->use_default_fragment_shader = false;

//std::cout <<"voxel_shaders_widget_class : update_shader_variables :Vertex glsl file  %i : %s\n" , shader_material->use_default_vertex_shader, shader_material->glsl_vertex_shader_file_pathname.c_str() );
//std::cout <<"voxel_shaders_widget_class : update_shader_variables :Geometry glsl file %i :: %i : %s\n" , shader_material->use_default_geometry_shader, shader_material->use_default_point_shader, shader_material->glsl_geometry_shader_file_pathname.c_str());
//std::cout <<"voxel_shaders_widget_class :update_shader_variables :Fragment glsl file %i : %s\n" , shader_material->use_default_fragment_shader, shader_material->glsl_fragment_shader_file_pathname.c_str());


//std::cout <<"voxel_shaders_widget_class::initialise_parameters : XXXXX %s:\n", voxel_shader_parameters->default_point_shader_file_pathname.c_str());
//std::cout <<"voxel_shaders_widget_class::initialise_parameters : YYYYY %i:\n", voxel_shader_parameters->variables.size());

	// Following required as generating a new shader program also needs to up date default shader variables
	// that are defined in the Hex Generation widget
	change_voxels_display();
}


void voxel_hcp_editor_generation_widget_class::change_voxels_display() {
			if (voxel_hcp_object_to_execute != NULL) {
				voxel_hcp_object_to_execute->voxel_object_data.shader_parameters.use_point_shader = true;
				//voxel_hcp_object_to_execute->voxel_object_data.shader_parameters.use_point_shader = display_as_points;

//printf("hcp_voxel_generation_widget_class :: change_voxels_display 000 : %i : %i\n", display_as_points, voxel_hcp_object_to_execute->voxel_object_data.shader_parameters.use_point_shader);
				voxel_hcp_object_to_execute->log_panel = log_panel;
				
				voxel_hcp_object_to_execute->update_shader(); // create shader program 

				shader_class shader;
				shader.set_b1(voxel_hcp_object_to_execute->geometry->shader->getProgram(), true, "display_as_points");
			}
	}

	// +++++++++++++++++++++++++++++++++++++++++++++++
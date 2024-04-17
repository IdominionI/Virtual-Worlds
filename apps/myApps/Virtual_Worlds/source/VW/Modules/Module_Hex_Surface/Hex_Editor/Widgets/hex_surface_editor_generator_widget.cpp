#include "hex_surface_editor_generator_widget.h"

#include <FrameWork/Kernels/imgui-docking/imgui.h>

#include <VW_framework/Types/vw_type_definitions.h>
#include <FrameWork/Kernels/ImGuiEx/imgui_widgets.h>

#include <Universal_FW/Interface/IconsFontAwesome.h>
#include <Universal_FW/Tools/dialogs.h>

#include "../../Object/hex_surface_object.h"

	void hex_surface_editor_generation_widget_class::display() {
		if (hex_surface_object_to_execute == NULL) {
			return;
		}

		float x_pos = 10.0f, y_pos = 180.0f;

		text("Expression : ", x_pos, y_pos);
		if (ex_button(hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.expression_file_name.c_str(), x_pos + 120, y_pos, 150, 20))
			get_hex_grid_file();

		y_pos += 25;
		text("Start             End", x_pos + 80, y_pos);

		x_pos = 20; y_pos += 20;

		text("X-axis", x_pos, y_pos);
		float_input("###hgxstart", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.x_start, x_pos + 60, y_pos, 75.0f);
		float_input("###hgxend", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.x_end, x_pos + 190.0f, y_pos, 75.0f);

		y_pos += 20;
		text("Y-axis", x_pos, y_pos);
		float_input("###hgystart", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.y_start, x_pos + 60, y_pos, 75.0f);
		float_input("###hgyend", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.y_end, x_pos + 190.0f, y_pos, 75.0f);

		//y_pos += 20;
		//text("Z-axis", x_pos, y_pos);
		//float_input("###hgzstart", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.z_start, x_pos + 60, y_pos, 75.0f);
		//float_input("###hgzend", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.z_end, x_pos + 190.0f, y_pos, 75.0f);

		y_pos += 30;
		if (ex_button("Limits of Hex Grid###hgvl", x_pos + 70, y_pos, 165, 20))
			get_hex_surface_limits();

		y_pos += 30;
		if (ex_button("Import Hex Grid###hgiv", x_pos + 85, y_pos, 140, 20))
			import_hex_grid();


		y_pos += 30;
		text("Min\nSurface\nValue", x_pos, y_pos);
		//integer_input("###hgminsv", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.min_surface_value, x_pos + 80, y_pos + 15, 50.0f);
		float_input("###hgminsv", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.min_surface_value, x_pos + 80, y_pos + 15, 50.0f);

		text("Max\nSurface\nValue", x_pos + 150, y_pos);
		//integer_input("####hgmaxsv", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.max_surface_value, x_pos + 220, y_pos + 15, 50.0f);
		float_input("####hgmaxsv", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.max_surface_value, x_pos + 220, y_pos + 15, 50.0f);

		//invocation is the default surface value used to simplify save/load of paramter file using existing save/load code
		y_pos += 45;
		text("Default\nSurface\nValue", x_pos+80, y_pos);
		integer_input("###hgdefsv", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.invocation, x_pos + 160, y_pos + 15, 50.0f);

		y_pos += 50;
		text("Resolution Step", x_pos + 30, y_pos);
		float_input("###hgresstep", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.resolution_step, x_pos + 160, y_pos, 75.0f);

		y_pos += 30;
		if (ex_button("Create New###hgnv", x_pos + 85, y_pos, 140, 20))
			create_new_hex_surface();

		y_pos += 30;
		if (ex_button("Save Expression###hgsvv", x_pos + 85, y_pos, 140, 20))
			save_hex_surface();

		y_pos += 30;

		if (ex_button(ICON_FA_UPLOAD " Save###hgs", x_pos + 35, y_pos, 100, 20))
			save_generation_parameters();

		if (ex_button(ICON_FA_DOWNLOAD " Load" "###hgdl", x_pos + 165, y_pos, 100, 20))
			load_generation_parameters();

	}

	void hex_surface_editor_generation_widget_class::create_new_hex_surface(bool notification) {
//std::cout <<"create_new_voxel_volume button clicked\n";
		if (hex_surface_object_to_execute == NULL) { 
//printf("hex_surface_editor_generation_widget_class :: hex_surface_object_to_execute == NULL #####\n");
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Could not perform hex surface generation. No entity selected.\n");
			return;
		}

		hex_surface_object_to_execute->hex_surface_object_data.hex_size = hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.resolution_step;// +++++

//printf("hex_surface_editor_generation_widget_class :: execute_voxel_function 1111\n");

		if (! hex_surface_object_to_execute->hex_surface_object_data.create_hex_grid(float(hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.invocation))) {
		//if (! hex_surface_object_to_execute->hex_surface_object_data.create_hex_grid(128.0f)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Create hex area failed.\n");
			return;
		}

if (log_panel != NULL) log_panel->application_log.AddLog("INFO : After  hex_generator.generate_hex_function() ::: %i \n", hex_surface_object_to_execute->hex_surface_object_data.hex_surface_matrix_data.size());

		// need to define values for min/max voxel value range or have incorrect to misleading display
		hex_surface_object_to_execute->set_hex_surface_value_range(hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.min_surface_value, hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.max_surface_value);// This is to be changed to a user defined input in a future update
		hex_surface_object_to_execute->define_geometry_data();// need to define values for min/max voxel value range or have incorrect to misleading display

if (log_panel != NULL) log_panel->application_log.AddLog("INFO : After  hex_surface_object_to_execute->define_geometry_data() !!!! %i \n", hex_surface_object_to_execute->geometry->get_number_of_vertices());
//printf("hex_surface_editor_generation_widget_class :: execute_voxel_function 4444\n");

		//####### GET RENDER OBJECT THAT HAS GEOMETRY DATA AND UPDATE #######
		if(generate_shader_program){
			update_hex_surface_shaders();
			//generate_shader_program = false;
		}

		hex_surface_object_to_execute->object_changed = true;
		hex_surface_object_to_execute->min_hex_surface_value = 0;
		hex_surface_object_to_execute->max_hex_surface_value = 255;
		hex_analysis_shader->update_editor_shader_uniforms();

//printf("hex_surface_editor_generation_widget_class :: execute_voxel_function 77777\n");
		if (notification)
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Voxel Creation From Function Completed.\n");

	}

	void hex_surface_editor_generation_widget_class::get_hex_grid_file() {
//printf("Expression file button clicked");// replace with get file pathname tool
		char const* patterns[] = { "*.hgs" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No voxel volume file selected to import\n");
			return;
		}else
			printf("expression_file_pathname != NULL %s \n", file_pathname);

		hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.expression_file_pathname = file_pathname;
		hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.expression_file_name     = vwDialogs::get_filename(file_pathname,"\\");
	}
	
	void hex_surface_editor_generation_widget_class::save_generation_parameters() {
//printf("save button clicked\n");// replace with clear variables
		char const* patterns[] = { "*.hep" };
		char const* file_pathname = vwDialogs::save_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No voxel volume file defined to save voxel volume generation parameter data to\n");
			return;
		} 
//else
//printf("save_generation_parameters != NULL %s \n", file_pathname);
//printf("save_generation_parameters != NULL  \n");

		hex_surface_function_import_export.hex_surface_generator_parameters = hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters;

		hex_surface_function_import_export.save_hex_surface_generated_function(file_pathname);
		if (log_panel != NULL) log_panel->application_log.AddLog("INFO :Voxel volume generation parameter data saved to file\n %s\n", file_pathname);
	}

	void hex_surface_editor_generation_widget_class::load_generation_parameters() {
//printf("load button clicked\n");// replace with clear variables
		clear_variables();

		char const* patterns[] = { "*.hep" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No Voxel volume file defined to import voxel volume generation parameter data from.\n");
			return;
		}
//else
//	printf("load_generation_parameters != NULL  \n");

		hex_surface_function_import_export.import_hex_surface_genereated_function(hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters, file_pathname);

		if (log_panel != NULL) log_panel->application_log.AddLog("INFO :Compute expresion voxel generation parameter data imported from file\n %s\n", file_pathname);
	}

	void hex_surface_editor_generation_widget_class::clear_variables() {
////printf("Clear Variables clicked");// replace with clear variables
		hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.variables.clear();
		hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.int_variables.clear();
		hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.bool_variables.clear();
	}

	void hex_surface_editor_generation_widget_class::get_hex_surface_limits() {
		if (hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.expression_file_pathname.size() == 0 ||
			hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.expression_file_name == "...") {

			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Cannot get voxel volume limits: invalid file pathname\n");
			return;
		}
			
		if (!hex_surface_editor_import_export.get_hex_surface_volume_limits(hex_surface_object_to_execute->hex_surface_object_data)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Cannot get voxel volume limits: Potential corrupted file\n");
			return;
		}

		hex_surface_object_data_class                hex_surface_object_data = hex_surface_object_to_execute->hex_surface_object_data;
		hex_surface_generator_parameters_struct_type hex_surface_generator_parameters = hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters;

		hex_surface_generator_parameters.resolution_step = hex_surface_object_data.hex_size;

		hex_surface_generator_parameters.x_start = hex_surface_object_data.grid_origin.x;
		hex_surface_generator_parameters.y_start = hex_surface_object_data.grid_origin.y;
		//hex_surface_generator_parameters.z_start = hex_surface_object_data.grid_origin.z;

		hex_surface_generator_parameters.x_end = hex_surface_object_data.grid_origin.x + hex_surface_object_data.hex_size * float(hex_surface_object_data.grid_dimension.x)* 2.0f;
		hex_surface_generator_parameters.y_end = hex_surface_object_data.grid_origin.y + hex_surface_object_data.hex_size * float(hex_surface_object_data.grid_dimension.y)* (3.0f / sqrt(3.0f));
		//hex_surface_generator_parameters.z_end = hex_surface_object_data.grid_origin.z + hex_surface_object_data.hex_size * float(hex_surface_object_data.matrix_dimension.z)* 2.0f * sqrt(6.0f) / 3.0f;

		hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters = hex_surface_generator_parameters;
	}
	
	void hex_surface_editor_generation_widget_class::import_hex_grid() {
		if (hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.expression_file_pathname.size() == 0 ||
			hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.expression_file_name == "...") {

			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Cannot get voxel volume limits: invalid file pathname\n");
			return;
		}

		if (!hex_surface_editor_import_export.import_hex_surface_volume_data(hex_surface_object_to_execute->hex_surface_object_data)) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Cannot get voxel volume data: Potential corrupted file\n");
			return;
		}

		hex_surface_object_to_execute->set_hex_surface_value_range(hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.min_surface_value, hex_surface_object_to_execute->hex_surface_object_data.hex_surface_generator_parameters.max_surface_value);// This is to be changed to a user defined input in a future update
//printf("hex_surface_editor_generation_widget_class :: import_voxel_volume 111111 ###\n");
		hex_surface_object_to_execute->define_geometry_data();// need to define values for min/max voxel value range or have incorrect to misleading display
//printf("hex_surface_editor_generation_widget_class :: import_voxel_volume 222222 ###\n");
//if (log_panel != NULL) log_panel->application_log.AddLog("INFO : After  hex_surface_object_to_execute->define_geometry_data() !!!! %i \n", hex_surface_object_to_execute->geometry->get_number_of_vertices());
//printf("hex_surface_editor_generation_widget_class :: import_voxel_volume 333333 ###\n");

		if(generate_shader_program){
			update_hex_surface_shaders();
			generate_shader_program = false;
		}

		hex_surface_object_to_execute->object_changed = true;
		hex_surface_object_to_execute->min_hex_surface_value = 0;
		hex_surface_object_to_execute->max_hex_surface_value = 255;
		hex_analysis_shader->update_editor_shader_uniforms();
	}
	
	void hex_surface_editor_generation_widget_class::save_hex_surface() {
		char const* patterns[] = { "*.hgs" };
		char const* file_pathname = vwDialogs::save_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No voxel volume file selected to export\n");
			return;
		}else
			printf("Voxel volume file pathname != NULL %s \n", file_pathname);


		hex_surface_editor_import_export.export_hex_surface_volume(file_pathname, hex_surface_object_to_execute->hex_surface_object_data);
	}

	// +++++++++++++++++++++++++++++++++++++++++++++++
void hex_surface_editor_generation_widget_class::update_hex_surface_shaders() {
//std::cout << "execute_shader_function button clicked\n";
//std::cout << "voxel_shaders_widget_class::update_voxel_shaders000\n";

	if (current_selected_entity_id < 0) {
		if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : There is no defined current selected entity. Cannot update voxel shaders\n");
		std::cout <<"voxel_shaders_widget_class::initialise_parameters :current_selected_enity_id < 0 : " << current_selected_entity_id << std::endl;
		return;
	}

//std::cout << "voxel_shaders_widget_class::update_voxel_shaders111\n";

	material_struct_type *shader_material = dynamic_cast<material_struct_type*>(&hex_surface_object_to_execute->hex_surface_object_data.hex_surface_shader_parameters); // Get the pointer to the shader properties fpr the render process

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
	change_hex_surface_display();
}


void hex_surface_editor_generation_widget_class::change_hex_surface_display() {
			if (hex_surface_object_to_execute != NULL) {
				hex_surface_object_to_execute->hex_surface_object_data.hex_surface_shader_parameters.use_point_shader = true;
				//hex_surface_object_to_execute->hex_surface_object_data.shader_parameters.use_point_shader = display_as_points;

//printf("hcp_voxel_generation_widget_class :: change_voxels_display 000 : %i : %i\n", display_as_points, hex_surface_object_to_execute->hex_surface_object_data.shader_parameters.use_point_shader);
				hex_surface_object_to_execute->log_panel = log_panel;
				
				hex_surface_object_to_execute->update_shader(); // create shader program 

				shader_class shader;
				shader.set_b1(hex_surface_object_to_execute->geometry->shader->getProgram(), true, "display_as_points");
			}
	}

	// +++++++++++++++++++++++++++++++++++++++++++++++
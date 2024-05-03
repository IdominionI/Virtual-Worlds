#pragma once

#include <VW/Editor/Main_Window/Panels/log_panel.h>

#include "../../Object/DataTypes/dt_hex_surface_generator.h"
#include "../../Kernal/hex_surface_function_import_export.h"

#include "../hex_surface_editor_import_export_.h"

#include "hex_surface_analysis_shader.h"// ++++
/*
		HCP Voxel Volume generation widget class

	This class widget defines an ImGui widget that are used
	to define the parameters and variables required to 
	generate a 3D HCP Voxel Volume to be edited.
*/

class hex_surface_object_class;
class hex_surface_editor_generation_widget_class {
public:
	log_panel_class				     *log_panel = NULL;

	float voxel_scale_value = 1.0f;
	
	//id_type                  current_selected_entity_id   = -1;  // entity id of the selected entity to display/modify
	int                        current_selected_entity_id  = -1;  // entity id of the selected entity to display/modify
	hex_surface_object_class  *hex_surface_object_to_execute = NULL; // Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model

	hex_analysis_shader_class *hex_analysis_shader;

	void display();
	void create_new_hex_surface(bool notification = true);
	void load_generation_parameters();// testing only delete comment out when done
private:
	//float height_scale_value = 0.0;// ++++
	float min_height_scale_value = 0.0;// ++++
	float max_height_scale_value = 1.0;// ++++

	hex_surface_function_import_export_class hex_surface_function_import_export;
	hex_surface_editor_import_export_class   hex_surface_editor_import_export;

	void get_hex_surface_limits();
	void import_hex_grid();
	void save_hex_surface();


	void get_hex_grid_file();
	void save_generation_parameters();

	void clear_variables();

	bool generate_shader_program = true;

	std::string shader_file_directory      = "Assets/Shaders/Default/Hex_surface/";

	std::string hcp_editor_vertex_shader   = "default_editor_gen_VS.glsl";// test only  : change when finished
	std::string hcp_editor_geometry_shader = "default_editor_gen_GS.glsl";// test only  : change when finished
	std::string hcp_editor_fragment_shader = "default_editor_gen_FS.glsl";// test only  : change when finished

	void update_hex_surface_shaders();
	void change_hex_surface_display();
	//void update_hex_height();// +++
};
#pragma once

#include <VW/Editor/Main_Window/Panels/log_panel.h>

#include "../../Object/DataTypes/dt_voxel_generator.h"
#include "../../Kernal/voxel_function_import_export.h"

#include "../hcp_voxel_editor_import_export_.h"

#include "hcp_voxel_analysis_shader.h"// ++++
/*
		HCP Voxel Volume generation widget class

	This class widget defines an ImGui widget that are used
	to define the parameters and variables required to 
	generate a 3D HCP Voxel Volume to be edited.
*/

class voxel_hcp_object_class;
class voxel_hcp_editor_generation_widget_class {
public:
	log_panel_class				     *log_panel = NULL;

	float voxel_scale_value = 1.0f;
	
	//id_type                  current_selected_entity_id   = -1;  // entity id of the selected entity to display/modify
	int                      current_selected_entity_id  = -1;  // entity id of the selected entity to display/modify
	voxel_hcp_object_class  *voxel_hcp_object_to_execute = NULL; // Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model

	hcp_analysis_shader_class *hcp_analysis_shader;

	void display();
	void create_new_voxel_volume(bool notification = true);
	void load_generation_parameters();// testing only delete comment out when done
private:
	voxel_function_import_export_class   voxel_function_import_export;
	hcp_voxel_editor_import_export_class hcp_voxel_editor_import_export;

	void get_voxel_volume_limits();
	void import_voxel_volume();
	void save_voxel_volume();


	void get_voxel_volume_file();
	void save_generation_parameters();

	void clear_variables();

	bool generate_shader_program = true;

	std::string shader_file_directory      = "Assets/Shaders/Default/Voxel/";

	std::string hcp_editor_vertex_shader   = "default_editor_gen_VS.glsl";// test only  : change when finished
	std::string hcp_editor_geometry_shader = "default_editor_gen_GS.glsl";// test only  : change when finished
	std::string hcp_editor_fragment_shader = "default_editor_gen_FS.glsl";// test only  : change when finished

	void update_voxel_shaders();
	void change_voxels_display();
};
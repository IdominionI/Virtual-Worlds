#pragma once

#include <string>

#include <VW_framework/Shader/shader_components.h>
#include <VW/Editor/Main_Window/Panels/log_panel.h>

/*
			HCP voxel volume Render Class

	This class mamages the generation and deletion of the shader program
	that is to be used to display the 3D HCP voxel volume that an instance
	of this class is asociated with.

	The user selects through the application the files that contain the
	glsl code to be compiled and when an update to the shader program is
	called, a new shader program is compiled, and if successful, the older
	shader progrmam is destroyed, and the new shader program assigned to
	the hex surface object render variable.
*/

class voxel_hcp_object_class;
class voxel_hcp_editor_render_class {
public:
	material_struct_type *voxel_material  = NULL;

	std::string shader_basis_code_directory_pathname = "Assets/Shaders/Default/Voxel/Shader_basis_code/";

	std::string default_shader_file_directory   = "Assets/Shaders/Default/Voxel/";
	std::string shader_file_directory           = "Assets/Shaders/Default/Voxel/";

	std::string voxel_hcp_editor_default_vertex_file   = "default_editor_gen_VS.glsl";
	std::string voxel_hcp_editor_default_geometry_file = "default_editor_gen_GS.glsl";
	std::string voxel_hcp_editor_default_fragment_file = "default_editor_gen_FS.glsl";

	std::string voxel_hcp_editor_vertex_file           = "default_editor_gen_VS.glsl";
	std::string voxel_hcp_editor_geometry_file         = "default_editor_gen_GS.glsl";
	std::string voxel_hcp_editor_fragment_file         = "default_editor_gen_FS.glsl";


	bool define_editor_shader_program(voxel_hcp_object_class *voxel_hcp_object, log_panel_class* log_panel);
	
private:
	//std::vector<float> vv;
};

#include "hcp_voxel_editor_render.h"

#include "../../Object/voxel_hcp_object.h"

// !!!!!!!!!!!!!!!!!!! This may all need to change !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	bool voxel_hcp_editor_render_class::define_editor_shader_program(voxel_hcp_object_class *voxel_hcp_object, log_panel_class *log_panel) {
		material_struct_type *shader_material = &voxel_hcp_object->voxel_object_data.shader_parameters;
		ofShader             *shader          = voxel_hcp_object->geometry->shader;
		bool shaders_loaded = false;

		shader_material->vertex_shader_file_pathname           = (default_shader_file_directory + voxel_hcp_editor_default_vertex_file).c_str();
		shader_material->default_vertex_shader_file_pathname   = (default_shader_file_directory + voxel_hcp_editor_default_vertex_file).c_str();
		shader_material->geometry_shader_file_pathname         = (default_shader_file_directory + voxel_hcp_editor_default_geometry_file).c_str();
		shader_material->default_geometry_shader_file_pathname = (default_shader_file_directory + voxel_hcp_editor_default_geometry_file).c_str();
		shader_material->fragment_shader_file_pathname		   = (default_shader_file_directory + voxel_hcp_editor_default_fragment_file).c_str();
		shader_material->default_fragment_shader_file_pathname = (default_shader_file_directory + voxel_hcp_editor_default_fragment_file).c_str();

//printf("define_initial_shader_program :Vertex glsl file %s\n"   , shader_material->default_vertex_shader_file_pathname.string().c_str() );
//printf("define_initial_shader_program :Geometry glsl file %s\n" , shader_material->default_point_shader_file_pathname.string().c_str());
//printf("define_initial_shader_program :Fragment glsl file %s\n" , shader_material->default_fragment_shader_file_pathname.string().c_str());

		if (shader == NULL) {
			shader = voxel_hcp_object->geometry->create_shader();
		} else {
			if (voxel_hcp_object->geometry->has_internal_shader()) {
				voxel_hcp_object->geometry->delete_shader();
				shader = voxel_hcp_object->geometry->create_shader();

				if (shader == NULL) {
					if (log_panel != NULL) log_panel->code_log.AddLog("ERROR : Could not create shader program.\n");
					return false;
				}
			}
		}
		
		if (!ofIsGLProgrammableRenderer()) {
			if (log_panel != NULL) log_panel->code_log.AddLog("ERROR : No openframworks progrmmable renderer available to display grapichs.\n");
			return false;
		}

		voxel_hcp_object->geometry->init();// Critical to have this function initialted or nothing will be displayed using a geometry shader.

		shader_material->glsl_vertex_shader_file_pathname   = shader_material->default_vertex_shader_file_pathname;
		shader_material->glsl_geometry_shader_file_pathname = shader_material->default_geometry_shader_file_pathname;
		shader_material->glsl_fragment_shader_file_pathname = shader_material->default_fragment_shader_file_pathname;

		shaders_loaded = shader->load(shader_material->glsl_vertex_shader_file_pathname, shader_material->glsl_fragment_shader_file_pathname, shader_material->glsl_geometry_shader_file_pathname);
//shaders_loaded = shader->load(shader_material->glsl_vertex_shader_file_pathname, shader_material->glsl_fragment_shader_file_pathname);// Testing only  comment out or delete when testing complete

		if (!shaders_loaded) {
//printf("voxel_hcp_render_class: shader program not created\n");
//printf("compile log \n %s \n", shader_db_manager.shader.compile_log.c_str());

			if (log_panel != NULL) log_panel->code_log.AddLog("INFO : Shader program compilation failed\n %s\n", shader->compile_log.c_str());
			return false;
		} else {
//printf("voxel_hcp_render_class : shader program created : %i\n", shader_id);
			shader_material->shader_program_id = shader->getProgram();
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Shader program created of ID : %i\n", shader_material->shader_program_id);
		}//shader program created

		return true;
	}

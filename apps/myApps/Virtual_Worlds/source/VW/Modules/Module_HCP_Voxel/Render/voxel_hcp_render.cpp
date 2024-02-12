#include "voxel_hcp_render.h"

#include "../Object/voxel_hcp_object.h"

	bool voxel_hcp_render_class::define_shader_program(voxel_hcp_object_class *voxel_hcp_object, log_panel_class *log_panel) {
		material_struct_type *shader_material = &voxel_hcp_object->voxel_object_data.shader_parameters;
		ofShader             *shader          = voxel_hcp_object->geometry->shader;// +++++
		bool shaders_loaded = false;

//std::cout << "VERTEX SOURCE 01\n" << shader_material->default_vertex_shader_file_pathname << "END VERTEX SOURCE\n";
//std::cout << "VERTEX SOURCE 02\n" << shader_material->vertex_shader_file_pathname << "END VERTEX SOURCE\n";
//printf("POINT SOURCE \n%s END POINT SOURCE\n", vw_point_geometry_shader.shader_code.c_str());

//std::cout << "voxel_hcp_render_class::define_shader_program 000 : " << shader_material->use_point_shader << std::endl;

		shader_material->glsl_vertex_shader_file_pathname = shader_material->vertex_shader_file_pathname;

		shader_material->glsl_geometry_shader_file_pathname = shader_material->geometry_shader_file_pathname;

		shader_material->glsl_fragment_shader_file_pathname = shader_material->fragment_shader_file_pathname;

		// Compile the OpenGL shader and store the id reference to it to be used
		if (shader_material->use_point_shader)
			shaders_loaded = shader->load(shader_material->glsl_vertex_shader_file_pathname, shader_material->glsl_fragment_shader_file_pathname);
		else
			shaders_loaded = shader->load(shader_material->glsl_vertex_shader_file_pathname, shader_material->glsl_fragment_shader_file_pathname, shader_material->glsl_geometry_shader_file_pathname);

//std::cout << "voxel_hcp_render_class::define_shader_program 222\n";
		if (!shaders_loaded) {
//std::cout << "voxel_hcp_render_class::define_shader_program 333\n";
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR :  Unable to compile shader program\n");
			log_panel->display_code_errors(shader->compile_log);
//printf("define_shader_program 22222DDDD\n");
			return false;
		} else {
//std::cout << "voxel_hcp_render_class::define_shader_program 444 : " << std::endl;
			shader_material->shader_program_id = shader->getProgram();
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Shader program created of ID : %i\n", shader_material->shader_program_id);
//printf("define_shader_program 44444 %i\n", entity_render_object->scene_graph_object.scene_object_class.shader_material.shader_program_id);
		}//shader program created


//std::cout << "voxel_hcp_render_class::define_shader_program 555 : " << std::endl;
		return true;
	}


	//void voxel_hcp_render_class::define_shader_variables(scene_node_class <render_object_class> *entity_render_object) {
		// TO DO 
		/*


				shader_parameter_variable_struct_type v1, v2, v3;
				v1.variable_name = "roughness"; v1.value = 0.2f;
				v2.variable_name = "metallic"; v2.value = 0.1f;
				v3.variable_name = "ao"; v3.value = 1.0f;
				test_mesh->scene_graph_object.scene_object_class.shader_material.variables.push_back(v1);
				test_mesh->scene_graph_object.scene_object_class.shader_material.variables.push_back(v2);
				test_mesh->scene_graph_object.scene_object_class.shader_material.variables.push_back(v3);
		*/
	//}

//-----------------------------------

	// !!!!!!!!!!!!!!!!!!! This may all need to change !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	bool voxel_hcp_render_class::define_initial_shader_program(voxel_hcp_object_class *voxel_hcp_object, log_panel_class *log_panel) {
		material_struct_type *shader_material = &voxel_hcp_object->voxel_object_data.shader_parameters;
		ofShader             *shader          = voxel_hcp_object->geometry->shader;// +++++
		bool shaders_loaded = false;

		shader_material->vertex_shader_file_pathname           = (default_shader_file_directory + voxel_hcp_default_vertex_file).c_str();
		shader_material->default_vertex_shader_file_pathname   = (default_shader_file_directory + voxel_hcp_default_vertex_file).c_str();
		shader_material->point_shader_file_pathname		       = (default_shader_file_directory + voxel_hcp_default_point_file).c_str();
		shader_material->default_point_shader_file_pathname    = (default_shader_file_directory + voxel_hcp_default_point_file).c_str();
		shader_material->geometry_shader_file_pathname         = (default_shader_file_directory + voxel_hcp_default_geometry_file).c_str();
		shader_material->default_geometry_shader_file_pathname = (default_shader_file_directory + voxel_hcp_default_geometry_file).c_str();
		shader_material->fragment_shader_file_pathname		   = (default_shader_file_directory + voxel_hcp_default_fragment_file).c_str();
		shader_material->default_fragment_shader_file_pathname = (default_shader_file_directory + voxel_hcp_default_fragment_file).c_str();

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

		if (voxel_hcp_object->voxel_object_data.shader_parameters.use_point_shader) {
			shader_material->glsl_vertex_shader_file_pathname   = shader_material->default_vertex_shader_file_pathname;
			shader_material->glsl_geometry_shader_file_pathname = shader_material->default_geometry_shader_file_pathname;
			shader_material->glsl_fragment_shader_file_pathname = shader_material->default_fragment_shader_file_pathname;
		} else {
			shader_material->glsl_vertex_shader_file_pathname   = shader_material->default_vertex_shader_file_pathname;
			shader_material->glsl_geometry_shader_file_pathname = shader_material->default_geometry_shader_file_pathname;
			shader_material->glsl_fragment_shader_file_pathname = shader_material->default_fragment_shader_file_pathname;
		}

		shader_material->glsl_geometry_shader_file_pathname = shader_material->default_point_shader_file_pathname;// Testing only comment out or delete when testing complete
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

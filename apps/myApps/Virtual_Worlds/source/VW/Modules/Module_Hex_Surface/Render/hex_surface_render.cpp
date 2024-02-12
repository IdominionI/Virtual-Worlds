#include "hex_surface_render.h"

#include "../Object/hex_surface_object.h"

	bool hex_surface_render_class::define_shader_program(hex_surface_object_class* hex_surface_object, log_panel_class* log_panel) {
		material_struct_type *shader_material = &hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters;
		ofShader             *shader          = hex_surface_object->geometry->shader;
		bool                  shaders_loaded  = false;

//printf("VERTEX SOURCE \n%s END VERTEX SOURCE\n",vw_vertex_shader.shader_code.c_str());
//printf("POINT SOURCE \n%s END POINT SOURCE\n", vw_point_geometry_shader.shader_code.c_str());

		// Copile the OpenGL shader and store the id reference to it to be used

//std::cout <<"hex_surface_render_class::define_shader_program 00AAA : " << shader_material->use_point_shader << ":" << shader_material->vertex_shader_file_pathname << std::endl;
//std::cout <<"hex_surface_render_class::define_shader_program 00BBB : " << shader_material->use_point_shader << ":" << shader_material->geometry_shader_file_pathname << std::endl;
//std::cout <<"hex_surface_render_class::define_shader_program 00CCC : " << shader_material->use_point_shader << ":" << shader_material->fragment_shader_file_pathname << std::endl;

		shader_material->glsl_vertex_shader_file_pathname = shader_material->vertex_shader_file_pathname;
		shader_material->glsl_geometry_shader_file_pathname = shader_material->geometry_shader_file_pathname;
		shader_material->glsl_fragment_shader_file_pathname = shader_material->fragment_shader_file_pathname;

//std::cout <<"hex_surface_render_class::define_shader_program 11AAA : " << shader_material->use_point_shader << ":" << shader_material->glsl_vertex_shader_file_pathname << std::endl;
//std::cout <<"hex_surface_render_class::define_shader_program 11BBB : " << shader_material->use_point_shader << ":" << shader_material->glsl_geometry_shader_file_pathname << std::endl;
//std::cout <<"hex_surface_render_class::define_shader_program 11CCC : " << shader_material->use_point_shader << ":" << shader_material->glsl_fragment_shader_file_pathname << std::endl;

//printf("define_shader_program 0000\n");

		// Compile the OpenGL shader and store the id reference to it to be used
		if (shader_material->use_point_shader)
			shaders_loaded = shader->load(shader_material->glsl_vertex_shader_file_pathname, shader_material->glsl_fragment_shader_file_pathname);
		else
			shaders_loaded = shader->load(shader_material->glsl_vertex_shader_file_pathname, shader_material->glsl_fragment_shader_file_pathname, shader_material->glsl_geometry_shader_file_pathname);

		if (!shaders_loaded) {// Shader compilation did not succeed
//printf("define_shader_program 22222\n");
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR :  Unable to compile hex surface shader program\n");

//printf("define_shader_program 22222BBBB \n");
//if(log_panel == NULL )printf("define_shader_program 22222BBBB log_panel == NULL");
//log_panel->display_code_log_code(compile_code);
//printf("define_shader_program 22222CCCC\n");
//log_panel->display_code_errors(shader_db_manager.shader.compile_log);
			log_panel->display_code_errors(shader->compile_log);
//printf("define_shader_program 22222DDDD\n");
			return false;
		} else {// Shader compilation successful
//printf("define_shader_program 33333 %i\n", shader_id);
			shader_material->shader_program_id = shader->getProgram();
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Shader program created of ID : %i\n", shader_material->shader_program_id);

//printf("define_shader_program 44444 %i\n", entity_render_object->scene_graph_object.scene_object_class.shader_material.shader_program_id);
		}//shader program created
		return true;
	}

//----------------------------------------------------

	bool hex_surface_render_class::define_initial_shader_program(hex_surface_object_class *hex_surface_object, log_panel_class *log_panel) {
std::cout << "hex_surface_render_class::define_initial_shader_program :: 0000 \n";
		if (!hex_surface_object) {
			std::cout << "ERROR : CRITCAL :: hex_surface_render_class::define_initial_shader_program :: have null hex surface object : \n";
			return false;
		}

		material_struct_type *shader_material = &hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters;
//std::cout << "hex_surface_render_class::define_initial_shader_program :: 0000AAAAA \n";
		ofShader             *shader          = hex_surface_object->geometry->shader;
//std::cout << "hex_surface_render_class::define_initial_shader_program :: 0000BBBBB \n";
		bool                  shaders_loaded  = false;

//std::cout << "hex_surface_render_class::define_initial_shader_program :: 1111 \n";

		shader_material->vertex_shader_file_pathname           = (default_shader_file_directory + hex_surface_default_vertex_file).c_str();
		shader_material->default_vertex_shader_file_pathname   = (default_shader_file_directory + hex_surface_default_vertex_file).c_str();
		shader_material->point_shader_file_pathname			   = (default_shader_file_directory + hex_surface_default_point_file).c_str();
		shader_material->default_point_shader_file_pathname    = (default_shader_file_directory + hex_surface_default_point_file).c_str();
		shader_material->fragment_shader_file_pathname		   = (default_shader_file_directory + hex_surface_default_fragment_file).c_str();
		shader_material->default_fragment_shader_file_pathname = (default_shader_file_directory + hex_surface_default_fragment_file).c_str();

//std::cout << "hex_surface_render_class::define_initial_shader_program :: 2222 \n";
//printf("define_initial_shader_program :Vertex glsl file \n%s\n"   , shader_material->default_vertex_shader_file_pathname.string().c_str() );
//printf("define_initial_shader_program :Geometry glsl file \n%s\n" , shader_material->default_point_shader_file_pathname.string().c_str());
//printf("define_initial_shader_program :Fragment glsl file \n%s\n" , shader_material->default_fragment_shader_file_pathname.string().c_str());
		// Copile the OpenGL shader and store the id reference to it to be used
		if (shader == NULL) {
			shader = hex_surface_object->geometry->create_shader();
		}
		else {
			if (hex_surface_object->geometry->has_internal_shader()) {
				hex_surface_object->geometry->delete_shader();
				shader = hex_surface_object->geometry->create_shader();

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

		hex_surface_object->geometry->init();// Critical to have this function initialted or nothing will be displayed using a geometry shader.

		if (shader_material->use_point_shader) {
			shader_material->glsl_vertex_shader_file_pathname   = shader_material->default_vertex_shader_file_pathname;
			shader_material->glsl_geometry_shader_file_pathname = shader_material->default_point_shader_file_pathname;
			shader_material->glsl_fragment_shader_file_pathname = shader_material->default_fragment_shader_file_pathname;
		}
		else {
			shader_material->glsl_vertex_shader_file_pathname   = shader_material->default_vertex_shader_file_pathname;
			shader_material->glsl_geometry_shader_file_pathname = shader_material->default_geometry_shader_file_pathname;
			shader_material->glsl_fragment_shader_file_pathname = shader_material->default_fragment_shader_file_pathname;
		}

		shader_material->glsl_geometry_shader_file_pathname = shader_material->default_point_shader_file_pathname;// Testing only comment out or delete when testing complete
		shaders_loaded = shader->load(shader_material->glsl_vertex_shader_file_pathname, shader_material->glsl_fragment_shader_file_pathname, shader_material->glsl_geometry_shader_file_pathname);

		if (!shaders_loaded) {
printf("hex_surface_render_class: shader program not created\n");
printf("compile log \n %s \n", shader_db_manager.shader.compile_log.c_str());
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : hex surface shader program compilation failed\n");
			if (log_panel != NULL) log_panel->code_log.AddLog("INFO : Shader program compilation failed\n %s\n", shader_db_manager.shader.compile_log.c_str());
			return false;
		}
		else {
printf("hex_surface_render_class : shader program created : %i\n", shader->getProgram());

			shader_material->shader_program_id = shader->getProgram();
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Shader program created of ID : %i\n", shader_material->shader_program_id);

			//entity_render_object->scene_graph_object.scene_object_class.shader_material->shader_program_id = shader_id;
		}//shader program created

printf("hex_surface_render_class : shader program created EEEENNNNDDDD : \n");
		// copy shader data used to render voxel object to voxel shader parameter data
		// Note : There are no existing user defined variables for default shader programs
		hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters = *shader_material;

		return true;
	}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/*
		//bool hex_surface_render_class::define_shader_program(scene_node_class <render_object_class> *entity_render_object, log_panel_class *log_panel) {
		shader_format_class* shader_material = entity_render_object->scene_graph_object.scene_object_class.shader_material;

		//printf("VERTEX SOURCE \n%s END VERTEX SOURCE\n",vw_vertex_shader.shader_code.c_str());
		//printf("POINT SOURCE \n%s END POINT SOURCE\n", vw_point_geometry_shader.shader_code.c_str());

				// Copile the OpenGL shader and store the id reference to it to be used
				// Define which shaders to use and the pathname to the shader snippit  code to define the shader
		if (shader_material->use_default_vertex_shader)
			shader_material->glsl_vertex_shader_file_pathname = shader_material->default_vertex_shader_file_pathname;
		else
			shader_material->glsl_vertex_shader_file_pathname = shader_material->vertex_shader_file_pathname;

		if (shader_material->use_point_shader) {
			if (shader_material->use_default_point_shader)
				shader_material->glsl_geometry_shader_file_pathname = shader_material->default_point_shader_file_pathname;
			else
				shader_material->glsl_geometry_shader_file_pathname = shader_material->point_shader_file_pathname;
		}
		else { // Use geometry shader
			if (shader_material->use_default_geometry_shader)
				shader_material->glsl_geometry_shader_file_pathname = shader_material->default_geometry_shader_file_pathname;
			else
				shader_material->glsl_geometry_shader_file_pathname = shader_material->geometry_shader_file_pathname;
		}

		if (shader_material->use_default_fragment_shader)
			shader_material->glsl_fragment_shader_file_pathname = shader_material->default_fragment_shader_file_pathname;
		else
			shader_material->glsl_fragment_shader_file_pathname = shader_material->fragment_shader_file_pathname;

		GLuint shader_id = 0;
		//printf("define_shader_program 0000\n");

		shader_id = shader_db_manager.create_glsl_shader_program(*shader_material);

		if (!shader_id) {// Shader compilation did not succeed
			//printf("define_shader_program 22222\n");
			if (log_panel != NULL) log_panel->application_log.AddLog("ERROR :  Unable to compile shader program\n");

			//printf("define_shader_program 22222BBBB \n");
			//if(log_panel == NULL )printf("define_shader_program 22222BBBB log_panel == NULL");
						//log_panel->display_code_log_code(compile_code);
			//printf("define_shader_program 22222CCCC\n");
						//log_panel->display_code_errors(shader_db_manager.shader.compile_log);
			log_panel->display_code_errors(shader_db_manager.shader.compile_log);
			//printf("define_shader_program 22222DDDD\n");
			return false;
		}
		else {// Shader compilation successful
			//printf("define_shader_program 33333 %i\n", shader_id);
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Shader program created of ID : %i\n", shader_id);

			//printf("define_shader_program 44444 %i\n", entity_render_object->scene_graph_object.scene_object_class.shader_material.shader_program_id);
			shader_material->shader_program_id = shader_id;
			//printf("define_shader_program 55555 %i\n", entity_render_object->scene_graph_object.scene_object_class.shader_material.shader_program_id);
		}//shader program created
		return true;
	}

	//----------------------------------------------------

		//bool hex_surface_render_class::define_initial_shader_program(scene_node_class <render_object_class>* entity_render_object,hex_surface_object_class *hex_surface_object, log_panel_class *log_panel) {
	bool define_initial_shader_program(hex_surface_object_class* hex_surface_object, log_panel_class* log_panel) {
		material_struct_type* shader_materialm = dynamic_cast<material_struct_type*>(entity_render_object->scene_graph_object.scene_object_class.shader_material);
		shader_format_class* shader_material = entity_render_object->scene_graph_object.scene_object_class.shader_material;

		shader_material->vertex_shader_file_pathname = (default_shader_file_directory + voxel_hcp_default_vertex_file).c_str();
		shader_material->default_vertex_shader_file_pathname = (default_shader_file_directory + voxel_hcp_default_vertex_file).c_str();
		shader_material->point_shader_file_pathname = (default_shader_file_directory + voxel_hcp_default_point_file).c_str();
		shader_material->default_point_shader_file_pathname = (default_shader_file_directory + voxel_hcp_default_point_file).c_str();
		shader_material->fragment_shader_file_pathname = (default_shader_file_directory + voxel_hcp_default_fragment_file).c_str();
		shader_material->default_fragment_shader_file_pathname = (default_shader_file_directory + voxel_hcp_default_fragment_file).c_str();

		//printf("define_initial_shader_program :Vertex glsl file \n%s\n"   , shader_material->default_vertex_shader_file_pathname.string().c_str() );
		//printf("define_initial_shader_program :Geometry glsl file \n%s\n" , shader_material->default_point_shader_file_pathname.string().c_str());
		//printf("define_initial_shader_program :Fragment glsl file \n%s\n" , shader_material->default_fragment_shader_file_pathname.string().c_str());
				// Copile the OpenGL shader and store the id reference to it to be used
		GLuint shader_id = 0;

		if (shader_material->use_point_shader) {
			shader_material->glsl_vertex_shader_file_pathname = shader_material->default_vertex_shader_file_pathname;
			shader_material->glsl_geometry_shader_file_pathname = shader_material->default_point_shader_file_pathname;
			shader_material->glsl_fragment_shader_file_pathname = shader_material->default_fragment_shader_file_pathname;

			shader_id = shader_db_manager.create_glsl_shader_program(*shader_material);
		}
		else {
			shader_material->glsl_vertex_shader_file_pathname = shader_material->default_vertex_shader_file_pathname;
			shader_material->glsl_geometry_shader_file_pathname = shader_material->default_geometry_shader_file_pathname;
			shader_material->glsl_fragment_shader_file_pathname = shader_material->default_fragment_shader_file_pathname;

			shader_id = shader_db_manager.create_glsl_shader_program(*shader_material);
		}

		if (!shader_id) {
			//printf("hex_surface_render_class: shader program not created\n");
			//printf("compile log \n %s \n", shader_db_manager.shader.compile_log.c_str());
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : hex surface shader program compilation failed\n");
			if (log_panel != NULL) log_panel->code_log.AddLog("INFO : Shader program compilation failed\n %s\n", shader_db_manager.shader.compile_log.c_str());
			return false;
		}
		else {
			//printf("hex_surface_render_class : shader program created : %i\n", shader_id);
			if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Shader program created of ID : %i\n", shader_id);

			//entity_render_object->scene_graph_object.scene_object_class.shader_material.shader_program_id = shader_id;
			entity_render_object->scene_graph_object.scene_object_class.shader_material->shader_program_id = shader_id;
		}//shader program created


		// copy shader data used to render voxel object to voxel shader parameter data
		// Note : There are no existing user defined variables for default shader programs
		hex_surface_object->hex_surface_object_data.hex_surface_shader_parameters = *shader_materialm;

		return true;
	}
	*/
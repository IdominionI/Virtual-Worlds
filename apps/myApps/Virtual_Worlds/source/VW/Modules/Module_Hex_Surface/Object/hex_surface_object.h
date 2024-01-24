#pragma once

#include <VW_Framework/Object/vw_object_base.h>
#include <VW_framework/Geometry/vw_point_cloud.h>

#include <VW/Editor/Main_Window/Panels/log_panel.h>


//#include <Source/Graphics_Engine/Scene/Scene_objects/camera_object.h>

#include "hex_surface_data_storage.h"

#include "../Render/hex_surface_render.h"
//#include <Source/Graphics_Engine/Scene/Scene_objects/bounding_area_object.h>

enum class hex_surface_vertex_order_enum { none, face, points, line };

/*
	hex_surface_object_class

	C++ class that represents and manages a hex surface data object to be displayed and
	processed in and by the Virtual Worlds application.

*/

#define SCENE_CATEGORY_HEX_SURFACE "Hex_Surface"

class hex_surface_object_class : public vw_object_base_class {
public:
	hex_surface_object_class() {
		// Perhaps create one instance of voxel_hcp_parameters_widget_class and have all voxel_hcp_object_class 
		// parameter_widget point to it as does not need to be unique for each voxel_hcp_object_class if no data
		// is stored. Second thought data may need to be stored within this class !!!!
		//voxel_hcp_parameters_widget_class *pw = new voxel_hcp_parameters_widget_class; // --------------
		//parameter_widget = pw; // --------------

		point_cloud = new point_cloud3D_class;
		geometry = point_cloud;
	}

	~hex_surface_object_class() {
		delete_object();// ++++
	}

	log_panel_class                  *log_panel = NULL;

	//point_cloud3D_class               point_cloud;
	point_cloud3D_class              *point_cloud = nullptr;
	std::vector<int>                  vertex_indicies;
	hex_surface_object_data_class     hex_surface_object_data;
	hex_surface_render_class          hex_surface_render; // Need to change code to edit or not to use this
	//camera_object_class              *camera = NULL;

	int number_faces = 0;
	
	hex_surface_vertex_order_enum  point_cloud_order = hex_surface_vertex_order_enum::none;

	///bounding_area_class bounding_area;

	bool display_hex_surface_object_as_points = false;
		bool  animate_automata = false;

	glm::uvec4 color                          = {1.0,1.0,1.0,1.0};

	float min_surface_display_value, max_surface_display_value;

	int min_hex_surface_value = 0, max_hex_surface_value = 0;

	void delete_object() {
		delete parameter_widget;
		delete geometry;

		parameter_widget = nullptr;
		geometry         = nullptr;
	}

	void clear_shader_variables();

	void define_initial_shader_program() {
		hex_surface_render.define_initial_shader_program(this, log_panel);
	}

	void update_shader() {
		hex_surface_render.define_shader_program(this, log_panel);
	}

	void update_shader_variables() {
		hex_surface_object_data.hex_surface_shader_parameters.update_shader_variables();
	}

	bool perform_animation_frame_functions(int frame) {
		return automata_rule_within_frame(frame);
	}

	bool automata_rule_within_frame(int frame);
	//bool define_vbo_vertices(int min_hex_surface_value, int  max_hex_surface_value);
	bool define_geometry_data();
	bool add_hex_surface_vbo_vertex(int index);
	int  define_vbo_faces();
	glm::vec3 calculate_normal_vector(glm::vec3& vertex_0, glm::vec3& vertex_1, glm::vec3& vertex_2);
	bool define_vbo_point_normals();
	bool define_vbo_face_normals();
	void update_bounding_area();

};

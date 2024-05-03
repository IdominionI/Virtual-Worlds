#pragma once

#include <Universal_FW/Interface/imgui_range_widget.h>
#include <Universal_FW/Interface/imgui_gradient/include/imgui_gradient/imgui_gradient.hpp>

#include "../../Object/hex_surface_object.h"

class hex_analysis_shader_class {
public:
	int                       current_selected_entity_id  = -1;  // entity id of the selected entity to display/modify
	hex_surface_object_class *hex_surface_object_to_execute = nullptr; // Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model

	ofShader    *oshader = nullptr;
	shader_class shader;

	int selected_value = 1;

	bool enable_range  = true;
	bool enable_value  = false;
	bool enable_edit   = false;

	int min_matrix_x = 0, max_matrix_x = 1;
	int min_matrix_y = 0, max_matrix_y = 1;
	int min_matrix_z = 0, max_matrix_z = 1;

	glm::vec3 min_matrix_coord = {-1.0f,-1.0f,-1.0f}, max_matrix_coord = {1.0f,1.0f,1.0f};

	float height_scale_value = 0.0f;// ++++

	imgui_int_range_widget_class   imgui_int_range_widget;

	//CoolLibs/imgui_gradient widget tool https://github.com/CoolLibs/imgui_gradient
	ImGG::GradientWidget gradient_widget{};
	ImGG::Settings       gradient_settings{};

	void update_editor_shader_uniforms() {
		if (!hex_surface_object_to_execute) return;

		min_matrix_x = 0;
		min_matrix_y = 0;
		min_matrix_z = 0;

		max_matrix_x = hex_surface_object_to_execute->hex_surface_object_data.grid_dimension.x;
		max_matrix_y = hex_surface_object_to_execute->hex_surface_object_data.grid_dimension.y;
		max_matrix_z = hex_surface_object_to_execute->hex_surface_object_data.grid_dimension.z;

		update_editor_shader_gradient();
		update_editor_shader_range();	
		update_editor_shader_value();
		update_editor_shader_matrix_range();
		update_editor_shader_hex_height();
	}

	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	void update_editor_shader_gradient() {
		ImGG::Gradient grad_list = gradient_widget.gradient();
		std::list<ImGG::Mark> gradient_marks = grad_list.get_marks();

		//ofShader *oshader = hex_surface_object_to_execute->geometry->shader;// +++++
		//shader_class shader;

		oshader = hex_surface_object_to_execute->geometry->shader;

//std::cout << "hcp_voxel_editor_editing_widget_class:: update_editor_shader_gradient " << gradient_marks.size() << std::endl;

		// ---------------- Gradient uniforms ------------------------------
		//ImGG::Interpolation interpolation_mode = gradient_widget.gradient().interpolation_mode();
		
		shader.set_i1(oshader->getProgram(),gradient_marks.size(),"number_gradient_marks");
		if(gradient_widget.gradient().interpolation_mode() == ImGG::Interpolation::Constant)
			shader.set_i1(oshader->getProgram(),0,"gradient_interpolation");
		else
			shader.set_i1(oshader->getProgram(),1,"gradient_interpolation");

		int i = 0; // Have to do this because kave list not vector used to store gradient mark element
		for (ImGG::Mark gradient_mark : gradient_marks) {
			float  pos = gradient_mark.position.get();
			ImVec4 col = gradient_mark.color;
			glm::vec4 gcol= { col.x,col.y,col.z,col.w };


//std::cout << "hcp_voxel_editor_editing_widget_class:: update_editor_shader_gradient " << pos << ":" << i << std::endl;
			std::string gradient_mark_location = "gradient_mark_location[" + std::to_string(i) + "]";
			std::string gradient_mark_color    = "gradient_mark_color[" + std::to_string(i) + "]";

			shader.set_f1(oshader->getProgram(),pos, gradient_mark_location.c_str());
			shader.set_vec4(oshader->getProgram(),gcol,gradient_mark_color.c_str());
			i++;
		}

//std::cout << "hcp_voxel_editor_editing_widget_class:: update_editor_shader_gradient " << enable_range << ":" << imgui_int_range_widget.current_min_frame << " : "<< imgui_int_range_widget.current_max_frame << std::endl;
	}

	void update_editor_shader_range() {
		oshader = hex_surface_object_to_execute->geometry->shader;
		// ---------------- Range uniforms ------------------------------
		shader.set_i1(oshader->getProgram(),enable_range,"enable_range");
		shader.set_i1(oshader->getProgram(),imgui_int_range_widget.current_min_frame,"min_range_value");
		shader.set_i1(oshader->getProgram(),imgui_int_range_widget.current_max_frame,"max_range_value");
	}

	void update_editor_shader_value() {
		oshader = hex_surface_object_to_execute->geometry->shader;
		// ---------------- Value uniforms -----------------------------
		shader.set_i1(oshader->getProgram(),enable_value,"enable_value");
		shader.set_i1(oshader->getProgram(),selected_value,"selected_value");
	}

	void update_editor_shader_matrix_range() {
		oshader = hex_surface_object_to_execute->geometry->shader;
		// ---------------- Voxel Volume range uniforms ------------------------
		min_matrix_coord = hex_surface_object_to_execute->hex_surface_object_data.get_hex_surface_world_cartesian_coordinate({min_matrix_x,min_matrix_y,min_matrix_z});
		max_matrix_coord = hex_surface_object_to_execute->hex_surface_object_data.get_hex_surface_world_cartesian_coordinate({max_matrix_x,max_matrix_y,max_matrix_z});

		shader.set_vec3(oshader->getProgram(),min_matrix_coord, "min_matrix_coord");
		shader.set_vec3(oshader->getProgram(),max_matrix_coord, "max_matrix_coord");
	}

	void update_editor_shader_hex_height() {
		if (hex_surface_object_to_execute != NULL) {

//printf("hcp_voxel_generation_widget_class :: change_voxels_display 000 : %i : %i\n", display_as_points, hex_surface_object_to_execute->hex_surface_object_data.shader_parameters.use_point_shader);

			shader_class shader;
			shader.set_f1(hex_surface_object_to_execute->geometry->shader->getProgram(), height_scale_value, "hex_height_scale");
		}
	}

};
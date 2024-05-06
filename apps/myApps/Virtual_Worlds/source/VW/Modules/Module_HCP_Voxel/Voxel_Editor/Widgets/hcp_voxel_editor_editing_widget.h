#pragma once
#include <cmath>

#include <VW/Editor/Main_Window/Panels/log_panel.h>

//#include <Universal_FW/Interface/imgui_range_widget.h>
//#include <Universal_FW/Interface/imgui_gradient/include/imgui_gradient/imgui_gradient.hpp>

#include <VW_framework/3D/vw_camera.h>

#include "hcp_voxel_analysis_shader.h"

#include "../hcp_voxel_brush.h"
//#include "../../Object/voxel_hcp_object.h" // included in hcp_voxel_brush.h

#define MAX_NUMBER_GRADIENT_MARKS 100

class hcp_voxel_editor_editing_widget_class {
public:
	log_panel_class	   *log_panel = nullptr;
	//int selected_value = 1;

	float voxel_scale_value = 1.0f;
	
	int                     current_selected_entity_id  = -1;  // entity id of the selected entity to display/modify
	voxel_hcp_object_class *voxel_hcp_object_to_execute = nullptr; // Pointer to the hcp voxel entity data stored in the Virtual Worlds scene data model
	vw_camera_class        *cam;

	hcp_analysis_shader_class *hcp_analysis_shader;

	void display();

	// +++++++++++ test brush +++++++++++++++++++
	//define a test 3d hcp voxel brush to test basic functions

	bool brush_changed = true;

	hcp_voxel_brush_class test_brush;

	void define_test_brush() {
		test_brush.hcp_voxel_3d_brush.type = voxel_3d_brush_type_enum::cubic;
		test_brush.hcp_voxel_3d_brush.radius_3d = { 1.0f,1.0f,1.0f };
		test_brush.hcp_voxel_3d_brush.brush_voxel_value = 128;

		voxel_brush_falloff_struct_type falloff0;
		voxel_brush_falloff_struct_type falloff1;

		falloff0.set_falloff_radius(0.0f);
		falloff0.set_falloff_value(1.0f);

		falloff1.set_falloff_radius(1.0f);
		falloff1.set_falloff_value(1.0f);

		test_brush.hcp_voxel_3d_brush.define_brush_falloff_element(falloff0);
		test_brush.hcp_voxel_3d_brush.define_brush_falloff_element(falloff1);
	}

	// +++++++++++++++++++++++++++++++++++++++++
private:
	//voxel_object_data_class &voxel_object_data;// using this creates compilation error !!!
	float x_pos = 10.0f, y_pos = 70.0f;
	
	bool enable_edit   = false;

	void hcp_voxel_analysis();
	int  hcp_voxel_selection();

	void save_gradient();
	void load_gradient();
};
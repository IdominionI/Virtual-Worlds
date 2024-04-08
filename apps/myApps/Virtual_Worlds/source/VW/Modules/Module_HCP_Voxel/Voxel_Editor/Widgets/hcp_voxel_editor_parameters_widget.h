#pragma once

#include <iostream>

#include <imgui-docking/imgui.h>

#include <VW_framework/Widgets/parameter_widget_base.h>

#include "hcp_voxel_generation_editor_widget.h"
#include "hcp_voxel_editor_editing_widget.h"

/*
				HCP voxel volume editor widget

	This widget class is a parent widget that manages the display
	of HCP voxel volume editor widget

	This widget class is thus the widget class that is called by
	the main application editor to display a hcp voxel editor widget.
*/

class voxel_hcp_parameters_editor_widget_class : public parameter_widget_base_class {
public:
	voxel_hcp_parameters_editor_widget_class() {
		hcp_analysis_shader = new hcp_analysis_shader_class;
	}

	~voxel_hcp_parameters_editor_widget_class() {
		delete hcp_analysis_shader;
	}

	void display_parameters_widget();

private:
	hcp_voxel_generation_editor_widget_class hcp_voxel_generation_widget;
	hcp_voxel_editor_editing_widget_class    hcp_voxel_editor_editing_widget;

	hcp_analysis_shader_class *hcp_analysis_shader =nullptr;

	void display_voxel_generation_widget(voxel_hcp_object_class *voxel_hcp_object_to_execute, id_type current_selected_object_id);
	void display_voxel_editing_widget(voxel_hcp_object_class* voxel_hcp_object_to_execute, id_type current_selected_object_id);
};

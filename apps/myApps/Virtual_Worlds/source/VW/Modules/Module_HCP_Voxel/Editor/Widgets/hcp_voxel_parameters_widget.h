#pragma once

#include <iostream>

#include <imgui-docking/imgui.h>

#include <VW_framework/Widgets/parameter_widget_base.h>

// Including any of the following generates linking errors unkown why
#include "hcp_voxel_generation_widget.h"
#include "hcp_voxel_shaders_widget.h"

/*
				HCP voxel volume paramters widget

	This widget class is a parent widget that manages the display
	of HCP voxel generation, shader variable and cellula automata child
	widgets via selection of ImGui tabs.

	This widget class is thus the widget class that is called by
	the main application editor to display hex surface parameter
	data to be changed or generate data.
*/

class voxel_hcp_parameters_widget_class : public parameter_widget_base_class {
public:

	void display_parameters_widget();

private:

	voxel_shaders_widget_class		   voxel_shaders_widget;

	hcp_voxel_generation_widget_class hcp_voxel_generation_widget;

	void display_voxel_generation_widget(voxel_hcp_object_class *voxel_hcp_object_to_execute, id_type current_selected_object_id);
	void display_voxel_shader_widget(voxel_hcp_object_class* voxel_hcp_object_to_execute, id_type current_selected_object_id);
};

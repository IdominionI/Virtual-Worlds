#pragma once

#include <iostream>

#include <imgui-docking/imgui.h>

#include <VW_framework/Widgets/parameter_widget_base.h>

#include "hex_surface_generation_editor_widget.h"
#include "hex_surface_editor_editing_widget.h"

/*
				HCP voxel volume editor widget

	This widget class is a parent widget that manages the display
	of HCP voxel volume editor widget

	This widget class is thus the widget class that is called by
	the main application editor to display a hcp voxel editor widget.
*/

class hex_surface_parameters_editor_widget_class : public parameter_widget_base_class {
public:
	hex_surface_parameters_editor_widget_class() {
		hex_analysis_shader = new hex_analysis_shader_class;
	}

	~hex_surface_parameters_editor_widget_class() {
		delete hex_analysis_shader;
	}

	void display_parameters_widget();

private:
	hex_surface_generation_editor_widget_class hex_surface_generation_widget;
	hex_surface_editor_editing_widget_class    hex_surface_editor_editing_widget;

	hex_analysis_shader_class *hex_analysis_shader =nullptr;

	void display_hex_surface_generation_widget(hex_surface_object_class *voxel_hcp_object_to_execute, id_type current_selected_object_id);
	void display_hex_surface_editing_widget(hex_surface_object_class* voxel_hcp_object_to_execute, id_type current_selected_object_id);
};

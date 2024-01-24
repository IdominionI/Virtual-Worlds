#pragma once
#include <imgui-docking/imgui.h>
#include <VW_framework/Widgets/parameter_widget_base.h>

#include "hex_generation_selection_widget.h"
#include "hex_shaders_widget.h"

/*
				Hex surface paramters widget

	This widget class is a parent widget that manages the display
	of hex generation, shader variable and cellula automata child
	widgets via selection of ImGui tabs.

	This widget class is thus the widget class that is called by
	the main application editor to display hex surface parameter
	data to be changed or generate data.
*/

class hex_surface_parameters_widget_class : public parameter_widget_base_class {
public:
	log_panel_class     *log_panel     = NULL;

	void display_parameters_widget();

private:
	hex_surface_shaders_widget_class              hex_surface_shaders_widget;
	hex_surface_generation_selection_widget_class hex_surface_generation_selection_widget;

	void display_hex_surface_generation_selection_widget(hex_surface_object_class* hex_surface_object_to_execute, id_type current_selected_object_id);
	void display_hex_surface_shader_widget(hex_surface_object_class* hex_surface_object_to_execute, id_type current_selected_object_id);
};